#include "GameHolderPCH.h"
#include "GameWorld.h"
#include "SkillAttack.h"
#include "Monster.h"
#include "Equipment.h"
#include "GameItemFactory.h"
#include "EventDispatcher.h"
#include "EventConstants.h"
#include "Player.h"
#include "Pet.h"
#include "puppet.h"
#include "BattleSystem.h"
#include "BuffManager.h"


CnImplementRTTI(Monster, Creature);

using namespace std;

const float Monster::ALERT_RANGE = 800.0;

bool Monster::m_StopAll = false;

MonsterDropInfoMap Monster::m_MonsterDropInfoMap;
DropPackInfoMap Monster::m_DropPackInfoMap;

Monster::Monster(int32 monsterId) :
    Creature(OBJ_MONSTER),
    m_MonsterId(monsterId),
    m_ActiveCountdown(5 * g_ServerUpdateTimesPerSecond), // 一出来活动30秒,比如搜索周围玩家.
    m_MonsterClass(MONSTER_NORMAL),
    m_AIId(0),
    m_pAI(nullptr),
    m_bHasExplode(false),
    m_LastCreatePuppetTime(0),
    m_HpRestorationEnabled(true),   // <== deprecated by m_HpRestore
    m_PatrolEnabled(false),
    m_PatrolCountdown(-1),
    m_UpdateFrameCount(0),
    m_lastEnablePosition(0, 0),
    m_HpRestore(false)
{
    SetForceId(0);
    SetMovementSpeed(250);

    char buf[10];
    Crown::SDItoa(buf, m_MonsterId);
    buf[9] = '\0';
    SetName(buf);

    // 怪物必备技能
    //AddSkill(SKILL_ATTACK);
    //AddSkill(SKILL_TOXOPHILY);

    m_GameItemManager.SetPackageVisibleToOthers(true); // 允许其他人观察怪物背包.
}

Monster::~Monster()
{
    if (m_pAI)
    {
        CnDelete m_pAI;
    }

  GameWorld *wd=g_game_holder->GetWorld(m_WorldID);
  if (wd!=NULL)
    wd->DestroyObjMap(this);
    //所有智能生物ai相关的都应该调用
    //SigDied(0);
}

bool Monster::IsHpRestorationEnabled() const
{
    return m_HpRestorationEnabled;
}

void Monster::DisableHpRestoration()
{
    m_HpRestorationEnabled = false;
}

uint32 Monster::GetAIId() const
{
    return m_AIId;
}

void Monster::SetAIId(uint32 aiId)
{
    m_AIId = aiId;
}

void Monster::SetAI(AIBase* pAI)
{
    m_pAI = pAI;
}

AIBase* Monster::GetAI()
{
    return m_pAI;
}

void Monster::UnderAttack(Creature *pAttacker)
{
    Creature::UnderAttack(pAttacker);
    if (!IsDead())
    {
        if (!m_bHasExplode)
        {
            m_bHasExplode = true;
            Puppet::MakePuppetEvent(PUPPET_CON_BE_ATTACK, GetPuppetId(), this,
                                    GetPosX(), GetPosY());
        }
        else
        {
            PuppetInfo* info = Puppet::GetPuppetInfo(GetPuppetId());
            if (info && info->PuppetFollow == 5)
            {
                if (m_JingDianCD < 0)
                {
                    Puppet::MakePuppetEvent(PUPPET_CON_BE_ATTACK, GetPuppetId(), this,
                                            GetPosX(), GetPosY());

                    m_JingDianCD = 2000;
                }

            }
        }

    }

//    if (GameHolder::IsOwnerEnabled())
//    {
//        auto ownerId = GetOwnerId();
//        if (ownerId != 0)
//        {
//            if (!World()->IsCreatureAlive(ownerId))
//                ownerId = 0;
//        }
//        if (ownerId == 0)
//        {
//            // this is the first attacker
//            auto attackerId = pAttacker->GetId();
//            if (pAttacker->GetMasterId() != 0)
//                attackerId = pAttacker->GetMasterId();
//            if (World()->IsCreatureAlive(attackerId))
//                ownerId = attackerId;
//        }
//        SetOwnerId(ownerId);
//    }

    if (m_pAI)
    {
        m_pAI->UnderAttack(pAttacker);

        // notify the friends nearby to revenge
        World()->GetCoWorld().IterateInRoundness<Creature>(
            GetPosition(), 500,
            boost::bind(&Monster::NotifyFriendNearbyCallback, this, _1, pAttacker));
    }
}

