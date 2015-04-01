#include "IState.h"
#include "StateMachine.h"

IState::IState( int type, StateMachine* pStateMachine ) :
m_pStateMachine(pStateMachine),
m_type(type)
{
  m_pCreature = pStateMachine->m_pOwner;
}

