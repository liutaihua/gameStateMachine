#include "Player.h"
#include "Utility.h"
#include "StateMachine.h"
#include <iostream>

Player::Player(int id, std::string name):
    user_id_(id),
    user_name_(name)
{
    move_distance_ = 0;
    move_speed_ = kSpeed;
    play_time_ = 0;
    //user_state_ = STATE_READY;
    hp_ = kMaxHp;
    m_pStateMachine = new StateMachine(this);
}

void Player::Update(uint64 frameCount){
    m_pStateMachine->Update(frameCount);
    int StateId = GetState();
    switch(StateId){
        case STATE_READY:

            m_pStateMachine->ChangeStateTo(STATE_IDLE);
            break;
        case STATE_IDLE:
            m_pStateMachine->ChangeStateTo(STATE_MOVE);
            break;
        case STATE_CASTING:
            m_pStateMachine->ChangeStateTo(STATE_DEAD);
            break;
        case STATE_MOVE:
            m_pStateMachine->ChangeStateTo(STATE_CASTING);
            break;
        case STATE_DEAD:
            m_pStateMachine->Reborn();
            break;
        default:
            std::cout<<"error\n";

}

    if(hp_ <= 0){
        //SetState(DIE);
    }
}

int Player::GetState(){
    return m_pStateMachine->GetCurrentStateId();
}

void Player::OnStateChanged(int oldStateId/*oldStateId*/, int newStateId)
{
    std::cout<<"Player State Changed TO: "<<std::to_string(newStateId)<<"\n";
    //std::cout<<"OnStateChanged, from:"<<std::to_string(oldStateId)<<"!!! to:"<<std::to_string(newStateId)<<"\n";
    //Monster* pMonster = CnDynamicCast(Monster, this);
    //PropertyChanged<STATUS>(newStateId, nullptr);
}

void Player::FindMonster(){
    monster_info_.count = 1;
    monster_info_.pos = 1;
}

void Player::doMoveTo(int x, int y){

}

void Player::doStopAt(int x, int y){

}

void Player::doDead(uint32 killerId)
{
    //m_pBuffManager->OnDie();
    OnDied(killerId);
}

void Player::OnDied(uint32 killerId){
    std::cout<<"I'm Dead, The killer is:"<<std::to_string(killerId)<<"\n";
}

void Player::Reborn(){
    m_pStateMachine->Reborn();
}

void Player::doReborn(){
    //LOG_W << "满血bug Error AIBoss::doReborn name " << GetName()<< std::endl;
    SetHp(GetMaxHp());
    //SetMp(GetMaxMp());

    //SigReborn();
}

int Player::GetMaxHp(){
    return 100;
}

void Player::SetHp(int hp){
    hp_ = hp;
}

void Player::Stand(float ts){
    int ret = 1;
    int guess = 0;
    while(true){
        std::cout << "guess the number in(0, 100):";
        std::cin >> guess;
        if(guess == ret){
            FindMonster();
            std::cout << "you have find "<< monster_info_.count << " monster\n";
            std::cout << "the pos is:" << monster_info_.pos << std::endl;
            //SetState(MOVE);
            break;
        }

        int lossHp = 1;
        hp_ = hp_ - lossHp;
        if(guess < ret){
            std::cout << "you are too small, loss hp:" << lossHp << "\n";
        }
        else{
            std::cout << "you are too big, loss hp:" << lossHp << "\n";
        }
        std::cout << "your current hp is:" << hp_ << std::endl;
    }
}

void Player::Move(float ts){
    move_distance_ += move_speed_ * ts;
    play_time_ += ts;
    if(play_time_ >= 1){
        std::cout << "the position is:" << move_distance_ << std::endl;
        play_time_ = 0;
    }

    if(move_distance_ >= monster_info_.pos){
        std::cout << "have reached the target:" << move_distance_ << std::endl;
        move_distance_ = 0;
        //SetState(ATTACK);
    }
}

void Player::Attack(float ts){
    if(monster_info_.count == 0){
        //SetState(STAND);
        return;
    }

    int monsterAttack = 1;
    while(true){
        if(hp_ <= 0){
            break;
        }

        int attack = 0;
        std::cout << "play your attack at "<< monster_info_.count <<" in(1, 100):";
        std::cin >> attack;

        if(attack == monsterAttack){
            monster_info_.count -= 1;
            std::cout << "you killed the monster\n";
            break;
        }

        int lossHp = 1;
        hp_ -= lossHp;
        if(attack > monsterAttack){
            std::cout << "your attack is too big, loss hp:" << lossHp << std::endl;
        }
        else{
            std::cout << "your attack is too small, loss hp:" << lossHp << std::endl;
        }
        std::cout << "your current hp is:" << hp_ << std::endl;
    }
}



bool Player::IsDie(){
    return false;
    //return user_state_ == DIE;
}

//void Player::SetState(PlayerState state){
//    if(state == user_state_){
//        return;
//    }
//    switch(state){
//        case STAND:
//            std::cout << "----begin stand--------\n";
//            break;
//        case MOVE:
//            std::cout << "----begin move--------\n";
//            break;
//        case ATTACK:
//            std::cout << "----begin attack--------\n";
//            break;
//        case DIE:
//            std::cout << "----begin die--------\n";
//            break;
//        default:
//            std::cout <<"the state is error";
//            break;
//    }
//    user_state_ = state;
//}
