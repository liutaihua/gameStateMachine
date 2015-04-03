#pragma once
#ifndef CREATUREOBSERVER_H
#define CREATUREOBSERVER_H

#include "fd/delegate.hpp"
#include "boost/bind.hpp"
//#include "Creature.h"


class Creature;


// CreatureObserver is supposed to live as long as its owner, a GameWorld.
// So, it will be deleted after all the creatures in the world deleted.
class CreatureObserver
{
public:
    CreatureObserver();

    void Observe(Creature *obj);

    // Signals
    fd::delegate<void(Creature*)> SigCreatureMoved;



private:
    void ObserveCreature(Creature* pCreature);

    void OnCreatureMove(Creature* pCreature);
};


#endif
