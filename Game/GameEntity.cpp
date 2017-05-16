#include "stdafx.h"
#include "GameEntity.h"
#include "Model.h"
#include "XmlTypes.h"
#include "FileSystem.h"
#include "GraphicDefines.h"

#include <CBXml/Serialize.h>
#include <CBXml/Node.h>
#include <CBXml/NodeList.h>
#include <CBXml/Attribute.h>
#include <CBXml/AttributeList.h>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

static const cb::string XML_ENTITYTYPES_ROOTNAME = L"EntityTypes";
static const float PROJECTILE_LENGTH = 3.0f;
static const float MAX_ENTITY_ROTATION = 70.0f;

CProjectile::CProjectile(const glm::vec2 & pos,
                         const glm::vec2 & dir,
                         const glm::vec4 & color,
                         const float speed,
                         const float damage)
  : mPos(pos)
  , mDir(dir)
  , mColor(color)
  , mSpeed(speed)
  , mDamage(damage)
  , mDeleted(false) {}

const glm::vec2 & CProjectile::GetPos() const {
  return mPos;
}

const glm::vec2 & CProjectile::GetDir() const {
  return mDir;
}

const float CProjectile::GetSpeed() const {
  return mSpeed;
}

const float CProjectile::GetDamage() const {
  return mDamage;
}

const float CProjectile::GetLength() const {
  return PROJECTILE_LENGTH;
}

const bool CProjectile::IsDeleted() const {
  return mDeleted;
}

void CProjectile::Update(const glm::vec2& playerVec, const float timeDelta) {
  if(mDeleted)
    return;

  mPos += ((mDir * mSpeed) - playerVec) * timeDelta;
}

void CProjectile::UpdateRender(CProjectileVertex & outVertex) {
  if(mDeleted)
    return;

  glm::vec2 endPos = mDir * PROJECTILE_LENGTH + mPos;
  outVertex.StartPos = glm::vec3(mPos.x, 0.0f, mPos.y);
  outVertex.EndPos = glm::vec3(endPos.x, 0.0f, endPos.y);

  outVertex.StartColor = mColor;
  outVertex.EndColor = mColor;
}

void CProjectile::Delete() {
  mDeleted = true;
}

CGameEntityType::CGameEntityType()
  : Name(L"")
  , Type(EntityType::ET_NONE)
  , Speed(0.0f)
  , Color(1.0f)
  , ModelFile()
  , MaxHealth(1.0f)
  , Damage(0.0f)
  , AIPause(0.5f)
  , RotSpeed(20.0f)
  , IgnoreProjectiles(false) 
  , Points(0)
{
  EventMap[EntityEvent::EE_ONDEATH] = {EntityEventAction::EEA_NONE};
  EventMap[EntityEvent::EE_ONENTITYCOLLISION] = {EntityEventAction::EEA_NONE};
  EventMap[EntityEvent::EE_ONPROJECTILECOLLISION] = {EntityEventAction::EEA_NONE};
}

const bool CGameEntityType::Save(const TypeMapT & typeMap, IFileSystem & fs, const cb::string & filepath) {
  return fs.WriteXml(filepath, XML_ENTITYTYPES_ROOTNAME, typeMap);
}

const bool CGameEntityType::Load(TypeMapT & typeMap, IFileSystem & fs, const cb::string & filepath) {
  return fs.ReadXml(filepath, XML_ENTITYTYPES_ROOTNAME, typeMap);
}

CGameEntity::CGameEntity(const CGameEntityType & type,
                         CModelRepository& modelRepo,
                         const glm::vec2& pos,
                         const float rotAngle)
  : mName(type.Name)
  , mType(type.Type)
  , mPos(pos)
  , mSpeed(type.Speed)
  , mColor(type.Color)
  , mModel(nullptr)
  , mMaxHealth(type.MaxHealth)
  , mHealth(type.MaxHealth)
  , mDamage(type.Damage)
  , mAIPause(type.AIPause)
  , mAITime(0.0f)
  , mRotAngle(rotAngle)
  , mRotSpeed(type.RotSpeed)
  , mCollRadius(0.7f)
  , mIgnoreProjectiles(type.IgnoreProjectiles)
  , mDeleted(false) 
  , mPoints(type.Points)
  , mEventMap(type.EventMap)
{
  mModel = modelRepo.GetModel(type.ModelFile);
}

