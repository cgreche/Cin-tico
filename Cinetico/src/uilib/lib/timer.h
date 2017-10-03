
//Timer
// File: timer.h
// Last Edit: 12/01/2015 20:42 (UTC-3)
// Author: CGR

#ifndef __TIMER_H__
#define __TIMER_H__

#include "..\\types.h"
#include "time.h"

namespace uilib {

	class Timer;
	class TimerSystem;

	typedef void(*TimerFunc)(void *param, int id);

	class Timer
	{
		friend class TimerSystem;

		TimerSystem &m_handler;

		bool m_enabled;
		bool m_periodic;
		osd_ticks_t m_startTime;
		osd_ticks_t m_expireTime;
		osd_ticks_t m_duration; //for periodic times

		TimerFunc m_timerFunc;
		void *m_param;
		int m_id;

		Timer *m_prev;
		Timer *m_next;

		Timer(TimerSystem &handler);
		~Timer();

		void trigger();

	public:

		void start(osd_ticks_t duration, bool periodic = true);
		void reset();
		void set(osd_ticks_t duration, bool periodic, bool enabled, TimerFunc callback, void *param, int id = 0);
		void setEnabled(bool enabled);
		void setDuration(osd_ticks_t duration, bool periodic, bool update = false);
		inline void setPeriodic(bool periodic) { m_periodic = periodic; }
		inline void setParam(void *param) { m_param = param; }

		bool enabled() const { return m_enabled; }
		bool periodic() const { return m_periodic; }
		osd_ticks_t startTime() const { return m_startTime; }
		osd_ticks_t expireTime() const { return m_expireTime; }
		osd_ticks_t duration() const { return m_duration; }

		osd_ticks_t elapsed() const;
		osd_ticks_t remaining() const;

		static inline osd_ticks_t seconds(int s);
		static inline osd_ticks_t miliseconds(int ms);
	};

	inline osd_ticks_t Timer::seconds(int s) { return s*osd_tps(); }
	inline osd_ticks_t Timer::miliseconds(int ms) { return ms*osd_tps() / 1000; }

	class TimerSystem
	{
		friend class Timer;

		osd_ticks_t m_currentTime;

		Timer *m_firstTimer;

		void insertTimerIntoList(Timer &timer);
		void removeTimerFromList(Timer &timer);
		inline void updateTimer(Timer &timer);

	public:
		TimerSystem();

		Timer *createTimer(TimerFunc callback, void *param, int id = 0);
		Timer *createTimer(int id = 0) { return createTimer(NULL, NULL, id); }
		Timer *createOneShotTimer(osd_ticks_t duration, TimerFunc callback, void *param, int id = 0);
		Timer *createPeriodicTimer(osd_ticks_t period, TimerFunc callback, void *param, int id = 0);

		void execute();

		osd_ticks_t currentTime() const;
	};

}

#endif