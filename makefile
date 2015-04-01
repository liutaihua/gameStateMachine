c1y=clang++ -std=c++1y

objects=main.o Player.o StateMachine.o ReadyState.o IState.o IdleState.o DeadState.o CastingState.o MoveState.o
all=./bin/gsm

bin/gsm:${objects}
	${c1y} -o $@ ${objects}
main.o:main.cc
	${c1y} -c $<

Player.o:Player.cc
	${c1y} -c $<

StateMachine.o:StateMachine.cc
	${c1y} -c $<

ReadyState.o:ReadyState.cc
	${c1y} -c $<

IState.o:IState.cc
	${c1y} -c $<

IdleState.o:IdleState.cc
	${c1y} -c $<

DeadState.o:DeadState.cc
	${c1y} -c $<

MoveState.o:MoveState.cc
	${c1y} -c $<

CastingState.o:CastingState.cc
	${c1y} -c $<

#Utility.o:Utility.cc
#	${c1y} -c $<
#PlayerStandState.o:PlayerStandState.cc
#	${c1y} -c $<
#
#PlayerMoveState.o:PlayerMoveState.cc
#	${c1y} -c $<
#
#PlayerAttackState.o:PlayerAttackState.cc
#	${c1y} -c $<
#
#PlayerDieState.o:PlayerDieState.cc
#	${c1y} -c $<
#
#StateManager.o:StateManager.cc
#	${c1y} -c $<

clean:
	rm -rf *.o ${all}
#PlayerStandState.o:PlayerStandState.cc
#	${c1y} -c $<
#
#PlayerMoveState.o:PlayerMoveState.cc
#	${c1y} -c $<
#
#PlayerAttackState.o:PlayerAttackState.cc
#	${c1y} -c $<
#
#PlayerDieState.o:PlayerDieState.cc
#	${c1y} -c $<
#
#StateManager.o:StateManager.cc
#	${c1y} -c $<

clean:
	rm -rf *.o ${all}
