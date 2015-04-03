c1y=clang++ -std=c++1y -I/usr/local/include -I/Users/liutaihua/GameState
#INCPATH = -I/usr/local/include

objects= StateMachine.o ReadyState.o IState.o IdleState.o DeadState.o CastingState.o MoveState.o Player.o Creature.o CreatureObserver.o main.o

all=./bin/gsm

bin/gsm:${objects}
	${c1y} -o $@ ${objects}
main.o:main.cc
	${c1y} -c $<

Creature.o:Creature.cc
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

CreatureObserver.o:CreatureObserver.cc
	${c1y} -c $<

Player.o:Player.cc
	${c1y} -c $<

#Utility.o:Utility.cc
#	${c1y} -c $<
#CreatureStandState.o:CreatureStandState.cc
#	${c1y} -c $<
#
#CreatureMoveState.o:CreatureMoveState.cc
#	${c1y} -c $<
#
#CreatureAttackState.o:CreatureAttackState.cc
#	${c1y} -c $<
#
#CreatureDieState.o:CreatureDieState.cc
#	${c1y} -c $<
#
#StateManager.o:StateManager.cc
#	${c1y} -c $<

clean:
	rm -rf *.o ${all}
#CreatureStandState.o:CreatureStandState.cc
#	${c1y} -c $<
#
#CreatureMoveState.o:CreatureMoveState.cc
#	${c1y} -c $<
#
#CreatureAttackState.o:CreatureAttackState.cc
#	${c1y} -c $<
#
#CreatureDieState.o:CreatureDieState.cc
#	${c1y} -c $<
#
#StateManager.o:StateManager.cc
#	${c1y} -c $<

clean:
	rm -rf *.o ${all}
