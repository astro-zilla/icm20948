#include "Invn/Devices/Drivers/Icm20948/Icm20948.h"
#include "Invn/Devices/Drivers/Icm20948/Icm20948MPUFifoControl.h"
#include "Invn/EmbUtils/Message.h"

#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>

#include "main.h"
#include "trigger.h"

static inline void icm20948_set_interrupts(const struct device *dev, bool enable)
{
    struct inv_icm20948 *s = dev->data;
    const struct icm20948_config *cfg = dev->config;

	unsigned int flags = enable
		? GPIO_INT_EDGE_TO_ACTIVE
		: GPIO_INT_DISABLE;
	
    if (enable) inv_icm20948_write_mems_reg(s, REG_INT_ENABLE, 4, s->int_enables);
    else {
        inv_icm20948_read_mems_reg(s, REG_INT_ENABLE, 4, s->int_enables);
        inv_icm20948_write_mems_reg(s, REG_INT_ENABLE, 4, (unsigned char[4]){0});
    }
	gpio_pin_interrupt_configure_dt(&cfg->int_gpio, flags);
}

static void icm20948_gpio_callback(const struct device *dev,
				  struct gpio_callback *cb, uint32_t pins)
{
    struct inv_icm20948 *s = dev->data;
    icm20948_set_interrupts(dev, false);

#if defined(CONFIG_ICM20948_TRIGGER_OWN_THREAD)
	k_sem_give(&s->gpio_sem);
#elif defined(CONFIG_ICM20948_TRIGGER_GLOBAL_THREAD)
	k_work_submit(&s->work);
#endif
}

static void icm20948_thread_cb(const struct device *dev)
{
	struct inv_icm20948 *s = dev->data;

	if (s->handler != NULL) {
		s->handler(dev, s->trigger);
	}

	icm20948_set_interrupts(dev, true);
}

#ifdef CONFIG_ICM20948_TRIGGER_OWN_THREAD
static void icm20948_thread(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	struct inv_icm20948 *s = p1;

	while (42) {
		k_sem_take(&s->gpio_sem, K_FOREVER);
		icm20948_thread_cb(s->dev);
	}
}
#endif

#ifdef CONFIG_ICM20948_TRIGGER_GLOBAL_THREAD
static void icm20948_work_cb(struct k_work *work)
{
	struct inv_icm20948 *s =
		CONTAINER_OF(work, struct inv_icm20948, work);
	icm20948_thread_cb(s->dev);
}
#endif

int icm20948_trigger_set(const struct device *dev,
			const struct sensor_trigger *trigger,
			sensor_trigger_handler_t handler)
{
	struct inv_icm20948 *s = dev->data;
	const struct icm20948_config *cfg = dev->config;

	if (!cfg->int_gpio.port) {
		return -ENOTSUP;
	}

    icm20948_set_interrupts(dev, false);

	switch (trigger->type) {
	case SENSOR_TRIG_FIFO_WATERMARK:
        s->trigger = trigger;
        s->handler = handler;
        s->int_enables[3] |= 0x1f; // Enable FIFO WM interrupt
        break;
    default:
        LOG_ERR("Trigger type not supported: %d", trigger->type);
        return -ENOTSUP;
	}

	icm20948_set_interrupts(dev, true);

	return 0;
}

int icm20948_init_interrupt(const struct device *dev)
{
	struct inv_icm20948 *s = dev->data;
	const struct icm20948_config *cfg = dev->config;

	if (!gpio_is_ready_dt(&cfg->int_gpio)) {
		LOG_ERR("%s: device %s is not ready", dev->name,
				cfg->int_gpio.port->name);
		return -ENODEV;
	}

	gpio_pin_configure_dt(&cfg->int_gpio, GPIO_INPUT | cfg->int_gpio.dt_flags);

	gpio_init_callback(&s->gpio_cb,
			   icm20948_gpio_callback,
			   BIT(cfg->int_gpio.pin));

	if (gpio_add_callback(cfg->int_gpio.port, &s->gpio_cb) < 0) {
		LOG_DBG("Failed to set gpio callback!");
		return -EIO;
	}

	s->dev = dev;

#if defined(CONFIG_ICM20948_TRIGGER_OWN_THREAD)
	k_sem_init(&s->gpio_sem, 0, K_SEM_MAX_LIMIT);

	k_thread_create(&s->thread, s->thread_stack,
			CONFIG_ICM20948_THREAD_STACK_SIZE,
			icm20948_thread, s,
			NULL, NULL, K_PRIO_COOP(CONFIG_ICM20948_THREAD_PRIORITY),
			0, K_NO_WAIT);
#elif defined(CONFIG_ICM20948_TRIGGER_GLOBAL_THREAD)
	s->work.handler = icm20948_work_cb;
#endif
	icm20948_set_interrupts(dev, true);

	return 0;
}