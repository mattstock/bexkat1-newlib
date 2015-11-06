#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include "spi.h"
#include "rtc.h"
#include <stdio.h>

int
_DEFUN (_times, _times (buf),
	struct tms *buf)
{
  errno = EINVAL;
  return (-1);
}

/*
 * time -- return current time in seconds.
 */
time_t
_DEFUN (_time, _time (t),
	time_t *t)
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
    if (val & 0x10)
      tm.tm_hour += 10;
    if (val & 0x20)
      tm.tm_hour += 20;
  }

  tm.tm_wday = BCD2DEC(rtc_cmd(RTC_DAY_OF_WEEK,0x00)) - 1;
  tm.tm_mday = BCD2DEC(rtc_cmd(RTC_DATE_OF_MONTH, 0x00));
  val = rtc_cmd(RTC_MONTH, 0x00);
  tm.tm_mon = BCD2DEC(val & 0x1f) - 1;
  tm.tm_year = BCD2DEC(rtc_cmd(RTC_YEAR, 0x00)) + 100*((val&0x80) >> 7);

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
_DEFUN (_gettimeofday, _gettimeofday (tv, tz),
	struct timeval *tv _AND
	void *tzvp)
{
  struct timezone *tz = tzvp;
  struct tm tm;
  if (tz)
    tz->tz_minuteswest = tz->tz_dsttime = 0;

  tv->tv_usec = 0;
  tv->tv_sec = _time(0);

  return 0;
}
