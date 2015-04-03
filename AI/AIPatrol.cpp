#include "GameHolderPCH.h"
#include <sdu.h>
#include <Creature.h>
#include <Monster.h>
#include <GameWorld.h>
#include <CoWorld.h>
#include <LambdaNew.h>

#include "AIAttacking.h"
#include "AIMovingToTarget.h"
#include "AIMovingToPosition.h"
#include "AIPatrol.h"
#include "BattleSystem.h"


AIPatrol::AIPatrol(Creature* pOwner, float followDist) :
    AI(pOwner),
    m_NextActionCountdown(0),
    m_RandomMovementAI(nullptr),
    m_AttackingAI(nullptr),
    m_Centre(pOwner->GetPosition()),
    m_MaxFollowDistance(followDist),
    m_SearchRadius(Monster::ALERT_RANGE),
    m_SkillId(pOwner->GetMonsterSkillId()),
    m_2thSkillId(0),
    m_EnableRandomMovement(false),
    m_RandomMovementRadius(100),
    m_TargetId(0)
{
    if (pOwner->IsPlayerMonster())
    {
        m_SearchRadius = Monster::ALERT_RANGE * 2.0f;
    }
    Init(pOwner);
}

AIPatrol::~AIPatrol()
{
}

void AIPatrol::SetTargetId(uint32 id)
{
    if (!m_TargetId)
    {
        StopRandomMove();

        m_AttackingAI = CnNew AIAttacking(
            m_pOwner,
            boost::bind(&AIPatrol::GetTarget, this),
            boost::bind(&AIPatrol::GetSkillId, this),
            boost::bind(&AIPatrol::SetSkillId, this, _1),
            boost::bind(&AIPatrol::OnLostTarget, this));
        if (m_2thSkillId)
            m_AttackingAI->Set2thSkillId(m_2thSkillId);
    }
    m_TargetId = id;
}

void AIPatrol::SetSkillId(int32 id)
{
    m_SkillId = id;
}

void AIPatrol::Set2thSkillId(int32 id)
{
    m_2thSkillId = id;
}

void AIPatrol::RandomTarget()
{
    std::vector<Creature*> container;

    const float ownerRadius = m_pOwner->GetRadius();
    const float step = 50;
    for (float searchRadius = ownerRadius + step; searchRadius <= m_SearchRadius; searchRadius += step)
    {
        m_pOwner->World()->GetCoWorld().GetInRoundness<Creature>(container, GetCentre(), searchRadius,
            boost::bind(&AIPatrol::ForceIdFilter, this, _1));

        if (container.size())
        {
            SetTargetId(container[Crown::SDRandom(container.size())]->GetId());
            break;
        }
    }
}

float AIPatrol::GetDistanceFromCentre(const Creature &obj) const
{
    return (obj.GetPosition() - GetCentre()).Length();
}

void AIPatrol::DisableRandomMovement()
{
    StopRandomMove();

    if (m_pOwner->GetActionState() != OBJECT_ACTIONSTATE_STOP)
    {
        // FIXME: We need a method 'Creature::Stop()'
        m_pOwner->StopAt(m_pOwner->GetPosition(), m_pOwner->GetAngle());
    }
    m_EnableRandomMovement = false;
}

void AIPatrol::Init(Creature *pOwner)
{
    Connect(pOwner->SigHpChanged, boost::bind(&AIPatrol::OnHpChanged, this, _1, _2));

    Connect(pOwner->SigUpdated, boost::bind(&AIPatrol::OnUpdated, this));
}

void AIPatrol::OnOtherRepositioned(Creature* obj)
{
    if (m_TargetId)
        return;

    IterationCallback(*obj);
}

void AIPatrol::OnHpChanged(float amount, SkillBase *pSkill)
{
    if (m_TargetId)
        return;

    if (!pSkill || amount > 0)
        return;

    m_TargetId = pSkill->GetCasterId();

    m_AttackingAI = CnNew AIAttacking(
        m_pOwner,
        boost::bind(&AIPatrol::GetTarget, this),
        boost::bind(&AIPatrol::GetSkillId, this),
        boost::bind(&AIPatrol::SetSkillId, this, _1),
        boost::bind(&AIPatrol::OnLostTarget, this));
    if (m_2thSkillId)
        m_AttackingAI->Set2thSkillId(m_2thSkillId);

    StopRandomMove();
}

void AIPatrol::OnUpdated()
{
    if (m_TargetId)
        return;

    if (m_pOwner->World()->GetCurrentFrame() % 5 != 0)
        return;

    // 搜索附近可攻击的敌人.
    ProgressiveSearch();

    if (m_pOwner->GetMonsterClass() != MONSTER_WORLD_BOSS)
        RandomMove();
}

void AIPatrol::OnAttackEnd()
{
    m_TargetId = 0;
    m_AttackingAI = nullptr;
}

void AIPatrol::RandomMove(bool stop)
{
    if (m_TargetId)
        return;

    if (!m_EnableRandomMovement)
        return;

    if (--m_NextActionCountdown > 0)
        return;

    StopRandomMove();

    if (stop || Crown::SDRandom(100) > 95)
    {
        // 原地等待随机一段时间.
        m_NextActionCountdown = /*(Crown::SDRandom(2) + 1) * */g_ServerUpdateTimesPerSecond;
    }
    else
    {

        m_RandomMovementAI = CreateMovingAI();
        m_NextActionCountdown = 3 * g_ServerUpdateTimesPerSecond;
    }
}

