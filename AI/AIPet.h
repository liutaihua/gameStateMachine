#pragma once

#include "AIPatrol.h"

class Creature;
class AIMoing;
class AIAttacking;
struct Position;

class AIPet : public AIPatrol
{
public:
    AIPet(Creature* pOwner, Creature* following = nullptr);

private:
    void Init(Creature* pOwner);
    virtual CoVec2 GetCentre() const;
    virtual AIMoving* CreateMovingAI();
    void OnUpdated();
    void OnOwnerMoved();
    void OnFollowingCreatureMoved();
    void OnFollowingCreatureDestroyed();

    void OnFollowDead();

    void ReverseDetectTarget(); // detect nearby enemies and let their ais set target to me.

private:
    Creature* m_Following;
};