IterationResult Monster::NotifyFriendNearbyCallback(Creature& obj, Creature* pAttacker)
{
    if (&obj == this)
        return CONTINUE;

    if (BattleSystem::CalcBattleRelation(&obj, pAttacker) != BATTLE_RELATION_ENEMY)
        return CONTINUE;

    Monster* pMonster = CnDynamicCast(Monster, &obj);
    if (!pMonster)
        return CONTINUE;

    AIBase* pAI = pMonster->GetAI();
    if (!pAI)
        return CONTINUE;

    pAI->FriendUnderAttack(pAttacker, this);

    return CONTINUE;
}

void Monster::SetBuffBaseSet(const BuffBaseSet& buffBaseSet)
{
    m_BuffSet.SetBuffBaseSet(buffBaseSet);
}

void Monster::OnRecv( uint32 /*MsgId*/, void* /*pData*/ )
{

}

bool Monster::IsNpc() const
{
    return GetTalkable() && IsInvincible();
}

void Monster::OnNotify(uint32 msgId, void* pData)
{
    if (IsDestroyed())
        return;
    if (m_ActiveCountdown)
        return;

    switch(msgId)
    {
    case MSG_MOVE_NTF:
    {
        GS_CLI_MOVE_NTF* ntf = static_cast<GS_CLI_MOVE_NTF*>(pData);
        Creature* p = World()->GetById<Creature>(ntf->Id);
        if (p && p->WakeUpNearbyMonster()) KeepActive();
        break;
    }
    case MSG_CREATURE_APPEAR_NTF:
    {
        GS_CLI_CREATURE_APPEAR_NTF* ntf = static_cast<GS_CLI_CREATURE_APPEAR_NTF*>(pData);
        Creature* p = World()->GetById<Creature>(ntf->Id);
        if (p && p->WakeUpNearbyMonster()) KeepActive();
        break;
    }
    case MSG_PROPERTYCHANGED_INT_NTF:
    {
        GS_CLI_PROPERTYCHANGED_INT_NTF* ntf = static_cast<GS_CLI_PROPERTYCHANGED_INT_NTF*>(pData);
        Creature* p = World()->GetById<Creature>(ntf->Id);
        if (p && p->WakeUpNearbyMonster()) KeepActive();
        break;
    }
    case MSG_PROPERTYCHANGED_UINT_NTF:
    {
        GS_CLI_PROPERTYCHANGED_UINT_NTF* ntf = static_cast<GS_CLI_PROPERTYCHANGED_UINT_NTF*>(pData);
        Creature* p = World()->GetById<Creature>(ntf->Id);
        if (p && p->WakeUpNearbyMonster()) KeepActive();
        break;
    }
    case MSG_PROPERTYCHANGED_FLOAT_NTF:
    {
        GS_CLI_PROPERTYCHANGED_FLOAT_NTF* ntf = static_cast<GS_CLI_PROPERTYCHANGED_FLOAT_NTF*>(pData);
        Creature* p = World()->GetById<Creature>(ntf->Id);
        if (p && p->WakeUpNearbyMonster()) KeepActive();
        break;
    }
    }
}

void Monster::ComposeAppearanceNTF(GS_CLI_CREATURE_APPEAR_NTF *ntf) const
{
    //ntf->UserId = GetMonsterId();
    //snprintf(ntf->UserId, sizeof(ntf->UserId), "%I64d", GetMonsterId());
    strncpy(ntf->UserId, Crown::ToString(GetMonsterId()).c_str(), sizeof(ntf->UserId));
}

void Monster::Update(unsigned int frameCount)
{
    if (m_pAI)
    {
        m_pAI->Update(frameCount);
    }
    if (g_game_holder->CmdVM["ruler"].as<std::string>() == "abyss"  && Crown::SDRandom(10) > 7) {
        return;
    }
    if (g_game_holder->CmdVM["ruler"].as<std::string>() == "battleland" && frameCount % 20 == 0) {
        if (GetBuffManager()) {
            auto pbuff = GetBuffManager()->FindBuff(18);
            if (pbuff) {
                GetBuffManager()->SendNetUpdateBuff(pbuff);
            }
        }
    }


    /*

    }*/
    if (m_PatrolCountdown > 0)
    {
        if (--m_PatrolCountdown <= 0)
        {
            //DisablePatrol();
        }
    }

    m_LastCreatePuppetTime += g_update_time;
    if (m_LastCreatePuppetTime > 250)
    {
        for (auto it = m_PuppetIds.begin(); it != m_PuppetIds.end(); it++)
        {
            Puppet::MakePuppetEvent(PUPPET_CON_ALL_CREATE, *it, this, GetPosX(), GetPosY());
        }

        m_LastCreatePuppetTime = 0;
    }

    if (!m_ActiveCountdown)
        return;

	Creature::Update(frameCount);

    m_ActiveCountdown--;
    m_UpdateFrameCount = frameCount;
}

