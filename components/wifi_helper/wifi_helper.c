#include "esp_event.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_wifi.h"

#include "protocol_examples_common.h"
#include "wifi_helper.h"

#define EXAMPLE_ESP_WIFI_SSID CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_MAXIMUM_RETRY CONFIG_ESP_MAXIMUM_RETRY

static const char *TAG = "wifi station";

EventGroupHandle_t s_wifi_event_group;

esp_err_t wifi_init(void) {
  // Initialize Wi-Fi here
  // This is a placeholder function, actual implementation will depend on your
  // Wi-Fi setup

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  esp_err_t ret = example_connect();

  wifi_ap_record_t ap_info;
  ESP_ERROR_CHECK(esp_wifi_sta_get_ap_info(&ap_info));
  ESP_LOGI(TAG, "--- Access Point Information ---");
  ESP_LOG_BUFFER_HEX("MAC Address", ap_info.bssid, sizeof(ap_info.bssid));
  ESP_LOG_BUFFER_CHAR("SSID", ap_info.ssid, sizeof(ap_info.ssid));
  ESP_LOGI(TAG, "Primary Channel: %d", ap_info.primary);
  ESP_LOGI(TAG, "RSSI: %d", ap_info.rssi);

  return ret;
}