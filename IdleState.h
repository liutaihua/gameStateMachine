#pragma once

// 待机状态
#ifndef IdleState_h__
#define IdleState_h__

#include "IState.h"
//#include "Utility.h"

class StateMachine;
class IState;

class IdleState : public IState
{
public:
	IdleState(StateMachine* pStateMachine);
	virtual ~IdleState();

    virtual void Update( uint64 frameCount) {}
    virtual void OnEnter(int /*lastStateId*/);
    virtual void OnLeave(int /*nextStateId*/);
    virtual void MoveTo(Position& pos/*nextStateId*/);
};

#endif // IdleState_h__
