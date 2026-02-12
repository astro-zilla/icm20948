#pragma once

void icm20948_trigger_handler(const struct device *dev, const struct sensor_trigger *trigger);
int icm20948_trigger_set(const struct device *dev, const struct sensor_trigger *trig, sensor_trigger_handler_t handler);