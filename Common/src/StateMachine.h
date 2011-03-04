#ifndef BELOTE_STATEMACHINE_H
#define BELOTE_STATEMACHINE_H

#include <queue>
#include <map>

// Saying it is a state machine is somewhat pompous. It's an overly
// simplified one, just to handle network protocols.

typedef unsigned int EventCode;

struct Action
{
	virtual void operator()() = 0;
};

struct State;
struct Transition
{
	Transition() : m_Source(0), m_Target(0), m_Action(0)	{ ; }
	Transition(State *src, State *dst, Action *a) : m_Source(src), m_Target(dst), m_Action(a)	{ ; }

	void	Execute();

	State *		m_Source;
	State *		m_Target;
	Action *	m_Action;
};

class StateMachine;
struct State
{
public:
	State(StateMachine *sm) : m_StateMachine(sm)	{ ; }

	// Setup
	void		AddTransition(EventCode evt, State *target)				{ AddTransition(evt, target, 0); }
	void		AddTransition(EventCode evt, State *target, Action *a)	{ m_EventMapping[evt] = new Transition(this, target, a); }

	// Infos
	bool		CanHandleEvent(EventCode evt) const				{ return m_EventMapping.find(evt) != m_EventMapping.end(); }
	Transition&	GetTransition(EventCode evt)					{ return *m_EventMapping.find(evt)->second; }

	// Runtime
	virtual void	Enter()						{ ; }
	virtual void	Update()					{ ; }
	virtual void	Leave()						{ ; }

protected:
	StateMachine *		m_StateMachine;

private:
	typedef std::map<EventCode, Transition*>					EventMap;
	typedef std::map<EventCode, Transition*>::iterator			EventMapIter;
	typedef std::map<EventCode, Transition*>::const_iterator	EventMapConstIter;

	EventMap			m_EventMapping;
};

class StateMachine
{
public:
	StateMachine() : m_CurrentState(0)	{ ; }

	void	Start(State *startState)
	{
		m_StopFlag		= false;
		m_CurrentState	= startState;
		m_CurrentState->Enter();
	}

	void	Stop()						{ m_StopFlag = true; }

	void	Notify(EventCode e)			{ m_EventQueue.push(e); }

	void	Update();
	bool	IsStopped() const			{ return m_StopFlag; }

private:
	State *					m_CurrentState;
	std::queue<EventCode>	m_EventQueue;
	bool					m_StopFlag;
};

#endif
