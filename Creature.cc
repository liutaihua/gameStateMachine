#include "Creature.h"
#include "Utility.h"
#include "StateMachine.h"
#include <iostream>

#define CnNew   new

Creature::Creature(enum ObjectType objType) :
    m_pCreatureObserver(CnNew CreatureObserver)
{
    move_distance_ = 0;
    move_speed_ = kSpeed;
    m_pStateMachine = new StateMachine(this);
    m_pCreatureObserver->Observe(this);
    m_pCreatureObserver->SigCreatureMoved.add(boost::bind(&Creature::OnCreatureMoved, this, _1));
}

void Creature::OnCreatureMoved(Creature* pCreature){
    std::cout<<"Creature::OnCreatureMoved Invoked by Observer"<<"\n";
}

void Creature::Update(uint64 frameCount){

}

void Creature::ChangeStateTo(int stateId){
    m_pStateMachine->ChangeStateTo(stateId);
}

int Creature::GetState(){
    return m_pStateMachine->GetCurrentStateId();
}

void Creature::OnStateChanged(int oldStateId/*oldStateId*/, int newStateId)
{
    //std::cout<<"Creature State Changed TO: "<<std::to_string(newStateId)<<"\n";
    //std::cout<<"OnStateChanged, from:"<<std::to_string(oldStateId)<<"!!! to:"<<std::to_string(newStateId)<<"\n";
    //Monster* pMonster = CnDynamicCast(Monster, this);
    //PropertyChanged<STATUS>(newStateId, nullptr);
}

void Creature::MoveTo(Position& pos){
    m_pStateMachine->MoveTo(pos);
}


void Creature::doMoveTo(Position& pos){
    if (pos.x > 10000)
        pos.x = 0;
    if (pos.y > 10000)
        pos.y = 0;
    std::cout<<"Creature doMoveTo-->: "<<std::to_string(pos.x)<<" , "<<std::to_string(pos.y)<<"\n";
    m_position = pos;
    SigMoved();
}

void Creature::doStopAt(Position& pos){

}

void Creature::doDead(uint32 killerId)
{
    //m_pBuffManager->OnDie();
    OnDied(killerId);
}

void Creature::OnDied(uint32 killerId){
    std::cout<<"I'm Dead, The killer is:"<<std::to_string(killerId)<<"\n";
}

bool Creature::IsDie(){
    return m_pStateMachine->GetCurrentStateId() == STATE_DEAD;
}

void Creature::Reborn(){
    m_pStateMachine->Reborn();
}

void Creature::doReborn(){
    //LOG_W << "满血bug Error AIBoss::doReborn name " << GetName()<< std::endl;
    SetHp(GetMaxHp());
    std::cout<<"I'm Reborn"<<"\n";
    //SetMp(GetMaxMp());

    //SigReborn();
}


int Creature::GetMaxHp(){
    return 100;
}

void Creature::SetHp(int hp){
    m_hp = hp;
}


