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
        //StopAt(0, 0);
    }
}

void MoveState::MoveTo(int x, int y)
{
    //DBG("moving...........................................................");
    m_pCreature->doMoveTo(x, y);
    m_pStateMachine->ChangeStateTo(STATE_MOVE);
}

void MoveState::StopAt( int x, int y )
{
    //m_pCreature->doMoveTo(position);
    //m_pStateMachine->ChangeStateTo(STATE_MOVE);
    m_pCreature->doStopAt(x, y);
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