void Monster::ActiveAll()
{
	m_StopAll = false;
}

void Monster::DeactiveAll()
{
    m_StopAll = true;
}

bool Monster::CanCastSkill(uint32 skillId)
{
    SkillInfo* info = SkillManager::GetSkillInfo(skillId);
    if(!info)
        return false;

    float needMana = GetCastSkillNeedMana(info);
    if (GetMp() < needMana)
        return false;

    if (GetCooldownLeft(skillId) > 0)
        return false;

    return true;
}

bool Monster::OnDrop(Player *pPlayer)
{
    if (GetK() != 1)
        return false;

    //if (GameHolder::IsTeamDrop())
    //{
    //    if (GetOwnerForceId() == 0)
    //        return false;
    //}

    auto players = World()->GetPlayers();
    for (auto it = players.begin(); it != players.end(); ++it)
    {
        auto* player = it->second;
        if (GameHolder::IsTeamDrop())
        {
            if (player->GetForceId() != pPlayer->GetForceId()) continue;
        }

        DropSinglePlayer(player);
    }

     return true;
}

bool Monster::OnDropTeam(uint64 teamId, CoVec2& pos)
{
    return false;
}

GameItemManagementError Monster::PickGameItem(const std::string &itemId, const std::string &type, uint32 templateId,
                                              uint32 level, byte dropperClass)
{
    return NO_ROOM;
}

GameItemManagementError Monster::PickGameItem(Crown::shared_ptr<GameItem>& item)
{
    return NO_ROOM;
}

Crown::shared_ptr<GameItem> Monster::CreateGameItem(const std::string &itemId, const std::string &type,
                             uint32 templateId)
{
    Crown::shared_ptr<GameItem> gi(GameItemFactory::CreateGameItem(type, itemId));
    gi->m_TemplateId = templateId;
    if (Equipment* eq = dynamic_cast<Equipment*>(gi.get()))
    {
        eq->m_DropperLevel = GetLevel();
        eq->m_DropperClass = GetMonsterClass();
    }

    return gi;
}

float Monster::GetAttackTime(uint32 num)
{
    if (num == 1)
        return m_Attack1Time;
    else if (num == 2)
        return m_Attack2Time;
    else
        return 1;
}

void Monster::SetAttack1Time(float val)
{
    m_Attack1Time = val;
}

void Monster::SetAttack2Time(float val)
{
    m_Attack2Time = val;
}

void Monster::AddIntervalSkill(uint32 skillId, float prob)
{
    if (skillId == 0)
        return;

    SkillProb sp = {skillId, prob};
    m_IntervalSkills.push_back(sp);
}

uint32 Monster::GetIntervalSkill()
{
    CnAssert(!m_IntervalSkills.empty());

    auto prob = Crown::SDRandomFloat(0.99);
    auto sum = 0.0;
    for (auto it = m_IntervalSkills.begin(); it != m_IntervalSkills.end(); ++it)
    {
        sum += (*it).prob;
        if (prob < sum) return (*it).id;
    }
    CnAssert(false);
    return m_IntervalSkills[m_IntervalSkills.size() - 1].id;
}

void Monster::DelIntervalSkill(uint32 skillId)
{
    auto sum = 1.0;
    for (auto it = m_IntervalSkills.begin();
         it != m_IntervalSkills.end(); ++it)
    {
        if ((*it).id == skillId)
        {
            sum -= (*it).prob;
            m_IntervalSkills.erase(it);
            break;
        }
    }
    // justify the left skills' prob
    for (auto it = m_IntervalSkills.begin();
         it != m_IntervalSkills.end(); ++it)
    {
        (*it).prob /= sum;
    }
}

void Monster::AddPuppetId(uint32 id)
{
    if (id != 0)
    {
       m_PuppetIds.push_back(id);
    }
}

uint32 Monster::GetPuppetId()
{
    if (m_PuppetIds.empty())
        return 0;
    return m_PuppetIds[Crown::SDRandom(m_PuppetIds.size())];
}

bool Monster::SetMonsterDropInfo(uint32 monsterid, MonsterDropInfo& info)
{
    m_MonsterDropInfoMap[monsterid] = info;
    return true;
}