void CGameEntity::ModHealth(const float value) {
  mHealth = glm::clamp(mHealth + value, 0.0f, mMaxHealth);
}

const float CGameEntity::GetValue() const {
  return 0.0f;
}

const float CGameEntity::GetHealth() const {
  return mHealth;
}

const float CGameEntity::GetDamage() const {
  return mDamage;
}

const float CGameEntity::GetCollRadius() const {
  return mCollRadius;
}

const EntityType CGameEntity::GetType() const {
  return mType;
}

const glm::vec2 & CGameEntity::GetPos() const {
  return mPos;
}

const glm::vec2 CGameEntity::GetDir() const {
  return glm::rotate(glm::vec2(0.0f, 1.0f), glm::radians(mRotAngle));
}

const glm::vec2 CGameEntity::GetVec() const {
  return GetDir() * mSpeed;
}

const bool CGameEntity::GetIgnoreProjectiles() const {
  return mIgnoreProjectiles;
}

const bool CGameEntity::IsDeleted() const {
  return mDeleted;
}

const Uint32 CGameEntity::GetPoints() const {
  return mPoints;
}

const EntityEventMapT & CGameEntity::GetEventMap() const {
  return mEventMap;
}

void CGameEntity::Update(const glm::vec2& playerVec,
                         const float timeDelta) {
  if(mDeleted)
    return;

  glm::vec2 racerPos(0.0f);
  mPos += (GetVec() - playerVec) * timeDelta;

  switch(mType) {
  default:
  case EntityType::ET_NONE:
    return;
  case EntityType::ET_ITEM:
  case EntityType::ET_OBSTACLE:
    mRotAngle += timeDelta * mRotSpeed;
    break;

  case EntityType::ET_ENEMY:
    {
      mAITime += timeDelta;
      if(mAITime > mAIPause) {
        glm::vec2 toRacerVec = glm::normalize(racerPos - mPos);
        mRotAngle = glm::degrees(glm::orientedAngle(toRacerVec, glm::vec2(0.0f, 1.0f)));
        mRotAngle = glm::clamp(mRotAngle, 
                               -MAX_ENTITY_ROTATION, 
                               MAX_ENTITY_ROTATION);

        mAITime -= mAIPause;
      }
    }
    break;
  }
}

void CGameEntity::Render(const glm::mat4 & transform) const {
  if(mDeleted)
    return;

  glm::mat4 mat = transform *
    glm::translate(glm::vec3(mPos.x, 3.0f, mPos.y)) *
    glm::rotate(glm::radians(mRotAngle), gAxis3DY);

  glLoadMatrixf(glm::value_ptr(mat));
  mModel->Render(mColor, gColorBlack);
}

void CGameEntity::Delete() {
  mDeleted = true;
}


typedef std::map<cb::string, EntityType> _EntityTypeNameMapT;
typedef std::map<cb::string, EntityEvent> _EntityEventNameMapT;
typedef std::map<cb::string, EntityEventAction> _EntityEventActionNameMapT;

static const _EntityTypeNameMapT gEntityTypeNameMap = {
  {L"None", EntityType::ET_NONE},
  {L"Item", EntityType::ET_ITEM},
  {L"Obstacle", EntityType::ET_OBSTACLE},
  {L"Enemy", EntityType::ET_ENEMY}
};

static const _EntityEventNameMapT gEntityEventNameMap = {
  {L"OnDeath", EntityEvent::EE_ONDEATH},
  {L"OnEntityCollision", EntityEvent::EE_ONENTITYCOLLISION},
  {L"OnProjectileCollision", EntityEvent::EE_ONPROJECTILECOLLISION}
};

static const _EntityEventActionNameMapT gEntityEventActionNameMap = {
  {L"None", EntityEventAction::EEA_NONE},
  {L"KillSelf", EntityEventAction::EEA_KILLSELF},
  {L"KillEntity", EntityEventAction::EEA_KILLENTITY},
  {L"AddPoints", EntityEventAction::EEA_ADDPOINTS},
  {L"RemPoints", EntityEventAction::EEA_REMPOINTS},
  {L"DamageSelf", EntityEventAction::EEA_DAMAGESELF},
  {L"DamageEntity", EntityEventAction::EEA_DAMAGEENTITY},
  {L"HealSelf", EntityEventAction::EEA_HEALSELF},
  {L"HealEntity", EntityEventAction::EEA_HEALENTITY},
};

