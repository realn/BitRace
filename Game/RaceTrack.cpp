#include "stdafx.h"
#include "RaceTrack.h"
#include "GLDefines.h"
#include "GUI.h"
#include "MeshFunctions.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>


CGameDifficulty::CGameDifficulty()
  : EntitySpawnPause(0.0f)
  , NextNeededPoints(0) {}

const cb::string CGameDifficulty::GetEntity(const Uint32 pos) const {
  Uint32 weigth = 0;
  for(EntitySpawnMapT::const_iterator it = EntitySpawnRates.begin();
      it != EntitySpawnRates.end(); it++) {
    if(weigth < pos && weigth + it->second > pos) {
      return it->first;
    }
    weigth += it->second;
  }
  return cb::string();
}

const Uint32 CGameDifficulty::GetEntityWeightSum() const {
  Uint32 weight = 0;
  for(EntitySpawnMapT::const_iterator it = EntitySpawnRates.begin();
      it != EntitySpawnRates.end(); it++) {
    weight += it->second;
  }
  return weight;
}


static const glm::vec2 gGridSize(800.0f);
static const glm::uvec2 gGridSteps(40);
static const float gMapEdgeFar = -100.0f;
static const float gMapEdgeNear = 4.0f;

CRaceTrack::CRaceTrack(const GameEntityTypeMapT& entityTypes)
  : mEntityTypes(entityTypes)
  , mGridTop(gGridSize, gGridSteps, 0.0f)
  , mGridBottom(gGridSize, gGridSteps, 0.0f)
  , m_pRacer(NULL)
  , mDiffId(L"DIFF_VERYEASY"),
  m_fMoveX(0.0f),
  m_fTime(0.0f),
  m_fDamage(15.0f),
  m_fIntroTime(0.0f),
  m_fGameOverTime(0.0f),
  m_fGameOverTime2(0.0f),
  m_uPoints(0),
  m_uFireCount(1),
  m_uNeedPoints(5000),
  m_uTrackState(TS_INTRO),
  m_unsignedroState(IS_STATE1),
  m_uGameOverCharCount(0),
  m_bGameOver(false),
  m_bGameRuning(false),
  m_strGameOver("GAME OVER") {
    {
      CGameDifficulty diff;
      diff.Name = L"VERY EASY";
      diff.EntitySpawnPause = 0.6f;
      diff.EntitySpawnRates = {
        {L"ENT_CRCBOMB", 6},
        {L"ENT_DLPART", 3},
        {L"ENT_HACK", 1}
      };
      diff.NextId = L"DIFF_EASY";
      diff.NextNeededPoints = 1000;

      mDifficultyMap[L"DIFF_VERYEASY"] = diff;
    }
    {
      CGameDifficulty diff;
      diff.Name = L"EASY";
      diff.EntitySpawnPause = 0.3f;
      diff.EntitySpawnRates = {
        {L"ENT_CRCBOMB", 5},
        {L"ENT_DLPART", 2},
        {L"ENT_HACK", 3}
      };
      diff.NextId = L"DIFF_MEDIUM";
      diff.NextNeededPoints = 3000;

      mDifficultyMap[L"DIFF_EASY"] = diff;
    }
    {
      CGameDifficulty diff;
      diff.Name = L"MEDIUM";
      diff.EntitySpawnPause = 0.3f;
      diff.EntitySpawnRates = {
        {L"ENT_CRCBOMB", 2},
        {L"ENT_HACK", 2},
        {L"ENT_DLPART", 1},
        {L"ENT_BIGDLPART", 1},
      };
      diff.NextId = L"DIFF_HARD";
      diff.NextNeededPoints = 9000;

      mDifficultyMap[L"DIFF_MEDIUM"] = diff;
    }
    {
      CGameDifficulty diff;
      diff.Name = L"HARD";
      diff.EntitySpawnPause = 0.22f;
      diff.EntitySpawnRates = {
        {L"ENT_CRCBOMB", 70},
        {L"ENT_HACK", 10},
        {L"ENT_LEETHACK", 10},
        {L"ENT_DLPART", 5},
        {L"ENT_BIGDLPART", 5},
      };
      diff.NextId = L"DIFF_VERYHARD";
      diff.NextNeededPoints = 27000;

      mDifficultyMap[L"DIFF_HARD"] = diff;
    }
    {
      CGameDifficulty diff;
      diff.Name = L"VERY HARD";
      diff.EntitySpawnPause = 0.13f;
      diff.EntitySpawnRates = {
        {L"ENT_CRCBOMB", 7},
        {L"ENT_LEETHACK", 2},
        {L"ENT_BIGDLPART", 1},
      };
      diff.NextId = L"DIFF_HOLYSHIT";
      diff.NextNeededPoints = 1000000;

      mDifficultyMap[L"DIFF_VERYHARD"] = diff;
    }
    {
      CGameDifficulty diff;
      diff.Name = L"HOLY SHIT!!!";
      diff.EntitySpawnPause = 0.08f;
      diff.EntitySpawnRates = {
        {L"ENT_CRCBOMB", 4},
        {L"ENT_LEETHACK", 1},
      };

      mDifficultyMap[L"DIFF_HOLYSHIT"] = diff;
    }
}

