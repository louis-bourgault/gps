idf_component_register(SRCS "main.c" "gps.c"
                       PRIV_REQUIRES spiffs esp_driver_uart esp_driver_gpio
                       INCLUDE_DIRS ".")
