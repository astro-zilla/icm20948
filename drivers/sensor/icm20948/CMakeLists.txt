zephyr_library()

zephyr_library_sources(main.c sensor.c)
zephyr_library_sources_ifdef(CONFIG_ICM20948_TRIGGER trigger.c)

add_subdirectory_ifdef(CONFIG_ICM20948 Invn)