#pragma once

// 移动状态
#ifndef MoveState_h__
#define MoveState_h__

#include "IState.h"
class IState;

class MoveState : public IState
{
public:
    MoveState(StateMachine* pStateMachine);
    virtual ~MoveState();

    virtual void Update( uint64 /*frameCount*/ );
    virtual void MoveTo(Position& pos);
    virtual void StopAt(Position& pos);
    virtual void Cast();
    virtual void Dead();

private:
    uint64 moveFrameCount;
};

#endif // MoveState_h__
