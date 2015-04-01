#include "StateMachine.h"
#include "Utility.h"
#include "ReadyState.h"
#include "IdleState.h"
//#include "MoveState.h"
//#include "CastingState.h"
//#include "StunState.h"
//#include "DeadState.h"
//#include "Player.h"


StateMachine::StateMachine(Player* pCreature) :
    m_pOwner(pCreature),
    m_ReadyState(this),
    m_IdleState(this),
    m_MoveState(this),
    m_CastingState(this),
//    m_StunState(this),
    m_DeadState(this)
//    m_MoveCastingState(this)
{
    // 根据不同类型的对象，创建对应的状态
    m_AllStateMap[STATE_READY]		= &m_ReadyState;
    m_AllStateMap[STATE_IDLE]		= &m_IdleState;
    m_AllStateMap[STATE_MOVE]		= &m_MoveState;
    m_AllStateMap[STATE_CASTING]	= &m_CastingState;
//    m_AllStateMap[STATE_STUN]		= &m_StunState;
    m_AllStateMap[STATE_DEAD]		= &m_DeadState;
//    m_AllStateMap[STATE_MOVE_CASTING]		= &m_MoveCastingState;

	m_pCurrentState = m_AllStateMap[STATE_READY];
    m_curStateId = STATE_READY;

}

StateMachine::~StateMachine()
{
//	for(auto p = m_AllStateMap.begin(); p != m_AllStateMap.end(); ++p)
//	{
//		CnDelete p->second;
//	}
    //m_AllStateMap.clear();
}

void StateMachine::Update( uint64 frameCount )
{

	m_pCurrentState->Update(frameCount);
}

void StateMachine::ChangeStateTo(int stateId)
{
    auto nextStateId = stateId;

    if (m_curStateId != nextStateId)
	{		
        IState*	pNextState = m_AllStateMap[nextStateId];
		if(pNextState)
		{
//            if (m_pOwner->GetObjectType() == OBJ_MONSTER
//                    && nextStateId == STATE_IDLE)
//            {
//                std::cout << "!!!!!!!!!!" << std::endl;
//            }
            int oldStateId = m_curStateId;
            m_pCurrentState->OnLeave(nextStateId);		// 离开前一个状态

            m_pCurrentState = pNextState;
            m_curStateId = nextStateId;
            m_pCurrentState->OnEnter(m_curStateId);				// 进入下一个状态

            m_pOwner->OnStateChanged(oldStateId, m_curStateId);
            //std::cout<< "OnStateChanged : " << std::to_string(m_curStateId) << "\n";
		}
		else
		{
            //CnVerify(false);			// 要转换的状态不存在
		}
    }
}


//void StateMachine::MoveTo(const CoVec2& position)
//{
//	m_pCurrentState->MoveTo(position);
//}
//
//void StateMachine::StopAt( const CoVec2& position, float angle )
//{
//	m_pCurrentState->StopAt(position, angle);
//}

//void StateMachine::OnCasted(SkillBase*)
//{
//    m_pCurrentState->OnCasted();
//}

//void StateMachine::Stun( int durFrame )
//{
//    if (m_StunState.m_durFrameCount < durFrame)
//        m_StunState.m_durFrameCount = durFrame;
//
//	m_pCurrentState->Stun();
//}
//
//void StateMachine::Dead(int killerId)
//{
//    m_DeadState.KillerId = killerId;
//	m_pCurrentState->Dead();
//}
//
void StateMachine::Reborn()
{
    m_pCurrentState->Reborn();
}
