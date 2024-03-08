/**
 * @file EFM8_TIME.h
 * @brief Header file for EFM8 time related functions and structures.
 *
 * This header file defines a custom `time_t` structure and various functions
 * for managing time on the EFM8 platform.
 */
#ifndef EFM8_TIME
#define EFM8_TIME

#include <stdint.h>

/**
 * @def MONTHS_PER_YEAR
 * @brief Number of months in a year.
 */
#define MONTHS_PER_YEAR 12

/**
 * @def LEAP_YEAR_DAYS
 * @brief Number of days in a leap year.
 */
#define LEAP_YEAR_DAYS 366

/**
 * @def NON_LEAP_YEAR_DAYS
 * @brief Number of days in a non-leap year.
 */
#define NON_LEAP_YEAR_DAYS 365

/**
 * @def HOURS_PER_DAY
 * @brief Number of hours in a day.
 */
#define HOURS_PER_DAY 24

/**
 * @def MINUTES_PER_HOUR
 * @brief Number of minutes in an hour.
 */
#define MINUTES_PER_HOUR 60

/**
 * @def SECONDS_PER_MINUTE
 * @brief Number of seconds in a minute.
 */
#define SECONDS_PER_MINUTE 60

/**
 * @def SECONDS_PER_HOUR
 * @brief Number of seconds in an hour.
 */
#define SECONDS_PER_HOUR 3600

/**
 * @typedef time_t
 * @brief Custom structure to represent time components similar to `tm`.
 *
 * This structure holds various time components:
 * - `tm_sec`: Seconds (0-59)
 * - `tm_min`: Minutes (0-59)
 * - `tm_hour`: Hour (0-23)
 * - `tm_mday`: Day of the month (1-31)
 * - `tm_mon`: Month (0-11)
 * - `tm_year`: Year since 1900 (adjusted for minimum year)
 */
typedef struct
{
    int tm_sec;  // Seconds (0-59)
    int tm_min;  // Minutes (0-59)
    int tm_hour; // Hour (0-23)
    int tm_mday; // Day of the month (1-31)
    int tm_mon;  // Month (0-11)
    int tm_year; // Year since 1900 (adjusted for minimum year)
} time_t;

/**
 * @brief Checks if a given year is a leap year.
 *
 * @param year The year to be checked.
 *
 * @return 1 if the year is a leap year, 0 otherwise.
 */
uint8_t is_leap_year(int year);

/**
 * @brief Calculates the number of days in a year.
 *
 * @param year The year for which to calculate the number of days.
 *
 * @return The number of days in the year.
 */
uint16_t days_in_year(int year);

/**
 * @brief Calculates the number of seconds since the epoch for a given time.
 *
 * @param tm A pointer to a `time_t` structure containing the time components.
 *
 * @return The number of seconds since the epoch for the given time - OK, -1 - ERROR.
 */
uint32_t seconds_since_epoch(time_t *tm);

/**
 * @brief Converts a number of seconds since the epoch to a `time_t` structure.
 *
 * @param seconds The number of seconds since the epoch.
 *
 * @return A `time_t` structure populated with the corresponding time components.
 */
time_t seconds_to_time(uint32_t seconds);

/**
 * @brief Converts a number of seconds since the epoch to a formatted string.
 *
 * @param seconds The number of seconds since the epoch.
 *
 * @return A string representation of the time in the format "YYYY:MM:DD:HH:MM:SS" or NULL on error.
 */
char *seconds_to_string(uint32_t seconds);


/**
 * @brief Converts a formatted string to a number of seconds since the epoch.
 *
 * @param time_string The string representation of the time in the format "YYYY:MM:DD:HH:MM:SS"
 *
 * @return The number of seconds since the epoch.
 */
time_t string_to_time(char *date_string);


#endif
