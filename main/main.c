#include <stdio.h>
#include <sys/stat.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_helper.h"
#include "wifi_helper.h"
#include "sntp_helper.h"
#include "esp_sleep.h"


#include "esp_wifi.h"

#define WAKEUP_TIME_H 5
#define WAKEUP_TIME_M 30
#define WAKEUP_TOLERANCE_M 15

static const char *TAG = "main";


void app_main(void)
{

    // initialize NVS
    esp_err_t ret = nvs_init();
    if (ret != ESP_OK) {
        printf("NVS initialization failed: %s\n", esp_err_to_name(ret));
        return;
    }

    // initialize Wi-Fi
    ret = wifi_init();
    if (ret != ESP_OK) {
        printf("Wi-Fi initialization failed: %s\n", esp_err_to_name(ret));
        return;
    }

    ret = obtain_time();
    if (ret != ESP_OK) {
        printf("Failed to obtain time: %s\n", esp_err_to_name(ret));
        return;
    }

    int time_until_alarm = calculate_until_alarm(WAKEUP_TIME_H, WAKEUP_TIME_M, WAKEUP_TOLERANCE_M);
    
    if(time_until_alarm == 0) {
        ESP_LOGI(TAG, "Current time matches the specified wakeup time.");
    } else {
        ESP_LOGI(TAG, "Current time does not match the specified wakeup time. next wakeup time: %d", time_until_alarm);
        esp_deep_sleep(time_until_alarm*10000000);
    }

    while (1) {
        vTaskDelay(100000);
    }
}
