#pragma once
#include <string>
#include <map>
#include "Utility.h"
#include "StateMachine.h"



static const int kMaxHp = 100;
static const float kSpeed = 2.0f;

struct MonsterInfo{
    int count;
    int pos;
};

class Player{
    public:
        Player(int id, std::string name);
        ~Player(){}
        void FindMonster();

        //void SetState(IState* state);
        void Update(uint64 frameCount);
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

        void doMoveTo(int x ,int y);
        void doStopAt(int x, int y);

        int GetState();
        int GetMaxHp();
        void SetHp(int);
        void OnStateChanged(int oldStateId, int newStateId);

    private:
        int hp_;
        int user_id_;
        int monster_count_;
        float move_speed_;
        float move_distance_;
        float play_time_;
        int32 killerId;
        MonsterInfo monster_info_;
        std::string  user_name_;
        StateMachine*	m_pStateMachine;

        //StateType user_state_;
};
