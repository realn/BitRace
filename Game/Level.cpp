#include "Level.h"
#include "GUIScreen.h"
#include "GUI.h"
#include "Space.h"
#include "Model.h"
#include "Entity.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/closest_point.hpp>

#define GLEW_STATIC
#include <GL/glew.h>


enum CLevel::DifficultyId {
  DID_VERY_EASY = 0,
  DID_EASY = 1,
  DID_MEDIUM = 2,
  DID_HARD = 3,
  DID_VERY_HARD = 4,
  DID_HOLY_SHIT = 5
};


//===================================================================================================


class CLevel::CDifficulty {
private:
  std::string m_Name;
  Uint32  m_PointsNeeded;
  float   m_SpawnTime;
  std::map<Uint32, Uint32> m_EntityChance;

public:
  CDifficulty(const std::string& name, const Uint32 pointsNeeded, const float spawnTime);
  ~CDifficulty();

  const std::string GetName() const;
  const Uint32 GetPointsNeeded() const;
  const float GetSpawnTime() const;

  void AddChance(const Uint32 entityId, const Uint32 chance);

  const std::map<Uint32, Uint32>& GetEntityChanceMap() const;
};

CLevel::CDifficulty::CDifficulty(const std::string & name, const Uint32 pointsNeeded, const float spawnTime) :
  m_Name(name), m_PointsNeeded(pointsNeeded), m_SpawnTime(spawnTime) {}

CLevel::CDifficulty::~CDifficulty() {}

const std::string CLevel::CDifficulty::GetName() const {
  return m_Name;
}

const Uint32 CLevel::CDifficulty::GetPointsNeeded() const {
  return m_PointsNeeded;
}

const float CLevel::CDifficulty::GetSpawnTime() const {
  return m_SpawnTime;
}

void CLevel::CDifficulty::AddChance(const Uint32 entityId, const Uint32 chance) {
  m_EntityChance[entityId] = chance;
}

const std::map<Uint32, Uint32>& CLevel::CDifficulty::GetEntityChanceMap() const {
  return m_EntityChance;
}


//===================================================================================================


class CLevel::CProjectile {
private:
  glm::vec2 m_Position;
  glm::vec2 m_Vector;
  glm::vec4 m_Color;
  float m_Size;
  float m_Speed;
  float m_Damage;
  bool  m_Deleted;

public:
  CProjectile(const glm::vec2& pos, const glm::vec2& vec, const glm::vec4& color, const float size, const float speed, const float damage);
  ~CProjectile();

  void Update(const float timeDelta);
  void UpdateParticle(CLineParticle& particle);
  void Delete();

  const bool  CheckCollision(const glm::vec2& point, const float radius, float& outCollisionDist);

  const glm::vec2 GetPosition() const;
  const float GetDamage() const;
  const bool IsDeleted() const;
};


CLevel::CProjectile::CProjectile(const glm::vec2 & pos, const glm::vec2 & vec, const glm::vec4& color, const float size, const float speed, const float damage) :
  m_Position(pos), m_Vector(vec), m_Color(color), m_Size(size), m_Speed(speed), m_Damage(damage), m_Deleted(false) {}

CLevel::CProjectile::~CProjectile() {}

void CLevel::CProjectile::Update(const float timeDelta) {
  if(m_Deleted)
    return;

  m_Position += m_Vector * m_Speed * timeDelta;
}

void CLevel::CProjectile::UpdateParticle(CLevel::CLineParticle& particle) {
  if(m_Deleted)
    return;

  glm::vec2 endPos = m_Position + m_Vector * m_Size;
  particle.Set(glm::vec3(m_Position.x, 0.0f, m_Position.y), glm::vec3(endPos.x, 0.0f, endPos.y), m_Color);
}

void CLevel::CProjectile::Delete() {
  m_Deleted = true;
}

const bool CLevel::CProjectile::CheckCollision(const glm::vec2 & point, const float radius, float & outCollisionDist) {
  glm::vec2 posNext = m_Position + m_Vector * m_Size;
  glm::vec2 closest = glm::closestPointOnLine(point, m_Position, posNext);

  outCollisionDist = glm::distance(closest, point);

  return outCollisionDist < radius;
}

const glm::vec2 CLevel::CProjectile::GetPosition() const {
  return m_Position;
}

const float CLevel::CProjectile::GetDamage() const {
  return m_Damage;
}

const bool CLevel::CProjectile::IsDeleted() const {
  return m_Deleted;
}


//===================================================================================================


