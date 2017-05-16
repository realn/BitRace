#include "stdafx.h"
#include "GameLevel.h"
#include "GameEntity.h"
#include "MeshFunctions.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/closest_point.hpp>

static const float gMapEdgeFar = -100.0f;
static const float gMapEdgeNear = 4.0f;

CGameLevel::CGameLevel(CModelRepository* pModelRepo,
                       const EntityTypeMapT* pEntityTypes)
  : mpModelRepo(pModelRepo)
  , mpEntityTypes(pEntityTypes)
{}

CGameLevel::~CGameLevel() {
  Clear();
}

void CGameLevel::Update(const glm::vec2& playerVec, const float timeDelta) {
  UpdateEntities(playerVec, timeDelta);
  UpdateProjectiles(playerVec, timeDelta);
}

void CGameLevel::UpdateRender() {
  UpdateRenderProjectiles();
}

void CGameLevel::Render(const glm::mat4& transform) const {
  RenderProjectiles(transform);
  for(EntityVectorT::const_iterator it = mEntities.begin(); it != mEntities.end(); it++) {
    it->Render(transform);
  }
}

const Uint32 CGameLevel::CheckProjectileCollisions() {
  Uint32 points = 0;

  for(EntityVectorT::iterator entIt = mEntities.begin(); 
      entIt != mEntities.end(); entIt++) {
    if(entIt->IsDeleted()) {
      continue;
    }
    if(entIt->GetIgnoreProjectiles()) {
      continue;
    }

    for(ProjectileVectorT::iterator prIt = mProjectiles.begin(); 
        prIt != mProjectiles.end(); prIt++) {
      if(prIt->IsDeleted()) {
        continue;
      }

      glm::vec2 pA = prIt->GetPos();
      glm::vec2 pB = (prIt->GetDir() * prIt->GetLength()) + pA;

      glm::vec2 point = glm::closestPointOnLine(entIt->GetPos(), pA, pB);
      if(glm::distance(point, entIt->GetPos()) < entIt->GetCollRadius()) {
        prIt->Delete();
        entIt->ModHealth(-prIt->GetDamage());

        if(entIt->GetHealth() == 0.0f) {
          points += entIt->GetPoints();
          entIt->Delete();
        }
      }
    }
  }
  
  return points;
}

const float CGameLevel::CheckEntityCollisions(const float playerRadius) {
  float damage = 0.0f;
  glm::vec2 playerPos(0.0f);

  for(EntityVectorT::iterator it = mEntities.begin(); it != mEntities.end(); it++) {
    if(it->IsDeleted()) {
      continue;
    }

    if(glm::distance(it->GetPos(), playerPos) < it->GetCollRadius() + playerRadius) {
      damage += it->GetDamage();
      it->Delete();
    }
  }

  return damage;
}

void CGameLevel::Clear() {
  mEntities.clear();
  mProjectiles.clear();
  mProjectileVertices.clear();
}


void CGameLevel::AddEntity(const cb::string& entityId) {
  CGameEntityType::TypeMapT::const_iterator it = mpEntityTypes->find(entityId);
  if(it == mpEntityTypes->end()) {
    cb::error(L"Unknown entity type of id " + entityId);
    return;
  }

  mEntities.push_back(CGameEntity(it->second,
                                  *mpModelRepo,
                                  CreateEntityPosition()));
}

void CGameLevel::AddProjectile(const glm::vec2 & startPos, 
                               const glm::vec2 & dir, 
                               const glm::vec4 & color, 
                               const float speed, 
                               const float damage) {
  mProjectiles.push_back(CProjectile(startPos, dir, color, speed, damage));
}

void CGameLevel::UpdateEntities(const glm::vec2& playerVec, const float timeDelta) {
  EntityVectorT::iterator it = mEntities.begin();
  while(it != mEntities.end()) {
    it->Update(playerVec, timeDelta);

    if(it->IsDeleted() || it->GetPos().y > gMapEdgeNear) {
      it = mEntities.erase(it);
    }
    else {  
      it++;
    }
  }
}

void CGameLevel::UpdateProjectiles(const glm::vec2& playerVec, const float timeDelta) {
  ProjectileVectorT::iterator it = mProjectiles.begin();
  while(it != mProjectiles.end()) {
    it->Update(playerVec, timeDelta);

    if(it->IsDeleted() || it->GetPos().y < gMapEdgeFar) {
      it = mProjectiles.erase(it);
    }
    else {
      it++;
    }
  }
}

void CGameLevel::UpdateRenderProjectiles() {
  if(mProjectiles.empty()) {
    mProjectileVertices.clear();
    return;
  }

  mProjectileVertices.resize(mProjectiles.size());
  for(size_t i = 0; i < mProjectiles.size(); i++) {
    mProjectiles[i].UpdateRender(mProjectileVertices[i]);
  }
}

void CGameLevel::RenderProjectiles(const glm::mat4 & transform) const {
  if(mProjectileVertices.empty())
    return;

  glLoadMatrixf(glm::value_ptr(transform));
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  const size_t vertSize = sizeof(CProjectileVertex) / 2;
  const cb::byte* vertPtr = cb::vectorcastptr<cb::byte>(mProjectileVertices);

  glVertexPointer(3, GL_FLOAT, vertSize, vertPtr);
  glColorPointer(4, GL_FLOAT, vertSize, vertPtr + sizeof(glm::vec3));

  glDrawArrays(GL_LINES, 0, (GLsizei)mProjectileVertices.size() * 2);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

const glm::vec2 CGameLevel::CreateEntityPosition() {
  float randF = float(rand() % 200 - 100);
  return glm::vec2(randF / 100.0f * 30.0f, gMapEdgeFar);
}
