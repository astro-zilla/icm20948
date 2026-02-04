zephyr_library()

zephyr_library_sources(icm20948.c ak09916.c icm20948_dmp.c)

add_subdirectory_ifdef(CONFIG_ICM20948 Invn)