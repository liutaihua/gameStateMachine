#include "Player.h"
#include "Utility.h"
#include "StateMachine.h"
#include <iostream>
//#include "AI/AIPet.h"

#define CnNew   new

Player::Player(int id, std::string name):
    Creature(OBJ_PLAYER),
    user_id_(id),
    m_petId(OBJ_PET),
    user_name_(name)
{
//    m_Pet = CnNew Pet(m_petId)
//    CnNew AIPet(m_Pet, this);
}


void Player::Update(uint64 frameCount){
    Creature::Update(frameCount);
    int StateId = GetState();
    switch(StateId){
        case STATE_READY:
            std::cout<<"Creature::Update got STATE_READY"<<"\n";
            ChangeStateTo(STATE_IDLE);
            break;
        case STATE_IDLE:
            std::cout<<"Creature::Update got STATE_IDLE"<<"\n";
            ChangeStateTo(STATE_MOVE);
            //SigMoved();
            break;
        case STATE_CASTING:
            std::cout<<"Creature::Update got STATE_CASTING"<<"\n";
            ChangeStateTo(STATE_DEAD);
            break;
        case STATE_MOVE:
            //std::cout<<"Creature::Update got STATE_MOVE"<<"\n";
            Position nextPos;
            //Position currentPosition = GetPosition();
            nextPos.x = GetPosition().x + rand() % 100;
            nextPos.y = GetPosition().y + rand() % 100;
            MoveTo(nextPos);
            //m_pStateMachine->MoveTo(pos);
            //m_pStateMachine->ChangeStateTo(STATE_CASTING);
            break;
        case STATE_DEAD:
            std::cout<<"Creature::Update got STATE_DEAD"<<"\n";
            Reborn();
            break;
        default:
            std::cout<<"error\n";
      }
}

//int Player::GetState(){
//    return m_pStateMachine->GetCurrentStateId();
//}


void Player::FindMonster(){
    monster_info_.count = 1;
    monster_info_.pos = 1;
}


