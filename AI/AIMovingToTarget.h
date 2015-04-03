#pragma once

#ifndef AIMOVINGTOTARGET_H
#define AIMOVINGTOTARGET_H

#include "AIMoving.h"
#include "WithTarget.h"
#include "Monster.h"

class Creature;

class AIMovingToTarget: public AIMoving, public WithTarget
{
	mutable CoVec2 m_LastPosition;
	mutable uint32 m_BlockedCountdown;
	float m_MaxDistance;
    float m_ArrivedDistance;

public:
    AIMovingToTarget(Creature* pOwner, uint32 targetId, float maxDistance = Monster::ALERT_RANGE * 10);
    AIMovingToTarget(Creature* pOwner, TargetGetter, float maxDistance = Monster::ALERT_RANGE * 10);

	virtual CoVec2 Destination() const;
	virtual bool Arrived() const;

	virtual bool Blocked() const;

    void SetArrivedDistance(float dist) { m_ArrivedDistance = dist; }
};

#endif // AIMOVINGTOTARGET_H
