#include "gps.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define GPS_UART_NUM   UART_NUM_2
#define GPS_TX_PIN     GPIO_NUM_17
#define GPS_RX_PIN     GPIO_NUM_16
#define BUF_SIZE       256

static void gps_read_task(void *arg)
{
    uint8_t data[BUF_SIZE];
    while (1) {
        int len = uart_read_bytes(GPS_UART_NUM, data, BUF_SIZE - 1, pdMS_TO_TICKS(100));
        if (len > 0) {
            data[len] = 0;
            printf("%s", (char *)data);
        }
    }
}

void gps_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    ESP_ERROR_CHECK(uart_param_config(GPS_UART_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(GPS_UART_NUM, GPS_TX_PIN, GPS_RX_PIN, -1, -1));
    ESP_ERROR_CHECK(uart_driver_install(GPS_UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0));

    xTaskCreate(gps_read_task, "gps_read", 4096, NULL, 10, NULL);
}
