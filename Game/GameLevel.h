#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>

#include "LaserGrid.h"
#include "GameEntity.h"

class CGamePlayer;
class CGUI;
class IFileSystem;

class CGameLevel {
private:
  CModelRepository* mModelRepo;

  const CGameEntityType::TypeMapT& mEntityTypes;

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
             const CGameEntityType::TypeMapT& entityTypes,
             IFileSystem& fs);
  ~CGameLevel();

  void AddEntity(const cb::string& typeId);

  void Free();
  bool Init();

  void Render(const glm::mat4& transform) const;
  void Update(CGamePlayer& player, const float timeDelta);

  void FireWeapon();

private:
  void UpdateEntities(CGamePlayer& player, const float timeDelta);
  void UpdateProjectiles(const float timeDelta);
  void UpdateRenderProjectiles();
  void RenderProjectiles(const glm::mat4& transform) const;
};
