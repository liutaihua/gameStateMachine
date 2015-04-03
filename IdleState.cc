#include "IdleState.h"
#include "StateMachine.h"
#include "Utility.h"

IdleState::IdleState(StateMachine* pStateMachine) :
IState(STATE_IDLE, pStateMachine)
{

}

IdleState::~IdleState()
{

}

void IdleState::OnEnter(int stateId) {
    //std::cout<<"enter idle state from state:"<<std::to_string(stateId)<<"\n";
}

void IdleState::OnLeave(int stateId){
    //std::cout<<"leave idle state to state:"<<std::to_string(stateId)<<"\n";
}

void IdleState::MoveTo(Position& pos)
{
    //DBG("moving...........................................................");
    m_pCreature->doMoveTo(pos);
    m_pStateMachine->ChangeStateTo(STATE_MOVE);
}


