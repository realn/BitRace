#include "stdafx.h"
#include "GameProjectile.h"
#include "GameObjectDefines.h"

static const float PROJECTILE_LENGTH = 3.0f;

CGameProjectile::CGameProjectile(const glm::vec2 & pos,
                                 const glm::vec2 & dir,
                                 const glm::vec4 & color,
                                 const float speed,
                                 const float damage)
  : CGameObject(GameObjectType::Projectile, pos, dir, glm::vec2(speed), color)
  , mDamage(damage)
{}

CGameProjectile::~CGameProjectile() {}

const float CGameProjectile::GetDamage() const {
  return mDamage;
}

const float CGameProjectile::GetLength() const {
  return PROJECTILE_LENGTH;
}

void CGameProjectile::Update(const glm::vec2& playerVec, const float timeDelta) {
  if(mIsDeleted)
    return;

  mPos += ((mDir * mSpeed) - playerVec) * timeDelta;
}

void CGameProjectile::UpdateRender(CGameProjectileVertex & outVertex) {
  if(mIsDeleted)
    return;

  glm::vec2 endPos = mDir * PROJECTILE_LENGTH + mPos;
  outVertex.StartPos = glm::vec3(mPos.x, 0.0f, mPos.y);
  outVertex.EndPos = glm::vec3(endPos.x, 0.0f, endPos.y);

  outVertex.StartColor = mColor;
  outVertex.EndColor = mColor;
}
