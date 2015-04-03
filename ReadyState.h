#pragma once

// 状态机初始化的状态
#ifndef ReadyState_h__
#define ReadyState_h__

#include "IState.h"
class IState;

class ReadyState : public IState
{
public:
  ReadyState(StateMachine* pStateMachine);
  virtual ~ReadyState();

  virtual void Update( uint64 frameCount );
  virtual void OnEnter(int stateId/*lastStateId*/);
  virtual void OnLeave(int stateId/*nextStateId*/);

  void Init();

};

#endif // ReadyState_h__

