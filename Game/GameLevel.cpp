#include "stdafx.h"
#include "GameLevel.h"
#include "MeshFunctions.h"
#include "GameDifficulty.h"
#include "Racer.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

static const float gMapEdgeFar = -100.0f;
static const float gMapEdgeNear = 4.0f;

CGameLevel::CGameLevel(CModelRepository* pModelRepo,
                       const GameEntityTypeMapT& entityTypes,
                       IFileSystem& fs)
  : mModelRepo(pModelRepo)
  , mEntityTypes(entityTypes)
  , m_fMoveX(0.0f)
  , m_fDamage(15.0f)
  , m_uFireCount(1)
{}

CGameLevel::~CGameLevel() {
  Free();
}

bool CGameLevel::Init() {

  return true;
}

void CGameLevel::Free() {
  m_fMoveX = 0.0f;
  m_uFireCount = 1;
  m_fDamage = 15.0f;
  Clear();
}

void CGameLevel::Update(CRacer& racer, const float timeDelta) {
  m_fMoveX += racer.GetVec().x * timeDelta;

  UpdateEntities(racer, timeDelta);
  UpdateProjectiles(timeDelta);
  UpdateRenderProjectiles();
}

void CGameLevel::Render(const glm::mat4& transform) const {
  glm::mat4 mat = transform * glm::translate(glm::vec3(-m_fMoveX, 0.0f, 0.0));

  RenderProjectiles(mat);
  for(GameEntityVectorT::const_iterator it = mEntities.begin(); it != mEntities.end(); it++) {
    (*it)->Render(mat);
  }
}

void CGameLevel::FireWeapon() {
  float fSpeed = 50.0f;
  unsigned i;
  glm::vec2 vStPos = glm::vec2(m_fMoveX, -2.0f);
  glm::vec2 vVec;
  for(i = 0; i < m_uFireCount; ++i) {
    if(m_uFireCount == 1) {
      vVec = glm::vec2(0.0f, -1.0f);
    }
    else {
      vVec = glm::rotate(glm::vec2(0.0f, -1.0f),
                         glm::radians(-6.0f * float(m_uFireCount - 1) / 2.0f + float(i) * 6.0f));
    }

    mProjectiles.push_back(CProjectile(vStPos,
                                       vVec,
                                       glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                                       fSpeed,
                                       m_fDamage));
  }
}

void CGameLevel::Clear() {
  for(GameEntityVectorT::iterator it = mEntities.begin();
      it != mEntities.end();
      it++) {
    delete (*it);
  }
  mEntities.clear();

  mProjectiles.clear();
  mProjectileVertices.clear();
}

const glm::vec2 CGameLevel::CreateEntityPosition() {
  float randF = float(rand() % 200 - 100);
  return glm::vec2(randF / 100.0f * 30.0f + m_fMoveX, gMapEdgeFar);
}

void CGameLevel::AddEntity(const cb::string& entityId) {
  GameEntityTypeMapT::const_iterator it = mEntityTypes.find(entityId);
  if(it == mEntityTypes.end()) {
    cb::error(L"Unknown entity type of id " + entityId);
    return;
  }

  CGameEntity* pEntity = new CGameEntity(it->second, 
                                         *mModelRepo, 
                                         CreateEntityPosition());
  mEntities.push_back(pEntity);
}

void CGameLevel::UpdateEntities(CRacer& racer, const float timeDelta) {
  GameEntityVectorT::iterator it = mEntities.begin();
  while(it != mEntities.end()) {
    if((*it)->Update(timeDelta, m_fMoveX, mProjectiles)) {
      //m_uPoints += 100;
    }

    if((*it)->IsDeleted()) {
      delete *it;
      it = mEntities.erase(it);
      continue;
    }

    glm::vec3 vPos = glm::vec3((*it)->GetPos().x, 0.0f, (*it)->GetPos().y);

    if(glm::distance(glm::vec3(m_fMoveX, 0.0f, 0.0f), vPos) < 2.0f) {
      racer.ModBitRate(-(*it)->GetDamage());
      if((*it)->GetDamage() > 0.0f) {
        //SetFSQ(0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
      }
      else {
        //SetFSQ(0.8f, glm::vec3(0.0f, 1.0f, 0.0));
      }
      (*it)->Delete();
    }

    if((*it)->GetPos().y > gMapEdgeNear) {
      (*it)->Delete();
    }

    it++;
  }
}

void CGameLevel::UpdateProjectiles(const float timeDelta) {
  ProjectileVectorT::iterator it = mProjectiles.begin();
  while(it != mProjectiles.end()) {
    it->Update(timeDelta);

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

