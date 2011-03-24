#ifndef BELOTE_TIMERPOOL_H
#define BELOTE_TIMERPOOL_H

#include <map>
#include <vector>

#include <SFML/System/Clock.hpp>

#include "Singleton.h"

class Timer;

class TimerPool : public Singleton<TimerPool>
{
public:
	static void		Init();

	void	Register(Timer *timer);
	void	Unregister(Timer *timer);

	// Important: the call frequency of this method defines the timers' precision!
	void	Update();

	// Internal use only!
	void	_Start(Timer *timer);

private:
	typedef std::map<Timer*, sf::Clock>	TimerPtrContainer;
	typedef std::vector<Timer*>			TimerToDelete;

private:
	TimerPool();

	struct TimerUpdate
		: public std::unary_function<TimerPtrContainer::value_type, void>
	{
		TimerPool *m_Self;
		TimerUpdate(TimerPool *self) : m_Self (self)	{;}

		void operator()(TimerPtrContainer::value_type &timerEntry);
	};

private:
	TimerPtrContainer	m_RegisteredTimers;
	TimerToDelete		m_TimerDeleteQueue;
};

#endif
