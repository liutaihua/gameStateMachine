#include "MoveState.h"
#include "StateMachine.h"

MoveState::MoveState(StateMachine* pStateMachine) :
IState(STATE_MOVE, pStateMachine)
{

}

MoveState::~MoveState()
{

}

void MoveState::Update(uint64)
{
    moveFrameCount += 1;
    if (moveFrameCount % 100 == 0){
        std::cout<<"Move tired "<<std::to_string(moveFrameCount)<<"\n";
        StopAt(m_pCreature->GetPosition());
    }
}

void MoveState::MoveTo(Position& pos)
{
    //DBG("moving...........................................................");
    m_pCreature->doMoveTo(pos);
    m_pStateMachine->ChangeStateTo(STATE_MOVE);
}

void MoveState::StopAt(Position& pos)
{
    //m_pCreature->doMoveTo(position);
    //m_pStateMachine->ChangeStateTo(STATE_MOVE);
    m_pCreature->doStopAt(pos);
    m_pStateMachine->ChangeStateTo(STATE_IDLE);
}

void MoveState::Cast()
{
    m_pStateMachine->ChangeStateTo(STATE_CASTING);
}

void MoveState::Dead()
{
    m_pStateMachine->ChangeStateTo(STATE_DEAD);
}

