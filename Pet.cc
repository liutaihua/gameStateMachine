#include "Pet.h"

#include "Creature.h"
#include "AI/AIPet.h"
//#include "AI/AITargetedBase.h"

//CnImplementRTTI(Pet, Creature);



Pet::Pet(uint32 pettypeid) :
    Creature(OBJ_PET, "PET"),
    m_PetTypeId(pettypeid),
//    m_RebornEnabled(true),
    m_MultiDamage(1.0f)
{
//    AddSkill(SKILL_ATTACK);
//    AddSkill(SKILL_TOXOPHILY);
//    SetRadius(0);
//    SetMasterId(masterId);
}

Pet::~Pet()
{
//    //所有智能生物ai相关的都应该调用
//    SigDied(0);
//    //管理器里删除再监测次
//    GameWorld *wd=g_game_holder->GetWorld(m_WorldID);
//    if (wd!=NULL)
//      wd->DestroyObjMap(this);
}


void Pet::Update(uint32 frameCount)
{

//    try {
//        Creature::Update(frameCount);
//    }
//    catch(std::exception& e)
//    {
//        LOG_E <<"pet Update error:"<< e.what() << std::endl;
//    }


//    if (GameHolder::IsSafeScene()) return;
//    if (frameCount % 20 != 0) return;

}


