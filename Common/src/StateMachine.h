#ifndef BELOTE_STATEMACHINE_H
#define BELOTE_STATEMACHINE_H

#include <queue>
#include <map>

// Forward decl
struct State;
typedef std::shared_ptr<State>	StatePtr;

class StateMachine;
typedef std::shared_ptr<StateMachine>	StateMachinePtr;

typedef unsigned int EventCode;

//! Action performed while transitionning from state s1 to state s2.
struct Action
{
	virtual ~Action()		{ ; }

	virtual void operator()() = 0;

private:
	Action& operator=(const Action &)	{ ; }
};
typedef std::shared_ptr<Action>	ActionPtr;

//! Transition defines how the SM goes from state s1 to state s2.
struct Transition
{
	Transition()				{ ; }

	Transition(StatePtr src, StatePtr dst, ActionPtr a) : m_Source(src), m_Target(dst), m_Action(a)	{ ; }

	void	Execute();

	StatePtr	m_Source;
	StatePtr	m_Target;
	ActionPtr	m_Action;
};
typedef std::shared_ptr<Transition>	TransitionPtr;

//! The state class is responsible of the main job: eventual enter/leave reaction, as well as eventually performing some stuff while the state is active.
struct State
{
public:
	State(StateMachinePtr sm) : m_StateMachine(sm)	{ ; }
	virtual ~State()								{ ; }

	// Setup
	void		AddTransition(EventCode evt, StatePtr target, ActionPtr a = ActionPtr())
	{
		m_EventMapping[evt] = TransitionPtr(new Transition(StatePtr(this), target, a));
	}

	// Infos
	bool		CanHandleEvent(EventCode evt) const				{ return m_EventMapping.find(evt) != m_EventMapping.end(); }
	Transition&	GetTransition(EventCode evt)					{ return *m_EventMapping.find(evt)->second; }

	// Runtime
	virtual void	Enter()						{ ; }
	virtual void	Update()					{ ; }
	virtual void	Leave()						{ ; }

protected:
	StateMachinePtr		m_StateMachine;

private:
	State& operator=(const State &)	{ ; }

private:
	typedef std::map<EventCode, TransitionPtr>					EventMap;
	typedef std::map<EventCode, TransitionPtr>::iterator		EventMapIter;
	typedef std::map<EventCode, TransitionPtr>::const_iterator	EventMapConstIter;

	EventMap			m_EventMapping;
};

class StateMachine
{
public:
	StateMachine()				{ ; }
	virtual ~StateMachine()		{ ; }

	void	Start(StatePtr startState)
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
	StatePtr				m_CurrentState;
	std::queue<EventCode>	m_EventQueue;
	bool					m_StopFlag;
};

#endif
