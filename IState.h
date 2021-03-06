#pragma once

// 状态基类
#ifndef IState_h__
#define IState_h__
#include "Utility.h"
//#include "Creature.h"
//#include "StateMachine.h"


//struct Position;
class StateMachine;
class Creature;

class IState
{
public:
  IState(int type, StateMachine* pStateMachine);
  virtual ~IState() {}

  virtual void Update( uint64 frameCount ) = 0;

  virtual void OnEnter(int stateId/*lastStateId*/) {}
  virtual void OnLeave(int stateId/*nextStateId*/) {}

  // 行为接口
  virtual void MoveTo(Position& pos/*position*/) {}
  //virtual void StopAt(const CoVec2& /*position*/, float /*angle*/) {}
  virtual void Cast() {}
  //virtual void OnCasted() {}
  //virtual void Stun() {}
  virtual void Dead() {}
  virtual void Reborn() {}
  //virtual void MoveCast() {}


  // 行为接口
  //virtual void MoveTo(const CoVec2& /*position*/) {}
  //virtual void StopAt(const CoVec2& /*position*/, float /*angle*/) {}

protected:
  StateMachine* m_pStateMachine;
  Creature*   m_pCreature;

private:
  int m_type;
  
};

#endif // IState_h__

