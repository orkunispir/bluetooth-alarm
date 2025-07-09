#include "esp_err.h"

/**
 * @brief Obtain the current time from an SNTP server.
 * 
 */
esp_err_t obtain_time(void);

/**
 * @brief Calculate time until next alarm.
 * 
 * @param hour 
 * @param minute 
 * @param tolerance
 * 
 * 
 * @return int 
 */
int calculate_until_alarm(int hour, int minute, int tolerance);