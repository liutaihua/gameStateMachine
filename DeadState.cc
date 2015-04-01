#include "DeadState.h"
#include "StateMachine.h"



DeadState::DeadState(StateMachine* pStateMachine) :
IState(STATE_DEAD, pStateMachine)
{

}

DeadState::~DeadState()
{

}

void DeadState::OnEnter(int /*lastStateId*/)
{
    m_pCreature->doDead(KillerId);
}

void DeadState::Dead()
{
    m_pStateMachine->ChangeStateTo(STATE_DEAD);
}

void DeadState::Reborn()
{
    m_pCreature->doReborn();
    m_pStateMachine->ChangeStateTo(STATE_IDLE);
}
