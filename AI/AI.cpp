#include "GameHolderPCH.h"
#include <Creature.h>

#include "AI.h"

AI::AI(Creature *pOwner) :
	Bindable(pOwner)
{    
    Connect(pOwner->SigDied, boost::bind(&AI::DeleteThis, this));
}

AI::~AI()
{

}
