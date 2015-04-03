#include "GameHolderPCH.h"
#include <sdu.h>
#include <Creature.h>

#include "AIMoving.h"

//#include <boost/lambda/lambda.hpp>
//#include <boost/bind.hpp>


AIMoving::AIMoving(Creature *pOwner) :
    AI(pOwner),
    m_CallbackAfterArrived(nullptr)
{
    Connect(pOwner->SigUpdated, boost::bind(&AIMoving::CreatureUpdated, this));
}

AIMoving::~AIMoving()
{
    // If owner is destroyed, then it may be destructing.
    if (!m_pOwner->IsDestroyed())
        m_pOwner->SetActionState(OBJECT_ACTIONSTATE_STOP);
}

bool AIMoving::TryNextStep(CoVec2& candidate, float initialAngle)
{
	float angleDelta = 0;
    const float angleDeltaDiff = static_cast<float>(M_PI / 6.0/*12.0*/);
	do
	{
		candidate = m_pOwner->OneStepAwayByAngle(initialAngle + angleDelta);
		if (m_pOwner->GetCoObject()->CanMoveTo(candidate.x, candidate.y))
			return true;

		candidate = m_pOwner->OneStepAwayByAngle(initialAngle - angleDelta);
		if (m_pOwner->GetCoObject()->CanMoveTo(candidate.x, candidate.y))
			return true;

		angleDelta += angleDeltaDiff;
    } while(angleDelta < M_PI / 2.0);

	return false;
}

void AIMoving::CreatureUpdated()
{
    if (!m_pOwner)
        return;

	CoVec2 dest;
	try
	{
		dest = Destination();
	}
	catch (LostTarget&)
	{
		m_pOwner->StopAt(m_pOwner->GetPosition(), m_pOwner->GetAngle());
		if (m_CallbackAfterLostTarget)
			m_CallbackAfterLostTarget();
		DeleteThis();
		return;
	}

	if (Arrived())
	{
		m_pOwner->StopAt(m_pOwner->GetPosition(), m_pOwner->GetAngle());
		if (m_CallbackAfterArrived)
			m_CallbackAfterArrived();
		DeleteThis();
		return;
	}

	float interaction = m_pOwner->GetCoObject()->GetAngleIntersectionWithDirection(dest);

	// Turning face to target a little.
	float faceTo = m_pOwner->GetAngle() + interaction / 2.0f;
	CoVec2 newPosition;
	if (TryNextStep(newPosition, faceTo))
	{
		m_pOwner->MoveTo(newPosition);
	}
	else
	{
		m_pOwner->StopAt(m_pOwner->GetPosition(), m_pOwner->GetAngle());
	}
}

void AIMoving::SetCallbackAfterLostTarget(const std::function<void()> &callback)
{
	m_CallbackAfterLostTarget = callback;
}

void AIMoving::SetCallbackAfterArrived(const std::function<void()> &callback)
{
	m_CallbackAfterArrived = callback;
}


