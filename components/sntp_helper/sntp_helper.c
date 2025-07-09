
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

#include "esp_err.h"
#include "esp_log.h"
#include "esp_netif_sntp.h"
#include "esp_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "sntp_helper.h"

static char *TAG = "sntp_helper";

esp_err_t obtain_time(void) {

  esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
  esp_err_t ret = esp_netif_sntp_init(&config);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize SNTP: %s", esp_err_to_name(ret));
    return ret;
  }

  if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) != ESP_OK) {
    printf("Failed to update system time within 10s timeout");
    return ESP_ERR_TIMEOUT;
  }

  // wait for time to be set
  time_t now = 0;
  struct tm timeinfo = {0};
  int retry = 0;
  const int retry_count = 15;
  while (esp_netif_sntp_sync_wait(5000 / portTICK_PERIOD_MS) ==
             ESP_ERR_TIMEOUT &&
         ++retry < retry_count) {
    ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry,
             retry_count);
  }
  time(&now);
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
  tzset();
  localtime_r(&now, &timeinfo);

  ESP_LOGI(TAG, "System time set to: %s", asctime(&timeinfo));

  return ESP_OK;
}

int calculate_until_alarm(int hour, int minute, int tolerance) {
  time_t now;
  struct tm timeinfo = {0};
  time(&now);
  localtime_r(&now, &timeinfo);

  // Check if the current time matches the specified hour and minute with a tolerance
    if (timeinfo.tm_hour == hour && timeinfo.tm_min >= minute - tolerance &&
        timeinfo.tm_min <= minute + tolerance) {
        ESP_LOGI(TAG, "Current time matches the specified hour and minute.");
        return 0;
    } else {
        ESP_LOGI(TAG, "Current time does not match the specified hour and minute.");
        // Calculate the next wakeup time
        // target time
        time_t target_time = now;
        struct tm target_tm = {0};
        localtime_r(&target_time, &target_tm);

        target_tm.tm_hour = hour;
        target_tm.tm_min = minute;
        target_tm.tm_sec = 0;

        target_time = mktime(&target_tm);

        // If the target time is in the past, add one day
        if (difftime(target_time,now) < 0) {
            target_tm.tm_mday += 1;
            target_time = mktime(&target_tm);
        }

        

        // calculate difference between tiems

        return difftime(target_time, now);

    }
}