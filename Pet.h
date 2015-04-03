#ifndef PET_H
#define PET_H
#include "Creature.h"


//class Creature;

class Pet : public Creature

{
public:
    Pet(uint32 pettypeid);
    virtual ~Pet();


public:
    //uint32 GetMasterId() const { return m_MasterId; }
    virtual uint32 GetPetTypeId() const { return m_PetTypeId; }
    //virtual uint32 GetPetId() const { return m_PetId; }
    virtual uint32 GetPetStatus() const { return m_PetStatus; }

//    virtual void OnDied(uint32 killerId);

//    void SetBuffBaseSet(const BuffBaseSet& buffBaseSet);

//    virtual void NotifyAppearance(WorldObject* pCreature = nullptr);

//    virtual	void RecieveEvent(uint32 frameCount, ulong type, ulong arg2);

//    virtual bool MayCollidedWith(const CoObjectOwner &) const;
//    virtual bool AttackableBy(const Creature& obj) const { return false; }
//    void DisableReborn();

    void Update(uint32 frameCount);

//    void OnNotify(uint32 MsgId, void* pData);

//    float GetPetMultiDamage() { return m_MultiDamage; }
//    void  SetPetMultiDamage(float val) { m_MultiDamage = val; }

    //覆盖掉基类类型
//    virtual int32 GetMonsterClass() const {return MONSTER_NO;}

protected:
    uint32 m_PetTypeId;
    uint32 m_PetStatus;

    float   m_MultiDamage;

 public:
     uint32   m_WorldID;
};



#endif // PET_H

