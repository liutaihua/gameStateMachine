#pragma once

#ifndef MONSTER_H_
#define MONSTER_H_

#include "Creature.h"
#include "GameItemManager.h"
#include "AI/AIBase.h"
#include "IterationUtils.h"
#include "sdtype.h"

const int32   DROP_PACK_NUM = 11;

struct MonsterDropInfo
{
    uint32 monterid;
    uint32 dropType;
    uint32 dropLevel;
    uint32 lifeBallMax;
    uint32 lifeBallMin;
    uint32 lifePro;
    uint32 attr10Max;
    uint32 attr10Min;
    uint32 attr10Pro;
    uint32 ZhanHunMax;
    uint32 ZhanHunMin;
    uint32 ZhanHunPro;
    uint32 ZhanHunNum;
    uint32 dropGoldMax;
    uint32 dropGoldMin;
    uint32 dropGoldNum;
    uint32 dropGoldPro;
    uint32 packId[DROP_PACK_NUM + 1];
    uint32 packIdPro[DROP_PACK_NUM + 1];

    MonsterDropInfo()
    {
        bzero(this, sizeof(MonsterDropInfo));
    }
};

struct DropItem
{
    uint32 itemid;
    uint32 itempro;
};

struct DropPackInfo
{
    uint32 packid;
    uint32 packtype;
   // uint32 itemid[25];
    std::vector<DropItem> items;

    DropPackInfo()
    {
        packid = 0;
        packtype = 0;
    }
};



typedef std::map<uint32, MonsterDropInfo> MonsterDropInfoMap;
typedef std::map<uint32, DropPackInfo> DropPackInfoMap;

struct SkillProb
{
    uint32 id;
    float prob;
};

class Monster: public Creature
{
#ifndef SWIG
	CnDeclareRTTI
#endif

	friend class GameWorld;
public:
    Monster(int monsterId);
	virtual ~Monster();

    virtual void SetBuffBaseSet(const BuffBaseSet& buffBaseSet);

    virtual bool MayCollidedWith(const CoObjectOwner &owner) const;

    bool IsNpc() const;

	void	OnRecv( uint32 MsgId, void* pData );
    virtual void OnNotify(uint32, void *);
    virtual void ComposeAppearanceNTF(GS_CLI_CREATURE_APPEAR_NTF *ntf) const;

	static const float ALERT_RANGE;

	static void ActiveAll();
	static void DeactiveAll();

    bool IsHpRestorationEnabled() const;
    void DisableHpRestoration();

    int32 GetMonsterId() const { return m_MonsterId; }

    bool    CanCastSkill(uint32 skillId);

    virtual bool OnDrop(Player* pPlayer);
    virtual bool OnDropTeam(uint64 teamId, CoVec2& pos);

    virtual GameItemManagementError PickGameItem(const std::string &itemId, const std::string &type,
                                                 uint32 templateId, uint32 level = 0, byte dropperClass = 0);
    virtual GameItemManagementError PickGameItem(Crown::shared_ptr<GameItem>& item);
    virtual void AddGameItem(const std::string& itemId, const std::string& type,
        uint32 templateId, uint32 count, uint32 = 0, byte = 0);

    void KeepActive();

    virtual bool AttackableBy(const Creature& obj) const { return !IsInvincible() && obj.GetForceId() != GetForceId(); }

    void SetMonsterClass(int32 MonsterClass) {m_MonsterClass = MonsterClass;}
    virtual int32 GetMonsterClass() const {return m_MonsterClass;}

    virtual bool HasSkill(int32 skillId) const { return true; }

    Crown::shared_ptr<GameItem> CreateGameItem(const std::string &itemId, const std::string &type, uint32 templateId);


    float GetAttackTime(uint32 num);
    void SetAttack1Time(float val);
    void SetAttack2Time(float val);

    void SetMonsterStrength(uint32 val) { m_MonsterStrength = val; }
    uint32 GetMonsterStrength() { return m_MonsterStrength; }

    void SetDizzyPro(uint32 val) { m_DizzyPro = val; }
    uint32 GetDizzyPro() { return m_DizzyPro; }

    void SetSkillInterval(uint32 val) { m_SkillInterval = val; }
    uint32 GetSkillInterval() { return m_SkillInterval; }
    void AddIntervalSkill(uint32 skillId, float prob);
    uint32 GetIntervalSkill();
    void DelIntervalSkill(uint32 skillId);

    void AddPuppetId(uint32 id);
    uint32 GetPuppetId();
    void    SetBornPoint(CoVec2 position) { m_BornPosition = position; }
    CoVec2  GetBornPoint() { return m_BornPosition; }

    uint64   GetParam() { return m_Param; }
    void    SetParam(uint64 val) { m_Param = val; }

    static  bool SetMonsterDropInfo(uint32 monsterid, MonsterDropInfo& info);
    static  MonsterDropInfo* GetMonsterDropInfo(uint32 monsterid);
    static  bool SetPackDropInfo(uint32 id, DropPackInfo& info);
    static  DropPackInfo* GetPackDropInfo(uint32 id);

    uint32 GetAIId() const;
    void SetAIId(uint32 aiId);
    void SetAI(AIBase* pAI);
    AIBase* GetAI();

    virtual void UnderAttack(Creature* pAttacker);

    bool IsPatrolEnabled() const;
    void EnablePatrol();
    void DisablePatrol();

    bool GetHpRestore() const { return m_HpRestore; }
    void SetHpRestore() { m_HpRestore = true; }

    void DropPackWithId(int packId, Player* pPlayer);

    ObjectType	GetObjectType() const
    {
        if (GetCareerId() == CAREER_WARRIOR || GetCareerId() == CAREER_WIZARD || GetCareerId() == CAREER_PRIEST)
            return OBJ_PLAYER;
        else
            return ObjectWithId::GetObjectType();
    }
protected:
	virtual	void RecieveEvent(uint32 frameCount, ulong type, ulong arg2);

	virtual void Update(unsigned int frameCount);

	void	OnMoveReq(int MoveState, float PosX, float PosY, float Angle);

	virtual void OnDied(uint32 killerId);

    virtual void DropSinglePlayer(Player* player);

private:
    IterationResult NotifyFriendNearbyCallback(Creature& obj, Creature* pAttacker);

	// Whether to attack incoming target.
	static bool m_StopAll;
    int32   m_MonsterId;
    uint32  m_ActiveCountdown;
    int32   m_MonsterClass;
    float   m_Attack1Time;
    float   m_Attack2Time;
    uint32  m_MonsterStrength;
    CoVec2  m_lastEnablePosition;
    uint32  m_DizzyPro;

    bool    m_bHasExplode;

    std::vector<uint32>  m_PuppetIds;

    uint32 m_AIId;
    uint64       m_Param;
    bool m_HpRestore;

    uint32  m_SkillInterval;
    std::vector<SkillProb> m_IntervalSkills;

    static MonsterDropInfoMap m_MonsterDropInfoMap;
    static DropPackInfoMap m_DropPackInfoMap;

    AIBase* m_pAI;

    uint32  m_LastCreatePuppetTime;
    bool m_HpRestorationEnabled;

    bool m_PatrolEnabled;
    int m_PatrolCountdown;  // after this number of updates, disable patrol
    CoVec2 m_BornPosition;  // 出生点
    uint32 m_UpdateFrameCount;
 public:
    uint32 m_WorldID;
};

#endif	// MONSTER_H_
