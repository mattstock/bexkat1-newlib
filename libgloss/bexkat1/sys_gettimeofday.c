#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include "spi.h"
#include "rtc.h"
#include <stdio.h>

// time2fat
int time2fat(time_t now) {
  struct tm ts;
  int timeval;

  ts = *localtime(&now);
  timeval = (ts.tm_year - 80) << 25;
  timeval |= (ts.tm_mon + 1) << 21;
  timeval |= ts.tm_mday << 16;
  timeval |= ts.tm_hour << 11;
  timeval |= ts.tm_min << 5;
  timeval |= ts.tm_sec >> 1;
  return timeval;
}

// fat2time
time_t fat2time(int fatdate, int fattime) {
  struct tm timeval;

  timeval.tm_sec = (fattime & 0x1f) << 2;
  fattime >>= 5;
  timeval.tm_min = (fattime & 0x1f);
  fattime >>= 5;
  timeval.tm_hour = (fattime & 0x1f);
  fattime >>= 5;
  timeval.tm_mday = (fatdate & 0x1f);
  fatdate >>= 5;
  timeval.tm_mon = (fatdate & 0xf) - 1;
  fatdate >>= 4;
  timeval.tm_year = (fatdate & 0x4f) + 80;

  return mktime(&timeval);
}

/*
 * time -- return current time in seconds.
 */
time_t
_time (time_t *t)
{
  struct tm tm;
  time_t ret;
  int val;

  tm.tm_sec = BCD2DEC(rtc_cmd(RTC_SECONDS,0x00));
  tm.tm_min = BCD2DEC(rtc_cmd(RTC_MINUTES,0x00));
  val = rtc_cmd(RTC_HOURS,0x00);
  tm.tm_hour = BCD2DEC(val & 0x1f);
  if (val & 0x40) {
    if (val & 0x20)
      tm.tm_hour += 12;
  } else {
    if (val & 0x20)
      tm.tm_hour += 20;
  }

  tm.tm_wday = BCD2DEC(rtc_cmd(RTC_DAY_OF_WEEK,0x00)) + 1;
  tm.tm_mday = BCD2DEC(rtc_cmd(RTC_DATE_OF_MONTH, 0x00));
  val = rtc_cmd(RTC_MONTH, 0x00);
  tm.tm_mon = BCD2DEC(val & 0x1f) - 1;
  tm.tm_year = BCD2DEC(rtc_cmd(RTC_YEAR, 0x00));
  if (val & 0x80)
    tm.tm_year += 100;

  tm.tm_isdst = -1;

  ret = mktime(&tm);

  if (t)
    *t = ret;

  return ret;
}

/*
 * _gettimeofday -- implement in terms of time, which means we can't
 * return the microseconds.
 */
int
_gettimeofday (struct timeval *tv, void *tzvp)
{
  struct timezone *tz = tzvp;
  struct tm tm;
  if (tz)
    tz->tz_minuteswest = tz->tz_dsttime = 0;

  tv->tv_usec = 0;
  tv->tv_sec = _time(0);

  return 0;
}