template<typename _Type, typename _TypeMapT = std::map<cb::string, _Type>>
const cb::string _toStrTempl(const _TypeMapT& typeMap, 
                             const _Type& value, 
                             const cb::string& defReturn) {
  for(_TypeMapT::const_iterator it = typeMap.begin(); it != typeMap.end(); it++) {
    if(it->second == value) {
      return it->first;
    }
  }
  return defReturn;
}

template<typename _Type, typename _TypeMapT = std::map<cb::string, _Type>>
const bool _fromStrTempl(const _TypeMapT& typeMap, const cb::string& text, _Type& outValue) {
  typename _TypeMapT::const_iterator it = typeMap.find(text);
  if(it != typeMap.end()) {
    outValue = it->second;
    return true;
  }
  return false;
}


const cb::string toStr(const EntityType type) {
  return _toStrTempl(gEntityTypeNameMap, type, L"None");
}

const cb::string toStr(const EntityEvent event) {
  return _toStrTempl(gEntityEventNameMap, event, cb::string());
}

const cb::string toStr(const EntityEventAction action) {
  return _toStrTempl(gEntityEventActionNameMap, action, L"None");
}

const bool fromStr(const cb::string & text, EntityType & outType) {
  return _fromStrTempl(gEntityTypeNameMap, text, outType);
}

const bool fromStr(const cb::string & text, EntityEvent & outEvent) {
  return _fromStrTempl(gEntityEventNameMap, text, outEvent);
}

const bool fromStr(const cb::string & text, EntityEventAction & outAction) {
  return _fromStrTempl(gEntityEventActionNameMap, text, outAction);
}



static const cb::string XML_ENTITYTYPE_NAME = L"Name";
static const cb::string XML_ENTITYTYPE_TYPE = L"Type";
static const cb::string XML_ENTITYTYPE_SPEED = L"Speed";
static const cb::string XML_ENTITYTYPE_COLOR = L"Color";
static const cb::string XML_ENTITYTYPE_MODELFILE = L"ModelFile";
static const cb::string XML_ENTITYTYPE_MAXHEALTH = L"MaxHealth";
static const cb::string XML_ENTITYTYPE_DAMAGE = L"Damage";
static const cb::string XML_ENTITYTYPE_AIPAUSE = L"AIPause";
static const cb::string XML_ENTITYTYPE_ROTSPEED = L"RotSpeed";
static const cb::string XML_ENTITYTYPE_IGNOREPROJECTILES = L"IgnoreProjectiles";
static const cb::string XML_ENTITYTYPE_POINTS = L"Points";

CB_DEFINEXMLRW(CGameEntityType) {
  RWAttribute(XML_ENTITYTYPE_NAME, mObject.Name);
  RWAttribute(XML_ENTITYTYPE_TYPE, mObject.Type);
  RWAttribute(XML_ENTITYTYPE_SPEED, mObject.Speed);
  RWAttribute(XML_ENTITYTYPE_COLOR, mObject.Color);
  RWAttribute(XML_ENTITYTYPE_MODELFILE, mObject.ModelFile);
  RWAttribute(XML_ENTITYTYPE_MAXHEALTH, mObject.MaxHealth);
  RWAttribute(XML_ENTITYTYPE_DAMAGE, mObject.Damage);
  RWAttribute(XML_ENTITYTYPE_AIPAUSE, mObject.AIPause);
  RWAttribute(XML_ENTITYTYPE_ROTSPEED, mObject.RotSpeed);
  RWAttribute(XML_ENTITYTYPE_IGNOREPROJECTILES, mObject.IgnoreProjectiles);
  RWAttribute(XML_ENTITYTYPE_POINTS, mObject.Points);
  RWAttribute(mObject.EventMap);

  return true;
}

static const cb::string XML_ENTITYTYPEMAP_ELEM = L"EntityType";
static const cb::string XML_ENTITYTYPEMAP_KEY = L"Id";

CB_DEFINEXMLRW(CGameEntityType::TypeMapT) {
  return
    RWNodeMap(mObject, XML_ENTITYTYPEMAP_ELEM, XML_ENTITYTYPEMAP_KEY);
}
