#include "ReadyState.h"
#include "Utility.h"
#include "StateMachine.h"


ReadyState::ReadyState(StateMachine* pStateMachine) :
IState(STATE_READY, pStateMachine)
{
}

ReadyState::~ReadyState()
{

}

void ReadyState::Update( uint64 frameCount)
{
  Init();
  m_pStateMachine->ChangeStateTo(STATE_READY);   // 初始化完成，进入idle状态
}

void ReadyState::Init()
{ // 状态机初始化

}

void ReadyState::OnEnter(int stateId) {
    std::cout<<"I am Ready From:"<<std::to_string(stateId)<<"\n";
}

void ReadyState::OnLeave(int stateId){
    std::cout<<"I am leave Ready to:"<<std::to_string(stateId)<<"\n";
}
