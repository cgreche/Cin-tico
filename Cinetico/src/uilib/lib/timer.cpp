
//Timer
// File: timer.cpp
// Last Edit: 12/01/2015 20:42 (UTC-3)
// Author: CGR

#include "timer.h"

namespace uilib {

	Timer::Timer(TimerSystem &handler)
		: m_handler(handler),
		m_enabled(false),
		m_periodic(false),
		m_duration(-1),
		m_timerFunc(NULL),
		m_param(NULL),
		m_id(0),
		m_startTime(0),
		m_expireTime(-1),

		m_prev(NULL),
		m_next(NULL)
	{
	}

	void Timer::trigger()
	{
		if (!m_periodic)
			m_enabled = false;

		//save information about the current startTime
		osd_ticks_t oldStart = m_startTime;

		if (m_timerFunc)
			(*m_timerFunc)(m_param, m_id);

		//if the startTime changed, it means, timer was modified during callback
		if (m_startTime == oldStart) {
			if (m_periodic) {
				//re-schedule
				m_startTime = m_expireTime;
				m_expireTime += m_duration;
			}
		}
	}

	void Timer::setEnabled(bool enabled)
	{
		if (m_enabled == enabled)
			return;

		m_enabled = enabled;
		m_handler.updateTimer(*this);
	}

	void Timer::start(osd_ticks_t duration, bool periodic)
	{
		m_enabled = true;
		m_duration = duration;
		m_periodic = periodic;

		osd_ticks_t time = m_handler.currentTime();
		m_startTime = time;
		m_expireTime = time + duration;
		m_handler.updateTimer(*this);
	}

	void Timer::reset()
	{
		start(m_duration);
	}

	void Timer::set(osd_ticks_t duration, bool periodic, bool enabled, TimerFunc callback, void *param, int id)
	{
		osd_ticks_t time = m_handler.currentTime();
		m_periodic = true;
		m_duration = duration;
		m_timerFunc = callback;
		m_param = param;
		m_id = id;

		setEnabled(enabled);
		m_startTime = time;
		m_expireTime = time + duration;
		m_handler.updateTimer(*this);
	}

	void Timer::setDuration(osd_ticks_t duration, bool periodic, bool update)
	{
		m_duration = duration;
		m_periodic = periodic;

		if (update) {
			m_expireTime = m_startTime + duration;
			m_handler.updateTimer(*this);
		}
	}

	osd_ticks_t Timer::elapsed() const
	{
		return m_handler.currentTime() - m_startTime;
	}

	osd_ticks_t Timer::remaining() const
	{
		osd_ticks_t current = m_handler.currentTime();
		if (current >= m_expireTime)
			return 0;
		return m_expireTime - current;
	}



	TimerSystem::TimerSystem()
		: m_firstTimer(NULL)
	{
		m_currentTime = osd_ticks();
	}

	void TimerSystem::insertTimerIntoList(Timer &timer)
	{
		Timer *t = m_firstTimer;

		if (!t) {
			m_firstTimer = &timer;
		}
		else {
			osd_ticks_t expire = timer.expireTime();
			bool enabled = timer.enabled();

			for (;;) {
				if (enabled && t->expireTime() > expire) {

					timer.m_prev = t->m_prev;
					timer.m_next = t;
					if (t->m_prev)
						t->m_prev->m_next = &timer;
					t->m_prev = &timer;

					if (m_firstTimer == t)
						m_firstTimer = &timer;
					return;
				}

				if (!t->m_next)
					break;

				t = t->m_next;
			}

			//we will be the last timer
			t->m_next = &timer;
			timer.m_prev = t;
			timer.m_next = NULL;
		}
	}

	void TimerSystem::removeTimerFromList(Timer &timer)
	{
		if (m_firstTimer == &timer) {
			m_firstTimer = timer.m_next;
		}
		else {
			if (timer.m_prev)
				timer.m_prev->m_next = timer.m_next;
		}

		if (timer.m_next)
			timer.m_next->m_prev = timer.m_prev;
	}

	inline void TimerSystem::updateTimer(Timer &timer)
	{
		removeTimerFromList(timer);
		insertTimerIntoList(timer);
	}

	Timer *TimerSystem::createTimer(TimerFunc callback, void *param, int id)
	{
		Timer *timer = new Timer(*this);
		timer->m_timerFunc = callback;
		timer->m_param = param;
		timer->m_id = id;
		insertTimerIntoList(*timer);
		return timer;
	}

	Timer *TimerSystem::createOneShotTimer(osd_ticks_t duration, TimerFunc callback, void *param, int id)
	{
		Timer *timer = new Timer(*this);
		timer->set(duration, false, true, callback, param, id);
		timer->reset();
		insertTimerIntoList(*timer);
		return timer;
	}

	Timer *TimerSystem::createPeriodicTimer(osd_ticks_t period, TimerFunc callback, void *param, int id)
	{
		Timer *timer = new Timer(*this);
		timer->set(period, true, true, callback, param, id);
		timer->reset();
		insertTimerIntoList(*timer);
		return timer;
	}

	void TimerSystem::execute()
	{
		if (!m_firstTimer)
			return;

		osd_ticks_t curTime = osd_ticks();
		m_currentTime = curTime;

		while (curTime > m_firstTimer->m_expireTime) {
			Timer &t = *m_firstTimer;

			if (t.enabled()) {
				t.trigger();
			}

			updateTimer(t);
		}
	}

	osd_ticks_t TimerSystem::currentTime() const
	{
		return m_currentTime;
	}

}