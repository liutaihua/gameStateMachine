#include "CastingState.h"
#include "StateMachine.h"


enum{
    Cast_None = 0,
    Cast_PreCast,               // 吟唱, 把吟唱作为起手预警来用,这里自己不能打断.
    Cast_CastingTime,           // 出招
    Cast_Duration               // 持续施法
};

CastingState::CastingState(StateMachine* pStateMachine) :
IState(STATE_CASTING, pStateMachine),
//m_lastFrameCount(0),

m_x(0), m_y(0),
 m_PreCastTime(0),
 m_CastingTime(0),
 m_Duration(0),
 m_CastState(0)
{

}

CastingState::~CastingState()
{

}

void CastingState::Update( uint64 /*frameCount*/ )
{
    //m_lastFrameCount++;

    if(m_CastState == Cast_PreCast)
    {
        m_PreCastTime -= 1;
        if(m_PreCastTime <= 0)
        {
           m_CastState = Cast_CastingTime;         // 吟唱结束，进入出招状态
        }
    }
    else if(m_CastState == Cast_CastingTime)
    {
        m_CastingTime -= 1;
        if(m_CastingTime <= 0 && m_PreCastTime <= 0)
        {
//            if(0 < m_Duration)
//                m_CastState = Cast_Duration;            // 持续施法状态
//            else
                m_pStateMachine->ChangeStateTo(STATE_IDLE);			// 施法结束
        }
    }
//    else if(m_CastState == Cast_Duration)
//    {
//        m_Duration -= g_update_time;
//        if(m_lastFrameCount > (m_Duration+m_PreCastTime))
//        {
//            m_pStateMachine->ChangeStateTo(STATE_IDLE);			// 施法结束
//        }
//    }

}

void CastingState::doCast()
{

}

void CastingState::OnEnter( int32 lastStateId )
{

    doCast();
}

void CastingState::OnLeave(int32 nextStateId)
{
    m_CastState = 0;
}

void CastingState::MoveTo(Position& pos/*position*/)
{
//    if(m_CastState == Cast_Duration)
//        m_pStateMachine->ChangeStateTo(STATE_MOVE);
}

void CastingState::StopAt(Position& pos)
{
//    if(m_CastState == Cast_Duration)
//        m_pStateMachine->ChangeStateTo(STATE_IDLE);
}

void CastingState::Cast()
{
    // 时间为0的技能，可以重叠施法


}

void CastingState::OnCasted()
{
    m_pStateMachine->ChangeStateTo(STATE_IDLE);
}

void CastingState::Dead()
{
    m_pStateMachine->ChangeStateTo(STATE_DEAD);
}

void CastingState::MoveCast()
{
    m_pStateMachine->ChangeStateTo(STATE_MOVE_CASTING);
}

void CastingState::Stun()
{
    m_pStateMachine->ChangeStateTo(STATE_STUN);
}
