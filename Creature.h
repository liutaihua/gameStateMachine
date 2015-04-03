#pragma once

#include <string>
#include <map>
#include "Utility.h"
//#include "StateMachine.h"
#include "CreatureObserver.h"
//#include "Pet.h"



static const int kMaxHp = 100;
static const float kSpeed = 2.0f;

struct MonsterInfo{
    int count;
    int pos;
};
class CreatureObserver;
class StateMachine;

struct Position;

enum ObjectType
{
    OBJ_MANAGER,				// 各管理器
    OBJ_PLAYER,					// 玩家角色
    OBJ_MONSTER,				// 怪
    OBJ_OPERATOR,				// 操作器
    OBJ_SKILLOBJECT,			// 技能对象
    OBJ_TRIGGER,				// 触发器
        OBJ_AREA,                   // 区域
    OBJ_PET,                    //宠物
    OBJ_OTHER,					// 其他?
    OBJECT_TYPE_COUNT
};


class Creature
        //Creature(const Creature&);		// 无拷贝构造函数，无实现
{
    public:

        Creature(enum ObjectType objType);
        ~Creature(){}

        fd::delegate<void()> SigMoved;
        //void SetState(IState* state);
        virtual void Update(uint64 frameCount);
        bool IsDie();

        void Move(float ts);
        void Attack(float ts);
        void Stop();
        void Stand(float ts);
        //void StopAt(const CoVec2& position, float angle);			// 在某位置停止
        //virtual void JumpTo(const CoVec2& position);
        void Stun(int32 durFrame);
        void Die(uint32 killerId = 0);
        void Reborn();
        void doReborn();
        void doDead(uint32 killerId);
        void OnDied(uint32 killerId);

        void MoveTo(Position& pos);
        void doMoveTo(Position& pos);
        void doStopAt(Position& pos);

        int GetState();

        void OnStateChanged(int oldStateId, int newStateId);
        void OnCreatureMoved(Creature* pCreature);
        void ChangeStateTo(int stateId);

        int GetMaxHp();
        void SetHp(int);

        Position& GetPosition(){ return m_position;}

    private:

        float move_speed_;
        float move_distance_;

        int32 killerId;
        int m_hp;
        MonsterInfo monster_info_;
        Position m_position;

        StateMachine*	m_pStateMachine;

        std::unique_ptr<CreatureObserver> m_pCreatureObserver;

        //StateType user_state_;
};
