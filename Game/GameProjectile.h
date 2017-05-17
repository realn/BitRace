#ifndef __BITRACE_GAMEPROJECTILE_H__
#define __BITRACE_GAMEPROJECTILE_H__

#include <SDL_types.h>
#include <glm/fwd.hpp>

#include "GameObject.h"

class CGameProjectileVertex {
public:
  glm::vec3 StartPos;
  glm::vec4 StartColor;
  glm::vec3 EndPos;
  glm::vec4 EndColor;
};

class CGameProjectile 
  : public CGameObject
{
private:
  float mDamage;

public:
  CGameProjectile(const glm::vec2& pos,
                  const glm::vec2& dir,
                  const glm::vec4& color,
                  const float speed,
                  const float damage);
  virtual ~CGameProjectile();

  const float GetDamage() const;
  const float GetLength() const;

  void Update(const glm::vec2& playerVec, const float timeDelta);
  void UpdateRender(CGameProjectileVertex& outVertex);
};


#endif // !__BITRACE_GAMEPROJECTILE_H__

