#pragma once

#ifndef AIPATROL_H
#define AIPATROL_H

#include "AI.h"
#include "AIMoving.h"
#include "Monster.h"

class Creature;
class SkillBase;
class AIAttacking;
class AIMoving;
struct CoVec2;

class AIPatrol : public AI
{
public:
    AIPatrol(Creature* pOwner, float followDist = Monster::ALERT_RANGE/* * 2.0f */);
    ~AIPatrol();

    void SetFollowDistance(float dist) { m_MaxFollowDistance = dist; }
    void SetSearchRadius(float radius) { m_SearchRadius = radius; }
    float GetRandomMovementRadius() const { return m_RandomMovementRadius; }
    void SetRandomMovementRadius(float radius) { m_RandomMovementRadius = radius; }
    void SetTargetId(uint32 id);
    void SetSkillId(int32 id);
    void Set2thSkillId(int32 id);
    void RandomTarget();
    void EnableRandomMovement() { m_EnableRandomMovement = true; }
    void DisableRandomMovement();

    uint32 GetTargetId() const { return m_TargetId; }
    float GetDistanceFromCentre(const Creature &obj) const;

    bool TooFarAwayFromCentre(const Creature &obj) const;
    virtual CoVec2 GetCentre() const;

    // GCC 4.4 does not allow boost::bind a protected method of base class in sub class.
    // But Clang does.
    void OnRandomMovementEnd();

protected:
    virtual CoVec2 GetNextTargetPosition() const;
    virtual AIMoving* CreateMovingAI();
    void StopRandomMove();

    virtual void OnLostTargetV();

private:
    void Init(Creature* pOwner);
    void OnOtherRepositioned(Creature* obj);
    void OnHpChanged(float amount, SkillBase* pSkill);
    void OnUpdated();
    void OnAttackEnd();
    void RandomMove(bool stop = false);
    void OnLostTarget();
    bool ProgressiveSearch();
    bool ForceIdFilter(Creature &pCreature) const;
    Creature* GetTarget();
    int32 GetSkillId() const { return m_SkillId; }

    IterationResult IterationCallback(Creature &obj);

protected:
    int32 m_NextActionCountdown;  // 站立的倒计时.
    AIMoving* m_RandomMovementAI;
    AIAttacking* m_AttackingAI;

public:
    CoVec2 m_Centre;
    float m_MaxFollowDistance;
    float m_SearchRadius;
    int32 m_SkillId;
    int32 m_2thSkillId;
    bool m_EnableRandomMovement;
    float m_RandomMovementRadius;
    uint32 m_TargetId;
};

#endif // AIPATROL_H
