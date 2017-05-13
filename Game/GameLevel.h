#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>

#include "LaserGrid.h"
#include "GameEntity.h"

class CRacer;
class CGUI;
class IFileSystem;

class CGameLevel {
private:
  CModelRepository* mModelRepo;

  const GameEntityTypeMapT& mEntityTypes;

  ProjectileVectorT mProjectiles;
  GameEntityVectorT mEntities;
  ProjectileVertexVectorT mProjectileVertices;

  float	m_fMoveX;
  float	m_fDamage;
  unsigned	m_uFireCount;

  const glm::vec2 CreateEntityPosition();

  void Clear();
public:
  CGameLevel(CModelRepository* pModelRepo,
             const GameEntityTypeMapT& entityTypes,
             IFileSystem& fs);
  ~CGameLevel();

  void AddEntity(const cb::string& typeId);

  void Free();
  bool Init();

  void Render(const glm::mat4& transform) const;
  void Update(CRacer& racer, const float timeDelta);

  void FireWeapon();

private:
  void UpdateEntities(CRacer& racer, const float timeDelta);
  void UpdateProjectiles(const float timeDelta);
  void UpdateRenderProjectiles();
  void RenderProjectiles(const glm::mat4& transform) const;
};