MonsterDropInfo *Monster::GetMonsterDropInfo(uint32 monsterid)
{
    MonsterDropInfoMap::iterator it = m_MonsterDropInfoMap.find(monsterid);
    if (it != m_MonsterDropInfoMap.end())
        return &it->second;

    return NULL;
}

bool Monster::SetPackDropInfo(uint32 id, DropPackInfo &info)
{
    m_DropPackInfoMap[id] = info;
    return true;
}

DropPackInfo *Monster::GetPackDropInfo(uint32 id)
{
    DropPackInfoMap::iterator it = m_DropPackInfoMap.find(id);
    if (it != m_DropPackInfoMap.end())
        return &it->second;

    return NULL;
}

//怪物掉落方法
void Monster::AddGameItem(const std::string &itemId, const std::string &type,
    uint32 templateId, uint32 count, uint32, byte)
{
    uint32 countLeft = count;
    if (Creature* pKiller = GetKiller())
    {
        for (uint32 i = 0; i < count; i++)
        {
            Crown::shared_ptr<GameItem> gi = CreateGameItem(itemId, type, templateId);
            if (GameItemManagementError e = pKiller->PickGameItem(gi))
                break;
            countLeft--;
        }
    }

    if (!countLeft) return;

    Crown::shared_ptr<GameItem> gi = CreateGameItem(itemId, type, templateId);
    gi->m_Count = countLeft;

    m_GameItemManager.AddGameItem(gi, BAG_AUTO);
}

void Monster::KeepActive()
{
    const uint32 activeCountdown = 20 * g_ServerUpdateTimesPerSecond;
    m_ActiveCountdown = activeCountdown;
}

bool Monster::MayCollidedWith(const CoObjectOwner &owner) const
{
    if (IsDead())
        return false;

    const CoObjectOwner* pOwner = &owner;
    if (dynamic_cast<const Puppet*>(pOwner))
    {
        const Puppet* pPuppet = dynamic_cast<const Puppet*>(pOwner);
        // 地刺专用旗子
        if (pPuppet->GetPuppetId() == 38)
        {
            return false;
        }
        return true;
    }
    return true;
}
//怪物死亡销毁对象再 次地图中异步进行
void Monster::OnDied(uint32 killerId)
{
    Puppet::MakePuppetEvent(PUPPET_CON_BE_KILLED, GetPuppetId(), this, GetPosX(), GetPosY());

    if (m_pAI)
    {
        CnDelete m_pAI;
        m_pAI = nullptr;
    }

    SetKillerId(killerId);

	// n秒后销毁
    World()->GetEventDispatcher()->Create(6 * g_ServerUpdateTimesPerSecond, GetId(), GetId(), DESTORY_SELF, 0);

	Creature::OnDied(killerId);

//    auto coobj = GetCoObject();
//    coobj->LeaveCell();

//    Creature*   pKiller = World()->GetById<Creature>(killerId);
//    if (pKiller && pKiller->GetObjectType() == OBJ_PET)
//    {
//        Pet* pet = dynamic_cast<Pet*>(pKiller);
//        if (pet)
//        {
//            Player* player = World()->GetById<Player>(pet->GetMasterId());
//            if (player)
//            {
//                //OnDrop(player);
//                //SigDied(pet->GetMasterId());

//            }
//        }
    //    }
}

