#pragma once

// 状态机(生物对象用, 不同的生物对象具体状态处理可能不同)

//#include "SkillDefine.h"
//#include "WorldObjectConstants.h"
#include "IState.h"
#include "Player.h"
#include "IdleState.h"
#include "ReadyState.h"
#include "CastingState.h"
#include "DeadState.h"
//#include "StunState.h"
//#include "movecastingstate.h"
#include "MoveState.h"
enum StateType
{
    STATE_READY = 0,
    STATE_IDLE,						// 正常状态
    STATE_MOVE,						// 移动
    STATE_CASTING,					// 施法
    STATE_STUN,						// 晕
    STATE_DEAD,						// 死亡
    STATE_MOVE_CASTING,             // 移动施法状态

    STATE_COUNT
};

class Player;
class IState;
//class SkillBase;
//class SkillCommon;

class StateMachine
{
    friend class IState;
public:
    StateMachine(Player* pPlayer);
    ~StateMachine();

    void Update(uint64 frameCount);

    void ChangeStateTo(int stateId);    // 状态转换(此函数应只由状态类调用)
    //void CanChangeStateTo(int stateId);

    int GetCurrentStateId() const {return m_curStateId;}

    // 生物行为接口
    //void MoveTo();                      // 移动
    //void StopAt();         // 停止
    //void Cast();      // 施法
    //void OnCasted();
    //void Stun();                      // 眩晕
    //void Dead();                     // 死亡
    void Reborn();                                                      // 重生
    //void MoveCast();

private:
    Player* m_pOwner;

    IState* m_pCurrentState;
    IState* m_AllStateMap[7];   // 此状态的机的所有状态

    int m_curStateId;

    ReadyState m_ReadyState;
    IdleState m_IdleState;
    CastingState  m_CastingState;     // 吟唱技能状态(记录要施展的法术)
//    StunState     m_StunState;      // 眩晕状态
    DeadState     m_DeadState;
//    MoveCastingState m_MoveCastingState;
    MoveState m_MoveState;
};

