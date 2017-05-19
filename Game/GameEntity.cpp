#include "stdafx.h"
#include "GameEntity.h"
#include "GameObjectEvent.h"
#include "GameObjectEventXml.h"
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
static const float MAX_ENTITY_ROTATION = 70.0f;

CGameEntityType::CGameEntityType()
  : Name(L"")
  , Type(EntityType::None)
  , Speed(0.0f)
  , Color(1.0f)
  , ModelFile()
  , MaxHealth(1.0f)
  , Damage(0.0f)
  , AIPause(0.5f)
  , RotSpeed(20.0f)
  , IgnoreProjectiles(false) 
  , Points(0)
{}

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
  : CGameObject(GameObjectType::Entity, pos, glm::vec2(0.0f, 1.0f), type.Speed, type.Color)
  , mName(type.Name)
  , mEntityType(type.Type)
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
  , mPoints(type.Points)
  , mEvents(type.Events)
{
  mModel = modelRepo.GetModel(type.ModelFile);
}

CGameEntity::~CGameEntity() {}

void CGameEntity::ModHealth(const float value) {
  mHealth = glm::clamp(mHealth + value, 0.0f, mMaxHealth);
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

const EntityType CGameEntity::GetEntityType() const {
  return mEntityType;
}

const bool CGameEntity::GetIgnoreProjectiles() const {
  return mIgnoreProjectiles;
}

const Uint32 CGameEntity::GetPoints() const {
  return mPoints;
}

const CGameEntity::EventVecT CGameEntity::GetEvents() const {
  return mEvents;
}

const CGameEntity::EventVecT CGameEntity::GetEvents(const GameEventTrigger trigger, const GameObjectType senderType) const {
  EventVecT result;
  for(EventVecT::const_iterator it = mEvents.begin(); it != mEvents.end(); it++) {
    if(it->Trigger != trigger)
      continue;
    if(it->SenderIs != senderType && it->SenderIs != GameObjectType::Unknown)
      continue;
    result.push_back(*it);
  }
  return result;
}

void CGameEntity::Update(const glm::vec2& playerVec,
                         const float timeDelta) {
  if(mIsDeleted)
    return;

  glm::vec2 racerPos(0.0f);

  mDir = glm::rotate(glm::vec2(0.0f, 1.0f), glm::radians(mRotAngle));
  mPos += (GetVec() - playerVec) * timeDelta;

  switch(mEntityType) {
  default:
  case EntityType::None:
    return;
  case EntityType::Item:
  case EntityType::Obstacle:
    mRotAngle += timeDelta * mRotSpeed;
    break;

  case EntityType::Enemy:
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
  if(mIsDeleted)
    return;

  glm::mat4 mat = transform *
    glm::translate(glm::vec3(mPos.x, 3.0f, mPos.y)) *
    glm::rotate(glm::radians(mRotAngle), gAxis3DY);

  glLoadMatrixf(glm::value_ptr(mat));
  mModel->Render(mColor, gColorBlack);
}


typedef std::map<EntityType, cb::string> _EntityTypeNameMapT;

static const _EntityTypeNameMapT gEntityTypeNameMap = {
  {EntityType::None, L"None"},
  {EntityType::Item, L"Item"},
  {EntityType::Obstacle, L"Obstacle"},
  {EntityType::Enemy, L"Enemy"}
};

const cb::string toStr(const EntityType type) {
  return cb::templToStr(gEntityTypeNameMap, type, L"None");
}

const bool fromStr(const cb::string & text, EntityType & outType) {
  return cb::templFromStr(gEntityTypeNameMap, text, outType);
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
static const cb::string XML_ENTITYTYPE_EVENT = L"OnEvent";

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
  RWNodeList(mObject.Events, XML_ENTITYTYPE_EVENT);

  return true;
}

static const cb::string XML_ENTITYTYPEMAP_ELEM = L"EntityType";
static const cb::string XML_ENTITYTYPEMAP_KEY = L"Id";

CB_DEFINEXMLRW(CGameEntityType::TypeMapT) {
  return
    RWNodeMap(mObject, XML_ENTITYTYPEMAP_ELEM, XML_ENTITYTYPEMAP_KEY);
}