void AIPatrol::OnRandomMovementEnd()
{
    m_RandomMovementAI = nullptr;
    m_NextActionCountdown = 0;

    RandomMove(true);
}

void AIPatrol::OnLostTarget()
{
    OnLostTargetV();
}

void AIPatrol::OnLostTargetV()
{
    //if (!ProgressiveSearch())
//    {
    OnAttackEnd();
//    if (m_pOwner->GetMonsterClass() != MONSTER_WORLD_BOSS)
//    {
//        m_pOwner->SetHp(m_pOwner->GetHealth());
//    }

    if (m_pOwner->GetObjectType() == OBJ_PET)
    {
        CreateMovingAI();
    }
    else if (m_pOwner->GetMonsterClass() != MONSTER_WORLD_BOSS)
    {
        CnNew AIMovingToPosition(m_pOwner, GetCentre(), boost::bind(&AIPatrol::OnAttackEnd, this));
    }
//    }
}

void AIPatrol::StopRandomMove()
{
    if (m_RandomMovementAI)
        m_RandomMovementAI->DeleteThis();
    m_RandomMovementAI = nullptr;
}

bool AIPatrol::ProgressiveSearch()
{
    // 搜索附近可攻击的敌人.

    //m_TargetId = 0;
    if (m_pOwner->GetMasterId() != 0)
        return true;

    const float ownerRadius = m_pOwner->GetRadius();
//    const float step = 50;
//    for (float searchRadius = ownerRadius + step; searchRadius <= m_SearchRadius; searchRadius += step)
//    {
        float searchRadius = m_SearchRadius;
        m_pOwner->World()->GetCoWorld().IterateInRoundness<Creature>(m_pOwner->GetPosition(), searchRadius,
            boost::bind(&AIPatrol::IterationCallback, this, _1));
        if (m_TargetId)
            return true;
//    }
    return false;
}

bool AIPatrol::ForceIdFilter(Creature& rCreature) const
{
//    if (m_pOwner->GetId() == rCreature.GetId())
//        return false;
  // 怪物永远可以打，人的forceid是1表示和平状态，forceid变成userid表示开战状态
//    if (m_pOwner->GetForceId()==0){               // 自己是怪物的情况
//        if (rCreature.IsInvincible()){             // 玩家是无敌
//            return false;
//        } else {
//            return true;
//        }
//    }
//    else {                                                            // 自己是玩家
//        if (m_pOwner->GetForceId()!=1 && rCreature.GetForceId()!=1 &&
//            m_pOwner->GetForceId()!=rCreature.GetForceId() && !rCreature.IsInvincible()){      // 两方都不是和平状态, 且对手不是无敌
//            return true;
//        } else {
//            return false;
//        }
//    }
    if (BattleSystem::CalcBattleRelation(m_pOwner, &rCreature) != BATTLE_RELATION_ENEMY)
        return false;
    return true;
  //return  (!pCreature.IsInvincible() && pCreature.GetForceId()==0) || (!m_pOwner->GetForceId() == 0) || (!pCreature.IsInvincible() && pCreature.GetForceId() != m_pOwner->GetForceId() && pCreature.GetForceId() != 1 && m_pOwner->GetForceId() != 1);
}

bool AIPatrol::TooFarAwayFromCentre(const Creature& obj) const
{
    return GetDistanceFromCentre(obj) > m_MaxFollowDistance;
}

Creature *AIPatrol::GetTarget()
{
    if (m_pOwner->World())
    {
        if (Creature* p = m_pOwner->World()->GetById<Creature>(m_TargetId))
        {
            if (!TooFarAwayFromCentre(*p) && !p->IsDead())
            return p;
        }
    }

    throw LostTarget();
}

CoVec2 AIPatrol::GetCentre() const
{
    return m_Centre;
}

CoVec2 AIPatrol::GetNextTargetPosition() const
{
    // 开方让结果在圆内均匀.
    float r = Crown::SDRandomFloat(1.0);
    CoVec2 offset;
    offset.SetLengthAndAngle(std::sqrt(r) * m_RandomMovementRadius, Crown::SDRandomFloat(M_PI * 2));
    return GetCentre() + offset;
}

AIMoving *AIPatrol::CreateMovingAI()
{
    return CnNew AIMovingToPosition(m_pOwner, GetNextTargetPosition(),
                                                          boost::bind(&AIPatrol::OnRandomMovementEnd, this));
}

IterationResult AIPatrol::IterationCallback(Creature& obj)
{
    if (m_TargetId)
        return STOP;

    if (m_pOwner == &obj)
        return CONTINUE;

//    if (!obj.AttackableBy(*m_pOwner))
//        return CONTINUE;

//    if (!m_pOwner->CanSee(obj))
//        return CONTINUE;

    if (BattleSystem::CalcBattleRelation(m_pOwner, &obj) == BATTLE_RELATION_ENEMY
            && m_pOwner->DistanceFrom(obj) <= m_SearchRadius)
    {
        m_TargetId = obj.GetId();

        //CnAssert(m_SubAIToken == 0);
        //CancelSubAI();
        m_AttackingAI = CnNew AIAttacking(
            m_pOwner,
            boost::bind(&AIPatrol::GetTarget, this),
            boost::bind(&AIPatrol::GetSkillId, this),
            boost::bind(&AIPatrol::SetSkillId, this, _1),
            boost::bind(&AIPatrol::OnLostTarget, this));
        if (m_2thSkillId)
            m_AttackingAI->Set2thSkillId(m_2thSkillId);

        StopRandomMove();

        return STOP;
    }

    return CONTINUE;
}
