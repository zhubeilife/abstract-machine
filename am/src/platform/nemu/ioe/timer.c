#include <am.h>
#include <nemu.h>

void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  // first read high and low 32-bit of the RTC
  // because the rtc_io_handler has if (!is_write && offset == 4) logic
  // detial can be found https://www.cnblogs.com/nosae/p/17066439.html#%E7%9C%8B%E7%9C%8Bnemu%E8%B7%91%E5%A4%9A%E5%BF%AB
  uint32_t high = inl(RTC_ADDR+4);
  uint32_t low = inl(RTC_ADDR);
  uptime->us = (uint64_t)low + (((uint64_t)high) << 32);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
