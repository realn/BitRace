#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>

#include "LaserGrid.h"

class IFileSystem;
class CGameProjectile;
class CGameProjectileVertex;
class CGameEntity;
class CGameEntityType;
class CModelRepository;

class CGameLevel {
public:
  typedef std::map<cb::string, CGameEntityType> EntityTypeMapT;
  typedef std::vector<CGameEntity> EntityVectorT;
  typedef std::vector<CGameProjectile> ProjectileVectorT;
  typedef std::vector<CGameProjectileVertex> ProjectileVertexVectorT;

private:
  CModelRepository* mpModelRepo;
  const EntityTypeMapT* mpEntityTypes;

  ProjectileVectorT mProjectiles;
  ProjectileVertexVectorT mProjectileVertices;
  EntityVectorT mEntities;

public:
  CGameLevel(CModelRepository* pModelRepo,
             const EntityTypeMapT* pEntityTypes);
  ~CGameLevel();

  void AddEntity(const cb::string& typeId);
  void AddProjectile(const glm::vec2& startPos,
                     const glm::vec2& dir,
                     const glm::vec4& color,
                     const float speed,
                     const float damage);

  void Clear();

  void Update(const glm::vec2& playerVec, const float timeDelta);
  void UpdateRender();
  void Render(const glm::mat4& transform) const;

  const Uint32 CheckProjectileCollisions();
  const float CheckEntityCollisions(const float playerRadius);

private:
  void UpdateEntities(const glm::vec2& playerVec, const float timeDelta);
  void UpdateProjectiles(const glm::vec2& playerVec, const float timeDelta);
  void UpdateRenderProjectiles();
  void RenderProjectiles(const glm::mat4& transform) const;
  const glm::vec2 CreateEntityPosition();
};
