#include "GameHolderPCH.h"
#include <Monster.h>
#include <GameWorld.h>
#include <SkillAttack.h>

#include "AIMovingToTarget.h"


const float DEFAULT_ARRIVEDDISTANCE = 50;

AIMovingToTarget::AIMovingToTarget(Creature* pOwner, uint32 targetId, float maxDistance) :
	AIMoving(pOwner), WithTarget(pOwner->World(), targetId),
    m_BlockedCountdown(10), m_MaxDistance(maxDistance), m_ArrivedDistance(DEFAULT_ARRIVEDDISTANCE)
{
}

AIMovingToTarget::AIMovingToTarget(Creature *pOwner, WithTarget::TargetGetter getter, float maxDistance) :
	AIMoving(pOwner), WithTarget(pOwner->World(), getter),
    m_BlockedCountdown(10), m_MaxDistance(maxDistance), m_ArrivedDistance(DEFAULT_ARRIVEDDISTANCE)
{
}

CoVec2 AIMovingToTarget::Destination() const
{
	if (Blocked())
		throw LostTarget();

    //DBG("distance %0.2f, max %f", m_pOwner->DistanceFrom(*Target()), m_MaxDistance);
	if (m_pOwner->DistanceFrom(*Target()) > m_MaxDistance)
		throw LostTarget();

	m_LastPosition = Target()->GetPosition();
	return m_LastPosition;
}

bool AIMovingToTarget::Arrived() const
{
    return m_pOwner->DistanceFrom(*Target()) < m_ArrivedDistance;
}

bool AIMovingToTarget::Blocked() const
{
	return false;

	if ((m_pOwner->GetPosition() - m_LastPosition).Length() < (m_pOwner->StepLength() / 2))
		m_BlockedCountdown--;
	if (!m_BlockedCountdown)
	{
		m_BlockedCountdown = 10;
		return true;
	}
	return false;
}
