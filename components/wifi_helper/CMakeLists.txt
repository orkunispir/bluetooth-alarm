idf_component_register(SRCS "wifi_helper.c"
                       INCLUDE_DIRS "include"
                       REQUIRES esp_wifi esp_event protocol_examples_common)