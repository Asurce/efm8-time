#include "efm8_time.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

// SECONDS_SINCE_EPOCH variables
int sse_days;
int sse_year;
int sse_month;

// SECONDS_TO_TIME variables
int stt_days;
time_t stt_tm;

// SECONDS_TO_STRING variables
time_t sects_tm;
int sects_bytes_written;
static char sects_buffer[20]; // Static buffer to avoid memory allocation

// STRING_TO_SECONDS variables
time_t stsec_tm;


/**
 * @brief Days in a non-leap year for each month
 */
static const uint8_t days_in_month[MONTHS_PER_YEAR] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

uint8_t is_leap_year(int year)
{
    /* Checking the century years */
    if (year % 400 == 0)
    {
        return 1;
    }
    else if (year % 100 == 0)
    {
        return 0;
    }
    /* Leap years are fully divisible by 4*/
    else if (year % 4 == 0)
    {
        return 1;
    }
    return 0;
}

uint16_t days_in_year(int year)
{
    return is_leap_year(year) ? LEAP_YEAR_DAYS : NON_LEAP_YEAR_DAYS;
}

uint32_t seconds_since_epoch(time_t *tm)
{
    /* Check  invalid values */
    if (tm->tm_mon > 11 ||
        tm->tm_mday < 1 || tm->tm_mday > 31 ||
        tm->tm_hour > 23 ||
        tm->tm_min > 59 ||
        tm->tm_sec > 60)
    {
        return 0;
    }

    // Calculate number of days since 1970-01-01
    sse_days = 0;

    // Adjust year calculation to start from 1970
    for (sse_year = 1970; sse_year < tm->tm_year; sse_year++)
    {
        sse_days += days_in_year(sse_year);
    }

    for (sse_month = 0; sse_month < tm->tm_mon; sse_month++)
    {
        sse_days += days_in_month[sse_month];
    }

    if (is_leap_year(tm->tm_year) && tm->tm_mon > 1)
    {
        sse_days += 1;
    }

    sse_days += tm->tm_mday - 1;

    // Convert to seconds since epoch
    return sse_days * HOURS_PER_DAY * SECONDS_PER_HOUR + tm->tm_hour * SECONDS_PER_HOUR + tm->tm_min * SECONDS_PER_MINUTE + tm->tm_sec;
}

time_t seconds_to_time(uint32_t seconds)
{
    // Extract seconds, minutes, and hours
    stt_tm.tm_sec = seconds % 60;
    seconds /= 60;
    stt_tm.tm_min = seconds % 60;
    seconds /= 60;
    stt_tm.tm_hour = seconds % 24;

    // Calculate days since epoch
    stt_days = seconds / 24;

    // Set year to 1970 (adjust based on your epoch if different)
    stt_tm.tm_year = 1970;

    // Iterate through years to find the current year
    while (stt_days >= days_in_year(stt_tm.tm_year))
    {
        stt_days -= days_in_year(stt_tm.tm_year);
        stt_tm.tm_year++;
    }

    // Calculate month and day within the current year
    stt_tm.tm_mon = 0;
    while (stt_days >= days_in_month[stt_tm.tm_mon])
    {
        stt_days -= days_in_month[stt_tm.tm_mon];
        stt_tm.tm_mon++;
    }

    // Account for leap day in February
    if (is_leap_year(stt_tm.tm_year) && stt_tm.tm_mon > 1 && stt_days > 0)
    {
        stt_days--;
    }

    stt_tm.tm_mday = stt_days + 1;

    return stt_tm;
}

char *seconds_to_string(uint32_t seconds)
{

    // Use seconds_to_tm function to get the time components
    sects_tm = seconds_to_time(seconds);

    // Format the string using snprintf
    sects_bytes_written =  sprintf(
                                sects_buffer,
                                "%04d:%02d:%02d:%02d:%02d:%02d",
                                sects_tm.tm_year,
                                sects_tm.tm_mon + 1,
                                sects_tm.tm_mday,
                                sects_tm.tm_hour,
                                sects_tm.tm_min,
                                sects_tm.tm_sec
                                );

    // Check for errors or buffer overflow
    if (sects_bytes_written < 0 || sects_bytes_written >= sizeof(sects_buffer))
    {
        return (char*) 0;
    }

    return sects_buffer;
}

time_t string_to_time(char *date_string)
{

  // Read the string using sscanf
  sscanf(
      date_string,
      "%d:%d:%d:%d:%d:%d",
      &stsec_tm.tm_year,
      &stsec_tm.tm_mon,
      &stsec_tm.tm_mday,
      &stsec_tm.tm_hour,
      &stsec_tm.tm_min,
      &stsec_tm.tm_sec
      );

    stsec_tm.tm_mon--;

  return stsec_tm;

}