void Monster::DropSinglePlayer(Player *pPlayer)
{
    MonsterDropInfo* info = Monster::GetMonsterDropInfo(GetMonsterId());
    if (!info)
    {
        //CnAssert(false);
        return;
    }
    float abyss_factor = 1.0f;
    if (IsBumper()){
        if (pPlayer->IsAbyssTired()){
            abyss_factor = 0.0f;
        }
    }

    int32 addLifePro = 0;
    BuffBase* pBuff = pPlayer->GetBuffManager()->FindBuffByType(BUFF_EFFECT_ADD_DROP_LIFEBOX);
    if (pBuff)
    {
        addLifePro += pBuff->GetBuffVal();
    }
    if (Crown::SDRandom(10000) < info->lifePro + addLifePro)
    {
        uint32 val = Crown::SDRandom(info->lifeBallMax - info->lifeBallMin + 1) + info->lifeBallMin;
        World()->CreateItemObj(GetPosX(), GetPosY(), val, LIFEBALL, pPlayer->GetId(), GetId());
    }

    if (Crown::SDRandom(10000) < (info->attr10Pro * pPlayer->SimpleProperties.get("income_factor", 1.0) * abyss_factor))
    {
        uint32 val = Crown::SDRandom(info->attr10Max - info->attr10Min + 1) + info->attr10Min;
        World()->CreateItemObj(GetPosX(), GetPosY(), val, POWERBALL, pPlayer->GetId(), GetId());
    }

    for (uint32 i = 0; i < info->ZhanHunNum; i++)
    {
        if (Crown::SDRandom(10000) < (info->ZhanHunPro * pPlayer->SimpleProperties.get("income_factor", 1.0) * abyss_factor))
        {

            uint32 val = Crown::SDRandom(info->ZhanHunMax - info->ZhanHunMin + 1) + info->ZhanHunMin;
            World()->CreateItemObj(GetPosX(), GetPosY(), val, ZHANHUN, pPlayer->GetId(), GetId());
        }
    }

     for (uint32 i = 0; i < info->dropGoldNum; i++)
     {
          if (Crown::SDRandom(10000) < info->dropGoldPro * pPlayer->SimpleProperties.get("income_factor", 1.0) * abyss_factor)
          {
              int32 money = Crown::SDRandom(info->dropGoldMax - info->dropGoldMin + 1) + info->dropGoldMin;
              // #2966
              if (pPlayer->GetSimpleProperty("vip5_factor", 0.0f) == 2){
                  money = money * 2;
              }
              World()->CreateItemObj(GetPosX(), GetPosY(), money, MONEY, pPlayer->GetId(), GetId());
          }


     }

     for (int i = 1; i <= DROP_PACK_NUM; i++)
     {
         // this special buff can got a extra drop chance
         if (Crown::SDRandom(10000) < (info->packIdPro[i] * pPlayer->SimpleProperties.get("income_factor", 1.0) * abyss_factor))
         {
             DropPackWithId(info->packId[i], pPlayer);
         }
     }
}

void Monster::DropPackWithId(int packId, Player* pPlayer) {
    DropPackInfo* packinfo = Monster::GetPackDropInfo(packId);
    if (packinfo)
    {
        if (packinfo->items.empty())
            return;
        if (packinfo->packtype == EQUIPMENT)
        {
            std::vector<DropItem> equipments;
            uint32 pro_max = 0;
            for (uint32 index = 0; index < packinfo->items.size(); index++)
            {
                if (packinfo->items[index].itemid / 10000000 == pPlayer->GetCareerId())
                {
                   equipments.push_back(packinfo->items[index]);
                   pro_max += packinfo->items[index].itempro;
                }
            }

            if (equipments.empty() || pro_max == 0)
                return;

            uint32 r = Crown::SDRandom(pro_max);
            uint32 pro_sum = 0;
            for (int32 index = 0; index < equipments.size(); index++)
            {
                pro_sum += equipments[index].itempro;
                if (r < pro_sum)
                {
                    if (packinfo->items[index].itemid != 0)
                        World()->CreateItemObj(GetPosX(), GetPosY(),
                                               equipments[index].itemid, packinfo->packtype,
                                               pPlayer->GetId(), GetId());
                    break;
                }
            }

        }
        else
        {
            uint32 pro_max = 0;
            uint32 pro_sum = 0;
            for (int32 index = 0; index < packinfo->items.size(); index++)
            {
                pro_max += packinfo->items[index].itempro;
            }

            if (pro_max == 0)
                return;

            uint32 r = Crown::SDRandom(pro_max);
            for (int32 index = 0; index < packinfo->items.size(); index++)
            {
                pro_sum += packinfo->items[index].itempro;
                if (r < pro_sum)
                {
                    if (packinfo->items[index].itemid != 0)
                        World()->CreateItemObj(GetPosX(), GetPosY(),
                                               packinfo->items[index].itemid,
                                               packinfo->packtype, pPlayer->GetId(), GetId());
                    break;
                }
            }
        }
    }
}


void Monster::RecieveEvent( uint32 frameCount, ulong type, ulong arg2 )
{
	Creature::RecieveEvent(frameCount, type, arg2);

	switch(type)
	{
	case DESTORY_SELF:
		{
            Destroy();
		}
		break;
    case DIE_SELF:
        {
            Die(GetId());
        }
        break;
	}
}

bool Monster::IsPatrolEnabled() const
{
    if (GameHolder::IsPatrolOptMode())
        return m_PatrolEnabled;
    else return true;
}
void Monster::EnablePatrol()
{
    m_PatrolEnabled = true;
    m_PatrolCountdown = 2 * g_ServerUpdateTimesPerSecond; // 2 seconds
    SetCollisionEnabled(true);
}
void Monster::DisablePatrol()
{
    m_PatrolEnabled = false;
}

