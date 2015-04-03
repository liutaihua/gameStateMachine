#include "GameHolderPCH.h"

#include <GameWorld.h>
#include <Creature.h>
#include <SkillAttack.h>
#include <LambdaNew.h>

#include "AIAttacking.h"
#include "AIMovingToTarget.h"
#include "AIPatrol.h"

AIAttacking::AIAttacking(Creature* pOwner, uint32 targetId, int32 skillId,
                         LostTargetCallback callback) :
    AI(pOwner),
    WithTarget(pOwner->World(), targetId),
    m_SkillIdGetter(Crown::Getter<int32>(skillId)),
    m_SkillId(skillId),
    m_2thSkillId(0),
    m_LostTargetCallback(callback),
    m_AIMoving(nullptr)
{
	Init();
}

AIAttacking::AIAttacking(Creature *pOwner, WithTarget::TargetGetter getter,
                         SkillIdGetter skillIdGetter, SkillIdSetter skillIdSetter,
                         LostTargetCallback callback) :
    AI(pOwner), WithTarget(pOwner->World(), getter), m_SkillIdGetter(skillIdGetter),
    m_SkillId(skillIdGetter()), m_2thSkillId(0),
    m_SkillIdSetter(skillIdSetter),
    m_LostTargetCallback(callback),
    m_AIMoving(nullptr)
{
    CnAssert(getter != nullptr);
	Init();
}

void AIAttacking::Init()
{
    CnAssert(m_pOwner);
    Connect(m_pOwner->SigUpdated, boost::bind(&AIAttacking::CreatureUpdated, this));
}

void AIAttacking::CreatureUpdated()
{
    if (m_AIMoving)
        return;

    if (m_pOwner->GetState() == STATE_CASTING)
        return;

    Creature* pTarget;
	try
	{
		pTarget = Target();
	}
    catch (LostTarget&)
	{
        OnLostTarget();
		return;
	}

    if (pTarget->IsDead())
    {
        OnLostTarget();
        return;
    }

	float range = m_pOwner->DistanceFrom(*pTarget);

    // TODO: Make it shorter.
    int32 activeSkillId = m_SkillIdGetter();
    if (m_pOwner->IsPlayerMonster())
    {
        activeSkillId = m_pOwner->GetNextSkill();
    }
    std::unique_ptr<SkillBase> pSkill(m_pOwner->World()->GetSkillManager()->CreateSkill(activeSkillId,
                                                                                        0, m_pOwner->GetId(), pTarget->GetId()));

    //LOG_D << "cast skill : " << m_SkillIdGetter() << std::endl;
    if (!pSkill)
    {
        LOG_E << "AI ATTACKING Skill NULL id: " << activeSkillId << std::endl;
        CnAssert(pSkill);
        return;
    }

    if (range > pSkill->GetSkillInfo().Range
            && m_pOwner->GetMonsterClass() != MONSTER_WORLD_BOSS)
    {
        if (m_2thSkillId)
        {
            pSkill.reset(m_pOwner->World()->GetSkillManager()->CreateSkill(m_2thSkillId, 0, m_pOwner->GetId(), pTarget->GetId()));
            activeSkillId = m_2thSkillId;
        }
        if (!m_2thSkillId || range > pSkill->GetSkillInfo().Range)
        {
            m_AIMoving = CnNew AIMovingToTarget(m_pOwner, GetTargetGetter());
            m_AIMoving->SetCallbackAfterArrived(boost::bind(&AIAttacking::OnClosedToTarget, this));
            m_AIMoving->SetCallbackAfterLostTarget(boost::bind(&AIAttacking::OnLostTarget, this));
            m_AIMoving->SetArrivedDistance(pSkill->GetSkillInfo().Range - 1);
            return;
        }
    }

    // 技能cd中
    if (m_pOwner->GetCooldownLeft(activeSkillId) > 0)
        return;

    CoVec2 targetPos;
    CoVec2 dir;

    if (activeSkillId == 1002400)
    {
//        dir.x = m_pOwner->GetPosX() + Crown::SDRandom(100) - 50;
//        dir.y = m_pOwner->GetPosY() + Crown::SDRandom(100) - 50;
//        dir = dir - m_pOwner->GetPosition();
//        dir.Normalize();
//        targetPos = m_pOwner->GetPosition() + 500 * dir;
        targetPos = pTarget->GetPosition();
    }
    else if (activeSkillId == 1002500)
    {
//        dir.x = m_pOwner->GetPosX() + Crown::SDRandom(100) - 50;
//        dir.y = m_pOwner->GetPosY() + Crown::SDRandom(100) - 50;
//        dir = dir - m_pOwner->GetPosition();
//        dir.Normalize();
//        targetPos = m_pOwner->GetPosition() + 500 * dir;
        targetPos = m_pOwner->GetPosition();
    }
    else if (activeSkillId == 1002600)
    {
        dir = pTarget->GetPosition() - m_pOwner->GetPosition();
        dir.Normalize();
        targetPos = m_pOwner->GetPosition() + 1500 * dir;
    }
    else if (activeSkillId == 1002700)
    {
        targetPos = m_pOwner->World()->RandomReachablePlaceInCircle(m_pOwner->GetPosition(),
                                                                                    1000,
                                                                    m_pOwner->GetRadius());
    }
    else
    {
        targetPos = pTarget->GetPosition();
    }

    m_pOwner->Cast(activeSkillId, 0, pTarget->GetId(), targetPos.x, targetPos.y);
    if (activeSkillId == SKILL_LAVA_BOSS_FIRE_ALL)
    {
        m_SkillIdSetter(SKILL_LAVA_BOSS_HIT);
    }
    else if (activeSkillId == SKILL_DILAO_BOSS_FIRE_ALL)
    {
        m_SkillIdSetter(SKILL_DILAO_BOSS_HIT);
    }
    else if (activeSkillId == SKILL_SHAMO_BOSS_FIRE_ALL)
    {
        m_SkillIdSetter(SKILL_SHAMO_BOSS_HIT);
    }

    //LOG_D << "CAST ID:" << activeSkillId << std::endl;
    // Refresh skill after casted.
    //m_SkillId = m_SkillIdGetter();

}

void AIAttacking::OnLostTarget()
{
    if (m_LostTargetCallback)
        m_LostTargetCallback();
    DeleteThis();
}

void AIAttacking::OnClosedToTarget()
{
    m_AIMoving = nullptr;
}
