#pragma once

// 死亡状态
#ifndef DeadState_h__
#define DeadState_h__

#include "IState.h"

class DeadState : public IState
{
public:
    DeadState(StateMachine* pStateMachine);
    virtual ~DeadState();

    virtual void OnEnter(int lastStateId);
    virtual void Update( uint64 /*frameCount*/ ) {}
    virtual void Dead();
    virtual void Reborn();

    uint32 KillerId;
};

#endif // DeadState_h__