CLevel::CLevel() :
  m_pGUIScreen(nullptr),
  m_pGUIPoints(nullptr),
  m_pGUIPointsNeeded(nullptr),
  m_pGUILevelText(nullptr),
  m_pGUIHealthBar(nullptr),
  m_pSpaceTop(nullptr),
  m_pSpaceBottom(nullptr),
  m_pPlayer(nullptr),
  m_WeaponDamage(15.0f),
  m_fIntroTime(0.0f),
  m_fGameOverTime(0.0f),
  m_fGameOverTime2(0.0f),
  m_WeaponNumberOfProjectiles(1),
  m_uPoints(0),
  m_uDifLevel(DID_VERY_EASY),
  m_uNeedPoints(5000),
  m_uTrackState(TS_GAME),
  m_IntroState(IS_STATE1),
  m_uGameOverCharCount(0),
  m_bGameOver(false),
  m_bGameRuning(false),
  m_strGameOver("GAME OVER") 
{
  m_pModelRepo = new CModelRepository();

  glm::vec4 playerColor(0.0f, 1.0f, 1.0f, 1.0f);

  m_EntityTypes[EID_HTTP10] = new CEntityType("Http 1.0", EG_PLAYER, glm::vec2(40.0f, 20.0f), playerColor, CModel::MT_HTTP10, 0, 75.0f, 100.0f, 1.5f);
  m_EntityTypes[EID_HTTP20] = new CEntityType("Http 2.0", EG_PLAYER, glm::vec2(40.0f, 20.0f), playerColor, CModel::MT_HTTP20, 0, 75.0f, 100.0f, 2.0f);
  m_EntityTypes[EID_P2PGNU] = new CEntityType("Gnu Connect", EG_PLAYER, glm::vec2(50.0f, 30.0f), playerColor, CModel::MT_P2PGNU, 0, 75.0f, 100.0f, 2.5f);
  m_EntityTypes[EID_P2PGNU2] = new CEntityType("Gnu Connect 2.0", EG_PLAYER, glm::vec2(50.0f, 35.0f), playerColor, CModel::MT_P2PGNU2, 0, 75.0f, 100.0f, 3.0f);
  m_EntityTypes[EID_P2PFT] = new CEntityType("p2p Freeza", EG_PLAYER, glm::vec2(50.0f, 40.0f), playerColor, CModel::MT_P2PFT, 0, 75.0f, 100.0f, 3.5f);
  m_EntityTypes[EID_P2PFT20] = new CEntityType("p2p Freeza 2.0", EG_PLAYER, glm::vec2(55.0f, 50.0f), playerColor, CModel::MT_P2PFT20, 0, 75.0f, 100.0f, 4.0f);
  m_EntityTypes[EID_P2PEDK2K] = new CEntityType("eDonkey 2000", EG_PLAYER, glm::vec2(60.0f, 55.0f), playerColor, CModel::MT_P2PEDK2K, 0, 75.0f, 100.0f, 4.5f);
  m_EntityTypes[EID_P2PBT] = new CEntityType("p2p BitTorrent", EG_PLAYER, glm::vec2(70.0f, 70.0f), playerColor, CModel::MT_P2PEDK2K, 0, 75.0f, 100.0f, 5.0f);

  m_EntityTypes[EID_BOMB] = new CEntityType("CRC Error", EG_ENEMY, glm::vec2(0.0f, 50.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), CModel::MT_BOMB, 0, -30.0f, 1000.0f, 15.0f);
  m_EntityTypes[EID_HACK] = new CEntityType("H4X0R", EG_ENEMY, glm::vec2(30.0f), glm::vec4(0.5f, 0.2f, 0.8f, 1.0f), CModel::MT_HACK, 200, 30.0f, 30.0f, 30.0f);
  m_EntityTypes[EID_HACK2] = new CEntityType("L33T H4X0R", EG_ENEMY, glm::vec2(50.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), CModel::MT_HACK2, 500, 70.0f, 50.0f, 50.0f);

  m_EntityTypes[EID_DL] = new CEntityType("DownLoad", EG_OTHER, glm::vec2(0.0f, 50.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), CModel::MT_DL_PART, 100, 0.0f, 1.0f, 10.0f);
  m_EntityTypes[EID_DL2] = new CEntityType("BigDownLoad", EG_OTHER, glm::vec2(0.0f, 40.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), CModel::MT_DL_PART2, 1000, 0.0f, 1.0f, 30.0f);

  CDifficulty* pDiff = nullptr;
  {
    pDiff = new CDifficulty("VERY EASY", 3000, 0.6f);
    pDiff->AddChance(EID_BOMB, 60);
    pDiff->AddChance(EID_DL, 30);
    pDiff->AddChance(EID_HACK, 10);
    this->m_DifficultyLevels[DID_VERY_EASY] = pDiff;
  }
  {
    pDiff = new CDifficulty("EASY", 8000, 0.3f);
    pDiff->AddChance(EID_BOMB, 50);
    pDiff->AddChance(EID_DL, 20);
    pDiff->AddChance(EID_HACK, 30);
    this->m_DifficultyLevels[DID_EASY] = pDiff;
  }
  {
    pDiff = new CDifficulty("MEDIUM", 15000, 0.3f);
    pDiff->AddChance(EID_BOMB, 40);
    pDiff->AddChance(EID_HACK, 40);
    pDiff->AddChance(EID_DL, 10);
    pDiff->AddChance(EID_DL2, 10);
    this->m_DifficultyLevels[DID_MEDIUM] = pDiff;
  }
  {
    pDiff = new CDifficulty("HARD", 25000, 0.22f);
    pDiff->AddChance(EID_BOMB, 70);
    pDiff->AddChance(EID_HACK, 10);
    pDiff->AddChance(EID_HACK2, 10);
    pDiff->AddChance(EID_DL, 5);
    pDiff->AddChance(EID_DL2, 5);
    this->m_DifficultyLevels[DID_HARD] = pDiff;
  }
  {
    pDiff = new CDifficulty("VERY HARD", 1000000, 0.13f);
    pDiff->AddChance(EID_BOMB, 70);
    pDiff->AddChance(EID_HACK2, 25);
    pDiff->AddChance(EID_DL2, 5);
    this->m_DifficultyLevels[DID_VERY_HARD] = pDiff;
  }
  {
    pDiff = new CDifficulty("HOLY SHIT!", 0, 0.08f);
    pDiff->AddChance(EID_BOMB, 70);
    pDiff->AddChance(EID_HACK2, 30);
    this->m_DifficultyLevels[DID_HOLY_SHIT] = pDiff;
  }
}

