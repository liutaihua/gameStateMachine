#pragma once

// 施法状态

#include "IState.h"
#include "Utility.h"

class CastingState : public IState
{
public:
    CastingState(StateMachine* pStateMachine);
    virtual ~CastingState();

    virtual void OnEnter(int lastStateId);
    virtual void OnLeave(int nextStateId);

    virtual void Update( uint64 frameCount );

    virtual void MoveTo(int x, int y /*position*/);
    virtual void StopAt(int x, int y);
    virtual void Cast();
    virtual void OnCasted();
    virtual void Stun();
    virtual void Dead();
    virtual void MoveCast();

    // 技能信息
    float	m_x;
    float	m_y;

private:
    void    doCast();

private:
    //int32	m_lastFrameCount;		// 持续帧数

    int32   m_CastState;            // 1:吟唱中 2:出招中 3:持续施法中

    int32       m_PreCastTime;      // 吟唱时间
    int32       m_CastingTime;      // 出招时间
    int32       m_Duration;         // 持续施法时间


};
