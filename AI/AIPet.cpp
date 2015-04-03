#include <Creature.h>
#include <Monster.h>


#include "AIAttacking.h"
#include "AIMovingToTarget.h"
#include "AIPet.h"
#include "Pet.h"
//#include "AITargetedBase.h"

AIPet::AIPet(Creature* pOwner, Creature* following)
    : AIPatrol(pOwner),
    m_Following(following)
{
    Init(pOwner);

    //DisableRandomMovement();
}


void AIPet::OnUpdated()
{

}
void AIPet::Init(Creature *pOwner)
{
    //空函数把统一下
//    Connect(pOwner->SigUpdated, boost::bind(&AIPet::OnUpdated, this));
    Connect(m_pOwner->SigMoved, boost::bind(&AIPet::OnOwnerMoved, this));
//    Connect(m_Following->SigDestroyed, boost::bind(&AIPet::OnFollowingCreatureDestroyed, this));
    Connect(m_Following->SigMoved, boost::bind(&AIPet::OnFollowingCreatureMoved, this));
//    Connect(m_Following->SigHurt, boost::bind(&AIPet::OnFollowingCreatureHurted, this, _1, _2));
//    Connect(m_Following->SigDied, boost::bind(&AIPet::OnFollowDead, this));
//    SetRandomMovementRadius(m_Following->GetRadius() * 4);
}

void AIPet::OnOwnerMoved()
{
    // FIXME: Not to check in EVERY FRAME.

    if (m_pOwner->GetActionState() == OBJECT_ACTIONSTATE_STOP)
        return;

    if (!m_Following)
        return;


    // Force pet to stop.
    // Bad solution, we had better check the AIMovingToPosition::Arrived().
    if (m_Following->GetActionState() == OBJECT_ACTIONSTATE_STOP
            && GetDistanceFromCentre(*m_pOwner) < GetRandomMovementRadius())
    {
        DBG("AIPet::OnOwnerMoved");
        m_pOwner->StopAt(m_pOwner->GetPosition(), m_pOwner->GetAngle());

        DisableRandomMovement();
    }

    ReverseDetectTarget();
}

void AIPet::ReverseDetectTarget()
{
//    CnAssert(m_pOwner);

//    if (!m_pOwner) return;
//    if (!m_pOwner->World()) return;

//    std::vector<CoObjectOwner*> coObjs;
//    m_pOwner->World()->GetCoWorld().GetObjectInRoundness(
//                coObjs, m_pOwner->GetPosition(), Monster::ALERT_RANGE);
//    for (auto itor = coObjs.begin(); itor != coObjs.end(); ++itor)
//    {
//        Monster* monster = dynamic_cast<Monster*>(*itor);
//        if (!monster) continue;

//        if (!AIBase::IsCandidateTarget(*monster, *m_pOwner)) continue;

//        AIBase* ai = monster->GetAI();
//        if (!ai) continue;

//        AITargetedBase* tai = dynamic_cast<AITargetedBase*>(ai);
//        if (!tai) continue;

//        tai->SetTargetId(0);
//        tai->SetTargetId(m_pOwner->GetId());
//    }
}

void AIPet::OnFollowingCreatureMoved()
{
    // FIXME: Not to check in EVERY FRAME.


    if (m_Following && TooFarAwayFromCentre(*m_pOwner))
    {
        DBG("AIPet::OnFollowingCreatureMoved TooFarAwayFromCentre");
        m_pOwner->JumpTo(m_Following->GetPosition());
        StopRandomMove();
    }

    if (m_Following && m_Following->GetActionState() == OBJECT_ACTIONSTATE_MOVING
            || GetDistanceFromCentre(*m_pOwner) > GetRandomMovementRadius())
    {
        //DBG("AIPet::OnFollowingCreatureMoved OBJECT_ACTIONSTATE_MOVING");
        EnableRandomMovement();
        // Force owner to move.
        if (m_RandomMovementAI == nullptr)
            m_NextActionCountdown = 0;
    }
    else
    {
        //DisableRandomMovement();
    }
}

void AIPet::OnFollowingCreatureDestroyed()
{
    m_Following = nullptr;
    m_pOwner->Destroy();
}

void AIPet::OnFollowingCreatureHurted(DamageType, SkillBase* pSkill)
{
//    if (!GetTargetId())
//        SetTargetId(pSkill->GetCasterId());
}

void AIPet::OnFollowDead()
{
   Pet* pPet = CnDynamicCast(Pet, m_pOwner);
    CnAssert(pPet);
    pPet->DisableReborn();
    m_pOwner->Die(0);
}

CoVec2 AIPet::GetCentre() const
{
    if (m_Following)
        return m_Following->GetPosition();
    return AIPatrol::GetCentre();
}

AIMoving *AIPet::CreateMovingAI()
{
    if (m_Following)
    {
        auto ai = CnNew AIMovingToTarget(m_pOwner, m_Following->GetId(), FLT_MAX);
        ai->SetCallbackAfterArrived(boost::bind(&AIPatrol::OnRandomMovementEnd, this));
        ai->SetArrivedDistance(m_pOwner->GetRadius() + m_Following->GetRadius() + 80 - 25);
        return ai;
    }

    return NULL;
}
