#include "stdafx.h"
#include "GameObject.h"

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