CRaceTrack::~CRaceTrack() {
  Free();
}

bool CRaceTrack::Init() {

  return true;
}

void CRaceTrack::Free() {
  this->m_vMove = glm::vec2(0.0f);
  this->m_fMoveX = 0.0f;
  m_fTime = 0.0f;
  m_uPoints = 0;
  m_uFireCount = 1;
  m_fDamage = 15.0f;
  m_uNeedPoints = 5000;
  m_uTrackState = TS_INTRO;
  m_unsignedroState = IS_STATE1;
  m_fIntroTime = 0.0f;
  m_fGameOverTime = 0.0f;
  m_fGameOverTime2 = 0.0f;
  m_bGameOver = false;
  m_bGameRuning = false;
  m_uGameOverCharCount = 0;
  Clear();
}

void CRaceTrack::Render(const glm::mat4& transform) const {
  switch(m_uTrackState) {
  case TS_NONE:
    return;
  case TS_INTRO:  Render_Intro(transform);	return;
  case TS_GAME:   Render_Track(transform); return;
  case TS_GAMEOVER: Render_GameOver(transform); return;
  };
}

void CRaceTrack::Update(const float timeDelta) {
  m_bGameRuning = false;
  switch(m_uTrackState) {
  case TS_NONE:
    return;
  case TS_INTRO:
    Engine_Intro(timeDelta);
    return;
  case TS_GAME:
    m_bGameRuning = true;
    Engine_Track(timeDelta);
    return;
  case TS_GAMEOVER:
    Engine_GameOver(timeDelta);
    return;
  }
}

void CRaceTrack::Engine_Intro(float fDT) {
  this->m_fIntroTime += fDT;
  switch(m_unsignedroState) {
  case IS_STATE1:
    if(m_fIntroTime > 0.3f) {
      m_unsignedroState = IS_STATE2;
      m_fIntroTime = 0.0f;
    }
    break;
  case IS_STATE2:
    if(m_fIntroTime > 0.7f) {
      m_unsignedroState = IS_STATE3;
      m_fIntroTime = 0.0f;
      m_vMove.y = -600.0f;
    }
    break;
  case IS_STATE3:
    this->m_vMove.y += 120.0f * fDT;
    if(this->m_vMove.y > 20.0f)
      this->m_vMove.y -= 20.0f;
    if(m_fIntroTime > 2.0f) {
      m_unsignedroState = IS_STATE4;
      m_fIntroTime = 0.0f;
    }
    break;
  case IS_STATE4:
    this->m_vMove.y += 120.0f * fDT;
    if(this->m_vMove.y > 20.0f)
      this->m_vMove.y -= 20.0f;
    if(m_fIntroTime > 3.0f) {
      m_unsignedroState = IS_ENDSTATE;
      m_fIntroTime = 0.0f;
    }
    break;
  case IS_ENDSTATE:
    this->m_vMove.y += 120.0f * fDT;
    if(this->m_vMove.y > 20.0f)
      this->m_vMove.y -= 20.0f;
    if(m_fIntroTime > 2.0f) {
      m_unsignedroState = IS_STATE1;
      m_uTrackState = TS_GAME;
      m_fIntroTime = 0.0f;
    }
    break;
  case IS_SKIP:
    m_unsignedroState = IS_STATE1;
    m_uTrackState = TS_GAME;
    m_fIntroTime = 0.0f;
    break;
  }
}

