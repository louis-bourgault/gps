#include <stdio.h>
#include "esp_spiffs.h"
#include "gps.h"

void app_main(void)
{
    printf("Starting...\n");
    gps_init();

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = "storage",
        .max_files = 5,
        .format_if_mount_failed = true,
    };
    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&conf));

    FILE *f = fopen("/spiffs/hello.txt", "w");
    if (f) {
        fprintf(f, "greetings, is this working\n");
        fclose(f);
    }

    f = fopen("/spiffs/hello.txt", "r");
    if (f) {
        char buf[64];
        if (fgets(buf, sizeof(buf), f)) {
            printf("read from file: %s", buf);
        }
        fclose(f);
    }

    esp_vfs_spiffs_unregister("storage");
}
