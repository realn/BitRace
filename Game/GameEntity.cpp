#include "stdafx.h"
#include "GameEntity.h"
#include "Model.h"
#include "XmlTypes.h"

#include <CBXml/Serialize.h>
#include <CBXml/Node.h>
#include <CBXml/NodeList.h>
#include <CBXml/Attribute.h>
#include <CBXml/AttributeList.h>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

CProjectile::CProjectile(const glm::vec2 & pos,
                         const glm::vec2 & vec,
                         const glm::vec4 & color,
                         const float speed,
                         const float damage)
  : mPos(pos)
  , mVec(vec)
  , mColor(color)
  , mSpeed(speed)
  , mDamage(damage)
  , mDeleted(false) {}

const glm::vec2 & CProjectile::GetPos() const {
  return mPos;
}

const glm::vec2 & CProjectile::GetVec() const {
  return mVec;
}

const float CProjectile::GetDamage() const {
  return mDamage;
}

const bool CProjectile::IsDeleted() const {
  return mDeleted;
}

void CProjectile::Update(const float timeDelta) {
  if(mDeleted)
    return;

  mPos += mVec * mSpeed * timeDelta;
}

void CProjectile::UpdateRender(CProjectileVertex & outVertex) {
  if(mDeleted)
    return;

  glm::vec2 endPos = mVec * 3.0f + mPos;
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
{}

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
  , mIgnoreProjectiles(type.IgnoreProjectiles)
  , mDeleted(false) 
  , mPoints(type.Points)
{
  mModel = modelRepo.GetModel(type.ModelFile);
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

const bool CGameEntity::IsDeleted() const {
  return mDeleted;
}

const Uint32 CGameEntity::GetPoints() const {
  return mPoints;
}

const bool CGameEntity::Update(const float timeDelta, const float racerPosX, ProjectileVectorT & projectiles) {
  if(mDeleted)
    return false;

  mPos += GetVec() * timeDelta;

  if(!mIgnoreProjectiles) {
    for(ProjectileVectorT::iterator it = projectiles.begin(); it != projectiles.end(); it++) {
      if(it->IsDeleted())
        continue;

      if(glm::distance(mPos, it->GetPos()) < 1.4f) {
        mHealth -= it->GetDamage();
        it->Delete();
      }
    }
  }

  if(mHealth <= 0.0f) {
    Delete();
    return true;
  }

  switch(mType) {
  default:
  case EntityType::ET_NONE:
    return false;
  case EntityType::ET_ITEM:
  case EntityType::ET_OBSTACLE:
    {
      mRotAngle += timeDelta * mRotSpeed;
    }
    break;

  case EntityType::ET_ENEMY:
    {
      mAITime += timeDelta;
      if(mAITime > mAIPause) {
        glm::vec2 racerPos = glm::vec2(racerPosX, 0.0f);
        glm::vec2 toRacerVec = glm::normalize(racerPos - mPos);
        mRotAngle = glm::degrees(glm::orientedAngle(toRacerVec, glm::vec2(0.0f, 1.0f)));
        mRotAngle = glm::clamp(mRotAngle, -70.0f, 70.0f);

        mAITime -= mAIPause;
      }
    }
    break;
  }
  return false;
}

void CGameEntity::Render(const glm::mat4 & transform) const {
  if(mDeleted)
    return;

  glm::vec3 rotVec(0.0f, 1.0f, 0.0f);

  glm::mat4 mat = transform *
    glm::translate(glm::vec3(mPos.x, 3.0f, mPos.y)) *
    glm::rotate(glm::radians(mRotAngle), rotVec);

  glLoadMatrixf(glm::value_ptr(mat));
  mModel->Render(mColor,
                 glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void CGameEntity::Delete() {
  mDeleted = true;
}


typedef std::map<cb::string, EntityType> _EntityTypeNameMapT;
static const _EntityTypeNameMapT gEntityTypeNameMap = {
  {L"None", EntityType::ET_NONE},
  {L"Item", EntityType::ET_ITEM},
  {L"Obstacle", EntityType::ET_OBSTACLE},
  {L"Enemy", EntityType::ET_ENEMY}
};

const cb::string toStr(const EntityType type) {
  for(_EntityTypeNameMapT::const_iterator it = gEntityTypeNameMap.begin(); it != gEntityTypeNameMap.end(); it++) {
    if(it->second == type) {
      return it->first;
    }
  }
  return L"None";
}

const bool fromStr(const cb::string & text, EntityType & outType) {
  _EntityTypeNameMapT::const_iterator it = gEntityTypeNameMap.find(text);
  if(it != gEntityTypeNameMap.end()) {
    outType = it->second;
    return true;
  }
  return false;
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

CB_DEFINEXMLREAD(CGameEntityType) {
  GetAttribute(XML_ENTITYTYPE_NAME, mObject.Name);
  GetAttribute(XML_ENTITYTYPE_TYPE, mObject.Type);
  GetAttribute(XML_ENTITYTYPE_SPEED, mObject.Speed);
  GetAttribute(XML_ENTITYTYPE_COLOR, mObject.Color);
  GetAttribute(XML_ENTITYTYPE_MODELFILE, mObject.ModelFile);
  GetAttribute(XML_ENTITYTYPE_MAXHEALTH, mObject.MaxHealth);
  GetAttribute(XML_ENTITYTYPE_DAMAGE, mObject.Damage);
  GetAttribute(XML_ENTITYTYPE_AIPAUSE, mObject.AIPause);
  GetAttribute(XML_ENTITYTYPE_ROTSPEED, mObject.RotSpeed);
  GetAttribute(XML_ENTITYTYPE_IGNOREPROJECTILES, mObject.IgnoreProjectiles);
  GetAttribute(XML_ENTITYTYPE_POINTS, mObject.Points);

  return true;
}

CB_DEFINEXMLWRITE(CGameEntityType) {
  SetAttribute(XML_ENTITYTYPE_NAME, mObject.Name);
  SetAttribute(XML_ENTITYTYPE_TYPE, mObject.Type);
  SetAttribute(XML_ENTITYTYPE_SPEED, mObject.Speed);
  SetAttribute(XML_ENTITYTYPE_COLOR, mObject.Color);
  SetAttribute(XML_ENTITYTYPE_MODELFILE, mObject.ModelFile);
  SetAttribute(XML_ENTITYTYPE_MAXHEALTH, mObject.MaxHealth);
  SetAttribute(XML_ENTITYTYPE_DAMAGE, mObject.Damage);
  SetAttribute(XML_ENTITYTYPE_AIPAUSE, mObject.AIPause);
  SetAttribute(XML_ENTITYTYPE_ROTSPEED, mObject.RotSpeed);
  SetAttribute(XML_ENTITYTYPE_IGNOREPROJECTILES, mObject.IgnoreProjectiles);
  SetAttribute(XML_ENTITYTYPE_POINTS, mObject.Points);

  return true;
}

static const cb::string XML_ENTITYTYPEMAP_ELEM = L"EntityType";
static const cb::string XML_ENTITYTYPEMAP_KEY = L"Id";

CB_DEFINEXMLREAD(GameEntityTypeMapT) {
  return
    GetNodeMap(mObject, XML_ENTITYTYPEMAP_ELEM, XML_ENTITYTYPEMAP_KEY);
}

CB_DEFINEXMLWRITE(GameEntityTypeMapT) {
  return
    SetNodeMap(mObject, XML_ENTITYTYPEMAP_ELEM, XML_ENTITYTYPEMAP_KEY);
}
