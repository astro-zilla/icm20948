zephyr_library()

zephyr_library_sources(main.c system.c sensor.c)

add_subdirectory_ifdef(CONFIG_ICM20948 Invn)