CLevel::~CLevel() {
  Free();

  for(std::map<Uint32, CEntityType*>::iterator it = m_EntityTypes.begin(); it != m_EntityTypes.end(); it++) {
    delete it->second;
  }
  m_EntityTypes.clear();

  for(std::map<Uint32, CDifficulty*>::iterator it = m_DifficultyLevels.begin(); it != m_DifficultyLevels.end(); it++) {
    delete it->second;
  }
  m_DifficultyLevels.clear();

  delete m_pModelRepo;
}

const bool CLevel::Init(CGUI* pGUI, const glm::vec2& screenSize) {
  m_pSpaceTop = new CSpace(glm::vec2(800.0f), glm::uvec2(40), 0.0f);
  m_pSpaceBottom = new CSpace(glm::vec2(800.0f), glm::uvec2(40), 0.0f);

  m_pPlayer = this->CreateEntity(EID_HTTP10, false);

  m_pGUIScreen = new CGUIScreen(pGUI, screenSize);

  {
    glm::vec4 textColor(1.0f, 0.5f, 0.5f, 1.0f);

    m_pGUIPoints = new CGUITextControl("POINTS: 0", textColor);
    m_pGUIScreen->AddControl(m_pGUIPoints, glm::vec2(10.0f, 10.0f));

    m_pGUIPointsNeeded = new CGUITextControl("NEED POINTS: 0", textColor);
    m_pGUIScreen->AddControl(m_pGUIPointsNeeded, glm::vec2(10.0f, 30.0f));

    m_pGUILevelText = new CGUITextControl("LEVEL: UNKNOWN", glm::vec4(1.0f));
    m_pGUIScreen->AddControl(m_pGUILevelText, glm::vec2(10.0f, 10.0f), CGUIScreen::IA_RIGHT | CGUIScreen::IA_TOP);

    m_pGUIHealthBar = new CGUIProgressBarControl(glm::vec2(200.0f, 20.0f), 0.0f, 100.0f, glm::vec4(1.0f, 0.0f, 0.0f, 0.6f));
    m_pGUIScreen->AddControl(m_pGUIHealthBar, glm::vec2(10.0f, 10.0f), CGUIScreen::IA_LEFT | CGUIScreen::IA_BOTTOM);

    m_pGUIHealOverlay = new CGUIRectControl(screenSize, glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    m_pGUIScreen->AddControl(m_pGUIHealOverlay, glm::vec2(0.0f), CGUIScreen::IA_CENTER | CGUIScreen::IA_MIDDLE);

    m_pGUIDamageOverlay = new CGUIRectControl(screenSize, glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    m_pGUIScreen->AddControl(m_pGUIDamageOverlay, glm::vec2(0.0f), CGUIScreen::IA_CENTER | CGUIScreen::IA_MIDDLE);

    m_pGUIControllerList = new CGUIControllerList();
    m_pGUIHealController = new CGUIFadeAnimation(m_pGUIControllerList, m_pGUIHealOverlay, 0.5f, false);
    m_pGUIDamageController = new CGUIFadeAnimation(m_pGUIControllerList, m_pGUIDamageOverlay, 0.3f, false);
  }

  glGenBuffers(1, &m_LineParticleBufferId);

  return true;
}

void CLevel::Free() {
  delete m_pGUIScreen;
  m_pGUIScreen = nullptr;
  m_pGUIPoints = nullptr;
  m_pGUIPointsNeeded = nullptr;
  m_pGUILevelText = nullptr;
  m_pGUIHealthBar = nullptr;
  m_pGUIHealOverlay = nullptr;
  m_pGUIDamageOverlay = nullptr;
  delete m_pGUIControllerList;
  m_pGUIControllerList = nullptr;
  m_pGUIHealController = nullptr;
  m_pGUIDamageController = nullptr;

  if(glIsBuffer(m_LineParticleBufferId))
    glDeleteBuffers(1, &m_LineParticleBufferId);

  delete m_pSpaceTop;
  delete m_pSpaceBottom;
  m_pSpaceTop = nullptr;
  m_pSpaceBottom = nullptr;

  m_vMove = glm::vec2(0.0f);

  m_WeaponNumberOfProjectiles = 1;
  m_WeaponDamage = 15.0f;

  m_uPoints = 0;
  m_uDifLevel = DID_VERY_EASY;
  m_uNeedPoints = 5000;
  m_uTrackState = TS_INTRO;
  m_IntroState = IS_STATE1;
  m_fIntroTime = 0.0f;
  m_fGameOverTime = 0.0f;
  m_fGameOverTime2 = 0.0f;
  m_bGameOver = false;
  m_bGameRuning = false;
  m_uGameOverCharCount = 0;

  Clear();
}

void CLevel::Render(const glm::mat4& transform) {
  switch(m_uTrackState) {
  case TS_NONE:
    return;

  case TS_INTRO:
    Render_Intro();	return;

  case TS_GAME:
    this->RenderGame(transform);
    return;

  case TS_GAMEOVER:
    Render_GameOver(); return;
  };
}

void CLevel::Update(const float timeDelta) {
  m_bGameRuning = false;
  switch(m_uTrackState) {
  case TS_NONE:
    break;

  case TS_INTRO:
    Engine_Intro(timeDelta);
    break;

  case TS_GAME:
    m_bGameRuning = true;
    UpdateGame(timeDelta);
    UpdateGUI(timeDelta);
    break;

  case TS_GAMEOVER:
    Engine_GameOver(timeDelta);
    break;
  }
}

const CEntity * CLevel::GetPlayer() const {
  return m_pPlayer;
}

void CLevel::UpdateGame(const float timeDelta) {
  m_SpawnTime += timeDelta;

  this->GenRandomObject();

  this->m_vMove = m_pPlayer->GetPos();
  this->m_vMove = glm::mod(m_vMove, 20.0f);

  this->UpdateEntities(timeDelta);
  this->UpdateProjectiles(timeDelta);
  this->CheckCollisions();
  this->CheckDifLevel();

  //if(m_pPlayer->GetHealth() < 0.0f)
  //  m_uTrackState = TS_GAMEOVER;
}

void CLevel::UpdateEntities(const float timeDelta) {
  std::vector<CEntity*>::iterator it = m_Entities.begin();
  while(it != m_Entities.end()) {
    if((*it)->IsDeleted()) {
      it = m_Entities.erase(it);
      continue;
    }

    (*it)->Update(timeDelta);

    if((*it)->GetPos().y > 4.0f && (*it)->GetType()->GetGroup() != EG_PLAYER) {
      (*it)->Delete();
    }

    it++;
  }
}

void CLevel::UpdateProjectiles(const float timeDelta) {
  std::vector<CProjectile*>::iterator it = this->m_Projectiles.begin();

  while(it != m_Projectiles.end()) {
    CProjectile* pProjectile = *it;


    if(pProjectile->GetPosition().y < -100.0f) {
      pProjectile->Delete();
    }

    if(pProjectile->IsDeleted()) {
      it = m_Projectiles.erase(it);
      continue;
    }

    pProjectile->Update(timeDelta);

    it++;
  }

  m_LineParticles.resize(m_Projectiles.size());
  for(Uint32 i = 0; i < m_Projectiles.size(); i++) {
    m_Projectiles[i]->UpdateParticle(m_LineParticles[i]);
  }

  if(!m_LineParticles.empty()) {
    glBindBuffer(GL_ARRAY_BUFFER, m_LineParticleBufferId);
    glBufferData(GL_ARRAY_BUFFER, m_LineParticles.size() * sizeof(CLineParticle), &m_LineParticles[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}

void CLevel::UpdateGUI(const float timeDelta) {
  m_pGUIPoints->SetText(CGUI::Format("POINTS: %u", m_uPoints));
  if(m_uDifLevel < DID_VERY_HARD) {
    m_pGUIPointsNeeded->SetVisible(true);
    m_pGUIPointsNeeded->SetText(CGUI::Format("NEED POINTS: %u", m_uNeedPoints));
  }
  else {
    m_pGUIPointsNeeded->SetVisible(false);
  }
  m_pGUILevelText->SetText("LEVEL: " + this->GetDifLevelString());
  m_pGUIHealthBar->SetValue(m_pPlayer->GetHealth());

  m_pGUIControllerList->Update(timeDelta);
}

void CLevel::RenderGame(const glm::mat4& transform) {
  glm::mat4 newTransform = transform * 
    glm::translate(glm::vec3(0.0f, 12.0f, -12.0f)) * 
    glm::rotate(glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  RenderSkybox(newTransform);

  glm::mat4 entityTransform =
    newTransform * glm::translate(glm::vec3(0.0f, -17.0f, 0.0f));

  for(std::vector<CEntity*>::iterator it = m_Entities.begin(); it != m_Entities.end(); it++) {
    (*it)->Render(entityTransform);
  }

  RenderProjectiles(entityTransform);
}

void CLevel::RenderSkybox(const glm::mat4 & transform) {
  glm::vec3 moveVec(-m_vMove.x, 0.0f, m_vMove.y);
  glm::mat4 moveMatrix = transform * glm::translate(moveVec);

  glm::vec4 color(0.0f, 1.0f, 0.0f, 1.0f);
  m_pSpaceTop->Render(moveMatrix * glm::translate(glm::vec3(0.0f, 20.0f, 0.0f)), color);
  m_pSpaceBottom->Render(moveMatrix * glm::translate(glm::vec3(0.0f, -20.0f, 0.0f)), color);
}

void CLevel::RenderProjectiles(const glm::mat4 & transform) {
  if(m_LineParticles.empty())
    return;

  glm::mat4 newTransform = transform * glm::translate(-glm::vec3(m_pPlayer->GetPos().x, 0.0f, 0.0f));
  glLoadMatrixf(glm::value_ptr(newTransform));

  Uint32 vertSize = sizeof(glm::vec3) + sizeof(glm::vec4);
  Uint32 vertOffset = 0;
  Uint32 colorSize = sizeof(glm::vec3) + sizeof(glm::vec4);
  Uint32 colorOffset = sizeof(glm::vec3);

  glBindBuffer(GL_ARRAY_BUFFER, this->m_LineParticleBufferId);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glVertexPointer(3, GL_FLOAT, vertSize, reinterpret_cast<void*>(vertOffset));
  glColorPointer(4, GL_FLOAT, colorSize, reinterpret_cast<void*>(colorOffset));

  glDrawArrays(GL_LINES, 0, m_LineParticles.size() * 2);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CLevel::CheckCollisions() {
  for(std::vector<CEntity*>::iterator it = m_Entities.begin(); it != m_Entities.end(); it++) {
    CheckEntityCollisions(*it);
    CheckProjectileCollisions(*it);
  }
}

void CLevel::CheckEntityCollisions(CEntity * pEntity) {
  if(pEntity->IsDeleted())
    return;

  std::map<float, CEntity*> collisions;

  glm::vec2 entAPos = GetLevelPos(pEntity);
  for(std::vector<CEntity*>::iterator it = m_Entities.begin(); it != m_Entities.end(); it++) {
    if(pEntity == *it || (*it)->IsDeleted())
      continue;

    glm::vec2 entBPos = GetLevelPos(*it);

    float dist = glm::distance(entAPos, entBPos);
    if(dist < 3.0f) {
      collisions[dist] = *it;
    }
  }

  for(std::map<float, CEntity*>::iterator it = collisions.begin(); it != collisions.end(); it++) {
    ExecuteCollision(pEntity, it->second);
    if(pEntity->IsDeleted())
      return;
  }
}

void CLevel::CheckProjectileCollisions(CEntity *pEntity) {
  if (pEntity->IsDeleted())
    return;

  std::map<float, CProjectile*> collisions;

  glm::vec2 entPos = GetLevelPos(pEntity);
  for (std::vector<CProjectile*>::iterator it = m_Projectiles.begin(); it != m_Projectiles.end(); it++) {
    if ((*it)->IsDeleted())
      continue;

    float dist;
    if ((*it)->CheckCollision(entPos, 1.4f, dist)) {
      collisions[dist] = *it;
    }
  }

  for(std::map<float, CProjectile*>::iterator it = collisions.begin(); it != collisions.end(); it++) {
    if(pEntity->GetType()->GetGroup() == EG_ENEMY) {
      pEntity->ModHealth(-it->second->GetDamage());
      it->second->Delete();
    }
  }
}

const bool CLevel::ExecuteCollision(CEntity * pEntityA, CEntity * pEntityB) {
  if(pEntityA->GetType()->GetGroup() == EG_PLAYER) {
    if(pEntityB->GetType()->GetGroup() == EG_ENEMY) {
      pEntityA->ModHealth(-pEntityB->GetType()->GetDamage());
      pEntityB->Delete();
      m_pGUIDamageController->Start();
      return true;
    }
    else if(pEntityB->GetType()->GetGroup() == EG_OTHER) {
      pEntityA->ModHealth(pEntityB->GetType()->GetDamage());
      pEntityB->Delete();
      m_pGUIHealController->Start();
      return true;
    }
  }

  return false;
}

void CLevel::Engine_Intro(float fDT) {
  this->m_fIntroTime += fDT;
  switch(m_IntroState) {
  case IS_STATE1:
    if(m_fIntroTime > 0.3f) {
      m_IntroState = IS_STATE2;
      m_fIntroTime = 0.0f;
    }
    break;
  case IS_STATE2:
    if(m_fIntroTime > 0.7f) {
      m_IntroState = IS_STATE3;
      m_fIntroTime = 0.0f;
      m_vMove.y = -600.0f;
    }
    break;
  case IS_STATE3:
    this->m_vMove.y += 120.0f * fDT;
    if(this->m_vMove.y > 20.0f)
      this->m_vMove.y -= 20.0f;
    if(m_fIntroTime > 2.0f) {
      m_IntroState = IS_STATE4;
      m_fIntroTime = 0.0f;
    }
    break;
  case IS_STATE4:
    this->m_vMove.y += 120.0f * fDT;
    if(this->m_vMove.y > 20.0f)
      this->m_vMove.y -= 20.0f;
    if(m_fIntroTime > 3.0f) {
      m_IntroState = IS_ENDSTATE;
      m_fIntroTime = 0.0f;
    }
    break;
  case IS_ENDSTATE:
    this->m_vMove.y += 120.0f * fDT;
    if(this->m_vMove.y > 20.0f)
      this->m_vMove.y -= 20.0f;
    if(m_fIntroTime > 2.0f) {
      m_IntroState = IS_STATE1;
      m_uTrackState = TS_GAME;
      m_fIntroTime = 0.0f;
    }
    break;
  case IS_SKIP:
    m_IntroState = IS_STATE1;
    m_uTrackState = TS_GAME;
    m_fIntroTime = 0.0f;
    break;
  }
}

void CLevel::Render_Intro() {
  switch(m_IntroState) {
  case IS_STATE1:
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
    glVertex3i(0, 0, -3);
    glEnd();
    break;
  case IS_STATE2:
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(m_fIntroTime * 3.0f, 0.0f, -3.0f);
    glVertex3f(-m_fIntroTime * 3.0f, 0.0f, -3.0f);
    glEnd();
    break;
  case IS_STATE3:
    glColor3f(0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, this->m_vMove.y);

    glPushMatrix();
    glTranslatef(0.0f, 20.0f * m_fIntroTime / 2.0f, 0.0f);
    //this->m_SpaceSky.Render(glm::vec3(0.0f, 1.0f, 0.0f));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -20.0f * m_fIntroTime / 2.0f, 0.0f);
    //this->m_SpaceGround.Render(glm::vec3(0.0f, 1.0f, 0.0f));
    glPopMatrix();
    glPopMatrix();
    break;
  case IS_STATE4:
    glPushMatrix();
    glTranslatef(0.0f, 12.0f * m_fIntroTime / 3.0f, 0.0f);
    glRotatef(15.0f * m_fIntroTime / 3.0f, 1.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, this->m_vMove.y);

    glPushMatrix();
    glTranslatef(0.0f, 20.0f, 0.0f);
    //this->m_SpaceSky.Render(glm::vec3(0.0f, 1.0f, 0.0f));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -20.0f, 0.0f);
    //this->m_SpaceGround.Render(glm::vec3(0.0f, 1.0f, 0.0f));
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
    break;
  case IS_ENDSTATE:
    glPushMatrix();
    glTranslatef(0.0f, 12.0f, -4.0f);
    glTranslatef(0.0f, 0.0f, -8.0f * m_fIntroTime / 2.0f);
    glRotatef(15.0f, 1.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, this->m_vMove.y);

    glPushMatrix();
    glTranslatef(0.0f, 20.0f, 0.0f);
    //this->m_SpaceSky.Render(glm::vec3(0.0f, 1.0f, 0.0f));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -20.0f, 0.0f);
    //this->m_SpaceGround.Render(glm::vec3(0.0f, 1.0f, 0.0f));
    glPopMatrix();

    glPopMatrix();

    glTranslatef(0.0f, -20.0f, 0.0f);
    //m_pPlayer->Render();

    glPopMatrix();
    break;
  case IS_SKIP:
    //this->Render_Track();
    return;
  };
}

void CLevel::Engine_GameOver(float fDT) {
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

void CLevel::Render_GameOver() {
  //if(m_fGameOverTime < 1.0f)
    //this->Render_Track();
}

void CLevel::PlayerModRotation(const float value) {
  if (m_pPlayer) {
    m_pPlayer->ModRotation(value);
  }
}

void CLevel::PlayerFireWeapon() {
  float fSpeed = 50.0f;
  glm::vec4 projectileColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
  glm::vec2 startPos = glm::vec2(this->m_pPlayer->GetPos().x, -2.0f);

  if(m_WeaponNumberOfProjectiles == 1) {
    CProjectile* pProj = new CProjectile(startPos, glm::vec2(0.0f, -1.0f), projectileColor, 1.0f, fSpeed, m_WeaponDamage);
    m_Projectiles.push_back(pProj);
  }
  else {
    for(Uint32 i = 0; i < m_WeaponNumberOfProjectiles; i++) {
      glm::vec2 vec = glm::rotate(glm::vec2(0.0f, -1.0f), glm::radians(-6.0f * float(m_WeaponNumberOfProjectiles - 1) / 2.0f + float(i) * 6.0f));

      CProjectile* pProj = new CProjectile(startPos, vec, projectileColor, 1.0f, fSpeed, m_WeaponDamage);
      m_Projectiles.push_back(pProj);
    }
  }
}

void CLevel::Clear() {
  for(std::vector<CProjectile*>::iterator it = m_Projectiles.begin(); it != m_Projectiles.end(); it++) {
    delete *it;
  }
  for(std::vector<CEntity*>::iterator it = m_Entities.begin(); it != m_Entities.end(); it++) {
    delete *it;
  }

  m_pPlayer = nullptr;
  m_Projectiles.clear();
  m_LineParticles.clear();
  m_Entities.clear();
}

void CLevel::RenderGUI(CGUI *GUI) {
  glm::vec2 screenSize(640.0f, 480.0f);

  if(m_uTrackState == TS_GAMEOVER) {
    if(m_fGameOverTime < 1.0f) {
      GUI->RenderQuadFullScreen(screenSize, glm::vec4(1.0f, 1.0f, 1.0f, m_fGameOverTime));
    }
    else {
      if(m_fGameOverTime < 2.0f) {
        float a = 1.0f - (m_fGameOverTime - 1.0f);
        glColor3f(a, a, a);
        GUI->RenderQuadFullScreen(screenSize, glm::vec4(a, a, a, 1.0f));
      }
      else {
        GUI->RenderQuadFullScreen(screenSize, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        GUI->Print(glm::vec2(270.0f, 230.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
                   m_strGameOver.substr(0, m_uGameOverCharCount) + "_");
      }
    }
    return;
  }
  if(m_uTrackState != TS_GAME)
    return;

  this->m_pGUIScreen->Render();
}

unsigned CLevel::GetDifLevel() {
  return m_uDifLevel;
}

void CLevel::SetDifLevel(unsigned uDifLevel) {
  m_uDifLevel = uDifLevel;
  if(m_uDifLevel > DID_HOLY_SHIT)
    m_uDifLevel = DID_VERY_HARD;
}

const glm::vec2 CLevel::CreateEntityPosition() {
  float randF = float(rand() % 200 + 1 - 100);
  return glm::vec2(randF / 100.0f * 30.0f, -80.0f);
}

CEntity* CLevel::CreateEntity(const Uint32 entityId, const bool randomStartPos) {
  if(entityId == EID_UNKNOWN)
    return nullptr;

  CEntityType* pEntityType = this->m_EntityTypes[entityId];

  glm::vec2 pos = randomStartPos ? CreateEntityPosition() : glm::vec2(0.0f);

  CEntity* pEntity = new CEntity(this, pos, (EntityId)entityId, pEntityType, m_pModelRepo);
  m_Entities.push_back(pEntity);

  return pEntity;
}

void CLevel::GenRandomObject() {
  CDifficulty* pDiff = this->m_DifficultyLevels[m_uDifLevel];

  if(m_SpawnTime < pDiff->GetSpawnTime())
    return;
  m_SpawnTime = 0.0f;

  Uint32 chance = rand() % 100;
  Uint32 lower = 0;
  for(std::map<Uint32, Uint32>::const_iterator it = pDiff->GetEntityChanceMap().begin(); it != pDiff->GetEntityChanceMap().end(); it++) {
    Uint32 upper = lower + it->second;

    if(lower < chance && upper >= chance) {
      CreateEntity((EntityId)it->first);
      break;
    }

    lower = upper;
  }
}


void CLevel::ResetGame() {
  Clear();

  m_uPoints = 0;
  m_uDifLevel = DID_VERY_EASY;
  m_uNeedPoints = m_DifficultyLevels[m_uDifLevel]->GetPointsNeeded();
  m_WeaponDamage = 15.0f;
  m_WeaponNumberOfProjectiles = 1;
  m_uGameOverCharCount = 0;
  m_fGameOverTime = 0.0f;
  m_fGameOverTime2 = 0.0f;
  m_bGameOver = false;
  m_bGameRuning = false;
  m_uTrackState = TS_GAME;
  m_SpawnTime = 0.0f;

  this->m_pPlayer = this->CreateEntity(EID_HTTP10, false);
}

void CLevel::CheckDifLevel() {
  if(m_uNeedPoints == 0)
    return;

  if(m_uPoints < m_uNeedPoints)
    return;

  m_uDifLevel++;
  m_uNeedPoints = m_DifficultyLevels[m_uDifLevel]->GetPointsNeeded();
  //m_uFireCount += 1;
  //m_fDamage += 1.5f;

  //if(m_pRacer->GetModel()->GetId() != this->GetLevelModelType()) {
  //  delete m_pRacer;
  //  m_pRacer = new CRacer(this->GetLevelModelType(), m_pModelRepo);
  //}

  //this->SetUpgScreen(10.0f);
}

std::string CLevel::GetDifLevelString() {
  CDifficulty* pDiff = m_DifficultyLevels[m_uDifLevel];
  return pDiff->GetName();
}

unsigned CLevel::GetPoints() {
  return m_uPoints;
}

void CLevel::SetPoints(unsigned uPoints) {
  m_uPoints = uPoints;
}

unsigned CLevel::GetLevelModelType() {
  switch(m_uDifLevel) {
  case DID_VERY_EASY:
    return CModel::MT_HTTP20;
  case DID_EASY:
    return CModel::MT_P2PGNU2;
  case DID_MEDIUM:
    return CModel::MT_P2PFT20;
  case DID_HARD:
    return CModel::MT_P2PEDK2K;
  case DID_VERY_HARD:
  case DID_HOLY_SHIT:
    return CModel::MT_P2PBT;
  };
  return CModel::MT_HTTP20;
}

const glm::vec2 CLevel::GetLevelPos(CEntity * pEntity) const {
  if(pEntity->GetType()->GetGroup() == EG_PLAYER)
    return glm::vec2(pEntity->GetPos().x, 0.0f);
  return pEntity->GetPos();
}

void CLevel::SkipIntro() {
  m_IntroState = IS_SKIP;
}

bool CLevel::IsGameOver() {
  return m_bGameOver;
}

bool CLevel::IsGameRuning() {
  return m_bGameRuning;
}

CLevel::CLineParticle::CLineParticle() : m_StartColor(1.0f), m_EndColor(1.0f) {}

CLevel::CLineParticle::CLineParticle(const CLineParticle & other) :
  m_StartPos(other.m_StartPos), m_StartColor(other.m_StartColor), m_EndPos(other.m_EndPos), m_EndColor(other.m_EndColor) {}

CLevel::CLineParticle::~CLineParticle() {}

void CLevel::CLineParticle::Set(const glm::vec3 & start, const glm::vec3 & end, const glm::vec4 & color) {
  m_StartPos = start;
  m_EndPos = end;
  m_StartColor = color;
  m_EndColor = color;
}

