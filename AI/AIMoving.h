#pragma once

#ifndef AIMOVING_H
#define AIMOVING_H

#include "AI.h"
//#include "CoMath.h"

class Creature;

class AIMoving : public AI
{
protected:
	std::function<void()> m_CallbackAfterLostTarget;
	std::function<void()> m_CallbackAfterArrived;

public:
    AIMoving(Creature* pOwner);

    ~AIMoving();

	virtual CoVec2 Destination() const = 0;
	virtual bool Arrived() const = 0;

	bool TryNextStep(CoVec2& result, float initialAngle);

	void CreatureUpdated();

	void SetCallbackAfterLostTarget(const std::function<void()>& callback);
	void SetCallbackAfterArrived(const std::function<void()>& callback);
};

#endif // AIMOVING_H
