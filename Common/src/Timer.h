#ifndef BELOTE_TIMER_H
#define BELOTE_TIMER_H

#include <boost/function.hpp>

#include <SFML/Config.hpp>

#include "TimerPool.h"

class Timer
{
public:
	enum TimerMode
	{
		TM_ONESHOT,
		TM_REPEAT,
	};

	typedef boost::function<void()>		TimeoutFunc;

public:
	Timer() : m_AutoDelete(false), m_IsStarted(false), m_Mode(TM_REPEAT), m_TimeoutPeriod(0)
	{
		TimerPool::getSingleton().Register(this);
	}
	~Timer()
	{
		Stop();
		TimerPool::getSingleton().Unregister(this);
	}

	TimerMode		GetMode() const			{ return m_Mode; }
	sf::Uint32		GetPeriod() const		{ return m_TimeoutPeriod; }
	TimeoutFunc&	GetTimeoutFunc()		{ return m_TimeoutFunc; }
	bool			IsStarted() const		{ return m_IsStarted; }
	bool			AutoDelete() const		{ return m_AutoDelete; }

	void	SetMode(TimerMode mode)				{ m_Mode = mode; }
	void	SetPeriod(sf::Uint32 period)		{ m_TimeoutPeriod = period; }
	void	SetTimeoutFunc(TimeoutFunc func)	{ m_TimeoutFunc = func; }

	void	Start(TimerMode mode, sf::Uint32 period, TimeoutFunc func)
	{
		SetMode(mode);
		SetPeriod(period);
		SetTimeoutFunc(func);
		Start();
	}

	void	Start()		{ m_IsStarted = true; TimerPool::getSingleton()._Start(this); }
	void	Stop()		{ m_IsStarted = false; }

	static	void	SingleShot(sf::Uint32 period, TimeoutFunc func)
	{
		Timer *t = new Timer();
		t->m_AutoDelete = true;
		t->Start(TM_ONESHOT, period, func);
	}

private:
	bool			m_AutoDelete;
	bool			m_IsStarted;
	TimerMode		m_Mode;
	sf::Uint32		m_TimeoutPeriod;
	TimeoutFunc		m_TimeoutFunc;
};

#endif
