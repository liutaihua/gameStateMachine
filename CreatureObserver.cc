#include "CreatureObserver.h"

#include "Creature.h"


CreatureObserver::CreatureObserver()
{
}

void CreatureObserver::Observe(Creature* obj)
{
    ObserveCreature(obj);
}


void CreatureObserver::ObserveCreature(Creature* pCreature)
{
    // boost::bind 默认会复制函数对象, 而fd::delegate不能被复制,所以改用 boost::ref(fd::delegate) 传入一个引用.
    pCreature->SigMoved.add(boost::bind(&CreatureObserver::OnCreatureMove, this, pCreature));
    //pCreature->SigForceRepositioned.add(boost::bind(boost::ref(SigForceRepositioned), pCreature, _1, _2));

    //pCreature->SigRepositioned.add(boost::bind(boost::ref(SigRepositioned), pCreature));
    //pCreature->SigIllegalSkill.add(boost::bind(boost::ref(SigIllegalSkill), pCreature, _1, _2, _3));

    //pCreature->SigDied.add(boost::bind(boost::ref(SigCreatureDied), pCreature, _1));
    //pCreature->SigKilled.add(boost::bind(boost::ref(SigCreatureKilled), pCreature, _1));
    //pCreature->SigReborn.add(boost::bind(boost::ref(SigCreatureReborn), pCreature));
    //pCreature->SigHurt.add(boost::bind(boost::ref(SigCreatureHurt), pCreature, _1, _2));

}



void CreatureObserver::OnCreatureMove(Creature* pCreature)
{
    std::cout<<"CreatureObserver::OnCreatureMove"<<"\n";
    SigCreatureMoved(pCreature);
}
