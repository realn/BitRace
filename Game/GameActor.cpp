#include "stdafx.h"
#include "GameActor.h"
#include "Model.h"

CGameActor::CGameActor(const cb::string& name,
                       const cb::string& modelFile,
                       const GameObjectType type,
                       const glm::vec2& pos,
                       const glm::vec2& dir,
                       const glm::vec2& speed,
                       const glm::vec4& color,
                       const float maxHealth,
                       const float collisionRadius)
  : CGameObject(type, pos, dir, speed, color)
  , mName(name)
  , mModelFile(modelFile)
  , mHealth(maxHealth)
  , mMaxHealth(maxHealth)
  , mCollisionRadius(collisionRadius)
{}

CGameActor::~CGameActor() {}

const bool CGameActor::LoadResources(CModelRepository & modelRepository) {
  mpModel = modelRepository.GetModel(mModelFile);
  return mpModel != nullptr;
}

void CGameActor::ModHealth(const float value) {
  mHealth = glm::clamp(mHealth + value, 0.0f, mMaxHealth);
}

void CGameActor::SetHealth(const float value) {
  mHealth = glm::clamp(value, 0.0f, mMaxHealth);
}

const cb::string & CGameActor::GetName() const {
  return mName;
}

const float CGameActor::GetHealth() const {
  return mHealth;
}

const float CGameActor::GetMaxHealth() const {
  return mMaxHealth;
}

const float CGameActor::GetCollisionRadius() const {
  return mCollisionRadius;
}
