#pragma once
#include "fd/delegate.hpp"
#include <string>
#include <map>
//#include "Utility.h"
//#include "StateMachine.h"
//#include "CreatureObserver.h"
//#include "Pet.h"
#include "Creature.h"



class CreatureObserver;
class Creature;
struct Position;

class Player : public Creature

{
public:
    Player(int id, std::string name);
    ~Player(){}
    void FindMonster();

    fd::delegate<void()> SigMoved;
    //void SetState(IState* state);
    virtual void Update(uint64 frameCount);



private:
    int user_id_;
    int32 killerId;
    int32 m_petId;
    MonsterInfo monster_info_;

    std::string  user_name_;


    //StateType user_state_;
};
