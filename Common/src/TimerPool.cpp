#include <algorithm>
#include <cassert>

#include "TimerPool.h"
#include "Timer.h"

TimerPool* Singleton<TimerPool>::ms_Singleton = 0;

void TimerPool::Init()
{
	new TimerPool();
}

TimerPool::TimerPool()
{
	m_TimerDeleteQueue.reserve(16);
}

void TimerPool::Register(Timer *timer)
{
	m_RegisteredTimers.insert(std::make_pair(timer, sf::Clock()));
}

void TimerPool::Unregister(Timer *timer)
{
	m_RegisteredTimers.erase(timer);
}

void TimerPool::_Start(Timer *timer)
{
	TimerPtrContainer::iterator it = m_RegisteredTimers.find(timer);
	assert(it !=  m_RegisteredTimers.end());

	it->second.Reset();
}

void TimerPool::TimerUpdate::operator()(TimerPtrContainer::value_type &timerEntry)
{
	Timer *t = timerEntry.first;
	if (!t->IsStarted())
		return;

	sf::Clock &clock = timerEntry.second;
	const sf::Uint32 elapsedMs = static_cast<sf::Uint32>(clock.GetElapsedTime() * 1000);

	if (elapsedMs < t->GetPeriod())
		return;

	t->GetTimeoutFunc()();

	if (t->GetMode() != Timer::TM_ONESHOT)
		return;

	t->Stop();

	if (t->AutoDelete())
		m_Self->m_TimerDeleteQueue.push_back(t);
}

void TimerPool::Update()
{
	std::for_each(m_RegisteredTimers.begin(), m_RegisteredTimers.end(), TimerUpdate(this));

	std::for_each(m_TimerDeleteQueue.begin(), m_TimerDeleteQueue.end(), [] (Timer *t) { delete t; } );
	m_TimerDeleteQueue.swap(TimerToDelete());
}
