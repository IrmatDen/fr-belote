#include "StateMachine.h"

void Transition::Execute()
{
	m_Source->Leave();

	if (m_Action)
	{
		(*m_Action)();
	}

	m_Target->Enter();
}

void StateMachine::Update()
{
	while (!m_EventQueue.empty())
	{
		EventCode e = m_EventQueue.front();
		m_EventQueue.pop();

		if (!m_CurrentState->CanHandleEvent(e))
			continue;

		Transition& t = m_CurrentState->GetTransition(e);
		t.Execute();

		m_CurrentState = t.m_Target;
	}
}