void CRaceTrack::Render_Intro(const glm::mat4& transform) const {
  switch(m_unsignedroState) {
  case IS_STATE1:
    {
      glLoadMatrixf(glm::value_ptr(transform));
      glColor3f(0.0f, 1.0f, 0.0f);

      vec3vector vlist = {glm::vec3(0.0f, 0.0f, -3.0f)};
      RenderVectorList(GL_POINTS, vlist);
    }
    break;

  case IS_STATE2:
    {
      glLoadMatrixf(glm::value_ptr(transform));
      glColor3f(0.0f, 1.0f, 0.0f);

      vec3vector vlist = {
        glm::vec3(m_fIntroTime * 3.0f, 0.0f, -3.0f),
        glm::vec3(-m_fIntroTime * 3.0f, 0.0f, -3.0f)
      };
      RenderVectorList(GL_LINES, vlist);
    }
    break;

  case IS_STATE3:
    {
      glm::mat4 baseMat = transform *
        glm::translate(glm::vec3(0.0f, 0.0f, this->m_vMove.y));

      glm::mat4 mat = baseMat *
        glm::translate(glm::vec3(0.0f, 20.0f * m_fIntroTime / 2.0f, 0.0f));
      mGridTop.Render(mat, glm::vec3(0.0f, 1.0f, 0.0f));

      mat = baseMat *
        glm::translate(glm::vec3(0.0f, -20.0f * m_fIntroTime / 2.0f, 0.0f));
      mGridBottom.Render(mat, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    break;

  case IS_STATE4:
    {
      glm::mat4 baseMat = transform *
        glm::translate(glm::vec3(0.0f, 12.0f * m_fIntroTime / 3.0f, 0.0f)) *
        glm::rotate(glm::radians(15.0f * m_fIntroTime / 3.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::translate(glm::vec3(0.0f, 0.0f, this->m_vMove.y));

      glm::mat4 mat = baseMat *
        glm::translate(glm::vec3(0.0f, 20.0f, 0.0f));
      mGridTop.Render(mat, glm::vec3(0.0f, 1.0f, 0.0f));

      mat = baseMat *
        glm::translate(glm::vec3(0.0f, -20.0f, 0.0f));
      mGridBottom.Render(mat, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    break;

  case IS_ENDSTATE:
    {
      glm::mat4 baseMat = transform *
        glm::translate(glm::vec3(0.0f, 12.0f, -4.0f)) *
        glm::translate(glm::vec3(0.0f, 0.0f, -8.0f * m_fIntroTime / 2.0f)) *
        glm::rotate(glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));

      glm::mat4 moveMat = baseMat *
        glm::translate(glm::vec3(0.0f, 0.0f, m_vMove.y));

      glm::mat4 skyMat = moveMat *
        glm::translate(glm::vec3(0.0f, 20.0f, 0.0f));
      mGridTop.Render(skyMat, glm::vec3(0.0f, 1.0f, 0.0f));

      glm::mat4 groundMat = moveMat *
        glm::translate(glm::vec3(0.0f, -20.0f, 0.0f));
      mGridBottom.Render(groundMat, glm::vec3(0.0f, 1.0f, 0.0f));

      glm::mat4 racerMat = baseMat *
        glm::translate(glm::vec3(0.0f, -20.0f, 0.0f));
      m_pRacer->Render(racerMat);
    }
    break;

  case IS_SKIP:
    this->Render_Track(transform);
    return;
  };
}

void CRaceTrack::Engine_Track(float fDT) {
  if(m_pRacer == NULL)
    return;

  m_fTime += fDT;
  m_fFSQTime += fDT;
  m_fUpgTime += fDT;

  GenRandomObject();

  m_pRacer->Engine(fDT);

  this->m_vMove.x -= m_pRacer->GetVec().x;
  this->m_vMove.y += 120.0f * fDT;
  this->m_fMoveX += m_pRacer->GetVec().x;

  if(this->m_vMove.y > 20.0f)
    this->m_vMove.y -= 20.0f;

  if(this->m_vMove.x > 20.0f)
    this->m_vMove.x -= 20.0f;
  if(this->m_vMove.x < -20.0f)
    this->m_vMove.x += 20.0f;

  UpdateEntities(fDT);
  UpdateProjectiles(fDT);
  UpdateRenderProjectiles();
  this->CheckDifLevel();

  if(m_pRacer->GetBitRate() <= 0.0f)
    m_uTrackState = TS_GAMEOVER;
}

void CRaceTrack::Render_Track(const glm::mat4& transform) const {
  glm::mat4 mat = transform *
    glm::translate(glm::vec3(0.0f, 12.0f, -12.0f)) *
    glm::rotate(glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  {
    glm::mat4 spaceMat = mat *
      glm::translate(glm::vec3(m_vMove.x, 0.0f, m_vMove.y)) *
      glm::translate(glm::vec3(0.0f, 20.0f, 0.0f));
    mGridTop.Render(spaceMat, glm::vec3(0.0f, 1.0f, 0.0f));

    spaceMat *= glm::translate(glm::vec3(0.0f, -40.0f, 0.0f));
    mGridBottom.Render(spaceMat, glm::vec3(0.0f, 1.0f, 0.0f));
  }

  mat *= glm::translate(glm::vec3(0.0f, -20.0f, 0.0f));
  if(m_pRacer != NULL)
    m_pRacer->Render(mat);

  mat *= glm::translate(glm::vec3(-m_fMoveX, 3.0f, 0.0));
  RenderProjectiles(mat);

  for(GameEntityVectorT::const_iterator it = mEntities.begin(); it != mEntities.end(); it++) {
    (*it)->Render(mat);
  }
}

void CRaceTrack::Engine_GameOver(float fDT) {
  m_fGameOverTime += 0.3f * fDT;
  if(m_fGameOverTime < 2.0f)
    m_fGameOverTime += 1.0f * fDT;
  else {
    m_fGameOverTime2 += 1.0f * fDT;
    if(m_uGameOverCharCount < unsigned(m_strGameOver.length())) {
      if(m_fGameOverTime2 > 0.2f) {
        m_uGameOverCharCount++;
        m_fGameOverTime2 = 0.0f;
      }
    }
    else {
      if(m_fGameOverTime2 > 2.0f)
        m_bGameOver = true;
    }
  }
}

void CRaceTrack::Render_GameOver(const glm::mat4& transform) const {
  if(m_fGameOverTime < 1.0f)
    this->Render_Track(transform);
}

void CRaceTrack::SetRacer(CRacer *pRacer) {
  m_pRacer = pRacer;
}

void CRaceTrack::FireWeapon() {
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

void CRaceTrack::Clear() {
  for(GameEntityVectorT::iterator it = mEntities.begin();
      it != mEntities.end();
      it++) {
    delete (*it);
  }
  mEntities.clear();

  mProjectiles.clear();
  mProjectileVertices.clear();
}

void CRaceTrack::RenderUI(CGUI& gui) const {
  glm::vec2 screenSize(640.0f, 480.0f);

  if(m_uTrackState == TS_GAMEOVER) {
    if(m_fGameOverTime < 1.0f) {
      gui.RenderQuadFullScreen(screenSize, glm::vec4(1.0f, 1.0f, 1.0f, m_fGameOverTime));
    }
    else {
      if(m_fGameOverTime < 2.0f) {
        float a = 1.0f - (m_fGameOverTime - 1.0f);
        glColor3f(a, a, a);
        gui.RenderQuadFullScreen(screenSize, glm::vec4(a, a, a, 1.0f));
      }
      else {
        gui.RenderQuadFullScreen(screenSize, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glColor3f(0.0f, 1.0f, 0.0f);
        gui.Print(glm::vec2(270.0f, 230.0f),
                  m_strGameOver.substr(0, m_uGameOverCharCount) + "_");
      }
    }
    return;
  }
  if(m_uTrackState != TS_GAME)
    return;
  gui.RenderQuad(glm::vec2(8.0f, 3.0f), glm::vec2(400.0f, 40.0f), glm::vec4(0.4f, 0.4f, 1.0f, 0.6f));
  glColor3f(1.0f, 0.5f, 0.5f);
  gui.Print(glm::vec2(10.0f, 5.0f), "POINTS: %u", m_uPoints);
  //if(m_uDifLevel < DL_VERY_HARD)
  gui.Print(glm::vec2(200.0f, 5.0f), "NEED POINTS: %u", m_uNeedPoints);
  glColor3f(1.0f, 1.0f, 1.0f);
  //gui.Print(glm::vec2(10.0f, 22.0f), "LEVEL: %s", GetDifLevelString().c_str());
  gui.RenderProgressBar(glm::vec2(20.0f, 450.0f), glm::vec2(200.0f, 20.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.6f), this->m_pRacer->GetBitRate());
  if(this->m_fUpgTime < this->m_fUpgTimeOut) {
    glPushMatrix();
    glColor4f(1.0f, 1.0f, 1.0f, (m_fUpgTimeOut - m_fUpgTime) / m_fUpgTimeOut);
    glScalef(2.0f, 2.0f, 2.0f);
    gui.Print(glm::vec2(120.0f, 35.0f), "UPGRADE");
    glPopMatrix();
  }
  if(this->m_fFSQTime < this->m_fFSQTimeOut) {
    glm::vec4 color(m_vFSQColor.x, m_vFSQColor.y, m_vFSQColor.z, ((m_fFSQTimeOut - m_fFSQTime) / m_fFSQTimeOut) * 0.5f);
    gui.RenderQuadFullScreen(screenSize, color);
  }
}

const glm::vec2 CRaceTrack::CreateEntityPosition() {
  float randF = float(rand() % 200 + 1 - 100);
  return glm::vec2(randF / 100.0f * 30.0f + m_fMoveX + (100.0f * this->m_pRacer->GetVec().x), gMapEdgeFar);
}

void CRaceTrack::AddEntity(const cb::string& entityId) {
  GameEntityTypeMapT::const_iterator it = mEntityTypes.find(entityId);
  if(it == mEntityTypes.end()) {
    cb::error(L"Unknown entity type of id " + entityId);
    return;
  }

  CGameEntity* pEntity = new CGameEntity(it->second, CreateEntityPosition());
  mEntities.push_back(pEntity);
}

void CRaceTrack::GenRandomObject() {
  const CGameDifficulty& diff = mDifficultyMap[mDiffId];
  if(m_fTime < diff.EntitySpawnPause) {
    return;
  }
  m_fTime -= diff.EntitySpawnPause;

  Uint32 pos = rand() % diff.GetEntityWeightSum();
  cb::string typeId = diff.GetEntity(pos);

  AddEntity(typeId);
}

void CRaceTrack::ResetGame() {
  m_uPoints = 0;
  m_uNeedPoints = 5000;
  m_uFireCount = 1;
  m_uGameOverCharCount = 0;
  m_fDamage = 15.0f;
  m_fGameOverTime = 0.0f;
  m_fGameOverTime2 = 0.0f;
  m_bGameOver = false;
  m_bGameRuning = false;
  m_fUpgTime = 0.0f;
  m_fUpgTimeOut = 0.0f;
  m_fFSQTime = 0.0f;
  m_fFSQTimeOut = 0.0f;
  m_uTrackState = TS_INTRO;
  m_pRacer->Free();
  m_pRacer->Init((Uint32)ModelType::MT_HTTP20);
  Clear();
}

void CRaceTrack::CheckDifLevel() {
  const CGameDifficulty& diff = mDifficultyMap[mDiffId];
  if(diff.NextNeededPoints == 0)
    return;

  if(m_uPoints < diff.NextNeededPoints)
    return;

  mDiffId = diff.NextId;

  m_uFireCount += 1;
  m_fDamage += 1.5f;
  if(m_pRacer->GetModel()->GetType() != this->GetLevelModelType()) {
    m_pRacer->Free();
    m_pRacer->Init((Uint32)this->GetLevelModelType());
  }
  this->SetUpgScreen(10.0f);
}

unsigned CRaceTrack::GetPoints() {
  return m_uPoints;
}

void CRaceTrack::SetPoints(unsigned uPoints) {
  m_uPoints = uPoints;
}

const ModelType CRaceTrack::GetLevelModelType() const {
  //switch(m_uDifLevel) {
  //case DL_VERY_EASY:
  //  return ModelType::MT_HTTP20;
  //case DL_EASY:
  //  return ModelType::MT_P2PGNU2;
  //case DL_MEDIUM:
  //  return ModelType::MT_P2PFT20;
  //case DL_HARD:
  //  return ModelType::MT_P2PEDK2K;
  //case DL_VERY_HARD:
  //case DL_HOLY_SHIT:
  //  return ModelType::MT_P2PBT;
  //};
  return ModelType::MT_HTTP20;
}

void CRaceTrack::SetFSQ(float fTimeOut, glm::vec3 vColor) {
  this->m_fFSQTime = 0.0f;
  this->m_fFSQTimeOut = fTimeOut;
  this->m_vFSQColor = vColor;
}

void CRaceTrack::SetUpgScreen(float fTimeOut) {
  this->m_fUpgTime = 0.0f;
  this->m_fUpgTimeOut = fTimeOut;
  this->SetFSQ(fTimeOut / 4.0f, glm::vec3(1.0f, 1.0f, 0.0f));
}

void CRaceTrack::SkipIntro() {
  m_unsignedroState = IS_SKIP;
}

bool CRaceTrack::IsGameOver() const {
  return m_bGameOver;
}

bool CRaceTrack::IsGameRuning() {
  return m_bGameRuning;
}

void CRaceTrack::UpdateEntities(const float timeDelta) {
  GameEntityVectorT::iterator it = mEntities.begin();
  while(it != mEntities.end()) {
    if((*it)->Update(timeDelta, m_fMoveX, mProjectiles)) {
      m_uPoints += 100;
    }

    if((*it)->IsDeleted()) {
      delete *it;
      it = mEntities.erase(it);
      continue;
    }

    glm::vec3 vPos = glm::vec3((*it)->GetPos().x, 0.0f, (*it)->GetPos().y);

    if(glm::distance(glm::vec3(m_fMoveX, 0.0f, 0.0f), vPos) < 2.0f) {
      m_pRacer->ModBitRate(-(*it)->GetDamage());
      if((*it)->GetDamage() > 0.0f) {
        SetFSQ(0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
      }
      else {
        SetFSQ(0.8f, glm::vec3(0.0f, 1.0f, 0.0));
      }
      (*it)->Delete();
    }

    if((*it)->GetPos().y > gMapEdgeNear) {
      (*it)->Delete();
    }

    it++;
  }
}

void CRaceTrack::UpdateProjectiles(const float timeDelta) {
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

void CRaceTrack::UpdateRenderProjectiles() {
  if(mProjectiles.empty()) {
    mProjectileVertices.clear();
    return;
  }

  mProjectileVertices.resize(mProjectiles.size());
  for(size_t i = 0; i < mProjectiles.size(); i++) {
    mProjectiles[i].UpdateRender(mProjectileVertices[i]);
  }
}

void CRaceTrack::RenderProjectiles(const glm::mat4 & transform) const {
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

