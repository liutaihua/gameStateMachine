#pragma once

#ifndef AIATTACKING_H
#define AIATTACKING_H

#include "AI.h"
#include "WithTarget.h"

#include "ValueGetter.hpp"

class Creature;
class AIMovingToTarget;

class AIAttacking : public AI, public WithTarget
{
public:
	typedef std::function<void()> LostTargetCallback;
    typedef std::function<int32()> SkillIdGetter;
    typedef std::function<void(int32)> SkillIdSetter;

    AIAttacking(Creature* pOwner, uint32 targetId, int32 skillId,
                LostTargetCallback callback = nullptr);

#ifndef SWIG
    AIAttacking(Creature* pOwner, TargetGetter getter,
                SkillIdGetter skillIdGetter,
                SkillIdSetter skillIdSetter, LostTargetCallback callback = nullptr);
#endif

	void CreatureUpdated();

    void Set2thSkillId(int32 skillId)  { m_2thSkillId = skillId; }

private:
    void OnLostTarget();
    void OnClosedToTarget();
    void Init();

    AIMovingToTarget* m_AIMoving;
    std::function<int32()> m_SkillIdGetter;
    SkillIdSetter m_SkillIdSetter;
    int32 m_SkillId;
    int32 m_2thSkillId;
    LostTargetCallback m_LostTargetCallback;
};

#endif // AIATTACKING_H
