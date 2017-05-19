#include "stdafx.h"
#include "GameObject.h"
#include "GameObjectDefines.h"
#include "GameObjectEvent.h"

CGameObject::CGameObject(const GameObjectType type, 
                         const glm::vec2 & pos, 
                         const glm::vec2 & dir, 
                         const glm::vec2 & speed, 
                         const glm::vec4 & color) 
  : mType(type)
  , mPos(pos)
  , mDir(dir)
  , mSpeed(speed)
  , mColor(color)
  , mIsDeleted(false)
{}

CGameObject::~CGameObject() {}

void CGameObject::Delete() {
  mIsDeleted = true;
}

const GameObjectType CGameObject::GetType() const {
  return mType;
}

const glm::vec2 & CGameObject::GetPos() const {
  return mPos;
}

const glm::vec2 & CGameObject::GetDir() const {
  return mDir;
}

const glm::vec2 & CGameObject::GetSpeed() const {
  return mSpeed;
}

const glm::vec2 CGameObject::GetVec() const {
  return mDir * mSpeed;
}

const glm::vec4 & CGameObject::GetColor() const {
  return mColor;
}

const bool CGameObject::IsDeleted() const {
  return mIsDeleted;
}

const CGameObject::EventVectT CGameObject::GetEvents() const {
  return EventVectT();
}

const CGameObject::EventVectT CGameObject::GetEvents(const GameEventTrigger trigger, const GameObjectType senderType) const {
  return EventVectT();
}



static const std::map<GameObjectType, cb::string> gObjectTypeNameMap = {
  {GameObjectType::Unknown, L"Unknown"},
  {GameObjectType::Entity, L"Entity"},
  {GameObjectType::Player, L"Player"},
  {GameObjectType::Projectile, L"Projectile"}
};

const cb::string toStr(const GameObjectType value) {
  return cb::templToStr(gObjectTypeNameMap, value, L"Unknown");
}

const bool fromStr(const cb::string & text, GameObjectType & outValue) {
  return cb::templFromStr(gObjectTypeNameMap, text, outValue);
}
