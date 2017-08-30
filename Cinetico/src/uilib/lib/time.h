
//Time
// File: time.h
// Last Edit: 11/01/2015 14:28 (UTC-3)
// Author: CGR

#ifndef __TIME_H__
#define __TIME_H__

typedef u64 osd_ticks_t;

osd_ticks_t osd_ticks();
osd_ticks_t osd_tps();
void osd_sleep(osd_ticks_t ticksToSleep);

#endif