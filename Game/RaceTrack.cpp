#include "RaceTrack.h"
#include "GUIScreen.h"
#include "GUI.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

enum CRaceTrack::EntityId {
  EID_UNKNOWN = 0,
  EID_DL,
  EID_DL2,
  EID_BOMB,
  EID_HACK,
  EID_HACK2
};

enum CRaceTrack::DifficultyId {
  DID_VERY_EASY = 0,
  DID_EASY = 1,
  DID_MEDIUM = 2,
  DID_HARD = 3,
  DID_VERY_HARD = 4,
  DID_HOLY_SHIT = 5
};

//===================================================================================================


class CRaceTrack::CEntityType {
private:
  std::string m_Name;
  glm::vec2 m_Vec;
  glm::vec4 m_Color;
  Uint32  m_ModelId;

public:
  CEntityType(const std::string name, const glm::vec2& vec, const glm::vec4& color, const Uint32 modelId);

  const std::string GetName() const;
  const glm::vec2 GetVec() const;
  const glm::vec4 GetColor() const;
  const Uint32 GetModelId() const;
};

CRaceTrack::CEntityType::CEntityType(const std::string name, const glm::vec2 & vec, const glm::vec4 & color, const Uint32 modelId) :
  m_Name(name), m_Vec(vec), m_Color(color), m_ModelId(modelId) {}

const std::string CRaceTrack::CEntityType::GetName() const {
  return m_Name;
}

const glm::vec2 CRaceTrack::CEntityType::GetVec() const {
  return m_Vec;
}

const glm::vec4 CRaceTrack::CEntityType::GetColor() const {
  return m_Color;
}

const Uint32 CRaceTrack::CEntityType::GetModelId() const {
  return m_ModelId;
}


//===================================================================================================


class CRaceTrack::CDifficulty {
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

CRaceTrack::CDifficulty::CDifficulty(const std::string & name, const Uint32 pointsNeeded, const float spawnTime) :
  m_Name(name), m_PointsNeeded(pointsNeeded), m_SpawnTime(spawnTime) {}

CRaceTrack::CDifficulty::~CDifficulty() {}

const std::string CRaceTrack::CDifficulty::GetName() const {
  return m_Name;
}

const Uint32 CRaceTrack::CDifficulty::GetPointsNeeded() const {
  return m_PointsNeeded;
}

const float CRaceTrack::CDifficulty::GetSpawnTime() const {
  return m_SpawnTime;
}

void CRaceTrack::CDifficulty::AddChance(const Uint32 entityId, const Uint32 chance) {
  m_EntityChance[entityId] = chance;
}

const std::map<Uint32, Uint32>& CRaceTrack::CDifficulty::GetEntityChanceMap() const {
  return m_EntityChance;
}


//===================================================================================================


CRaceTrack::CShot::CShot(glm::vec2 vVec, glm::vec2 vStartPos, glm::vec3 vColor, float fDamage) :
  m_vVec(vVec), m_vPos(vStartPos), m_vColor(vColor), m_fDamage(fDamage), m_bCanDelete(false) {

}

void CRaceTrack::CShot::Engine(float fDT) {
  if (m_bCanDelete)
    return;

  m_vPos += m_vVec * fDT;
}

void CRaceTrack::CShot::SetRender(glm::vec3 *vpLine, glm::vec3 *vpColor) {
  if (m_bCanDelete)
    return;

  glm::vec2 t = glm::normalize(m_vVec) * 3.0f + m_vPos;
  vpLine[0] = glm::vec3(t.x, 0.0f, t.y);
  vpLine[1] = glm::vec3(m_vPos.x, 0.0f, m_vPos.y);

  vpColor[0] = m_vColor;
  vpColor[1] = m_vColor;
}

glm::vec2 CRaceTrack::CShot::GetPos() const {
  return m_vPos;
}

glm::vec2 CRaceTrack::CShot::GetVec() const {
  return m_vVec;
}

float CRaceTrack::CShot::GetDamage() const {
  return m_fDamage;
}

void CRaceTrack::CShot::SetCanDelete(bool bSet) {
  m_bCanDelete = bSet;
}

bool CRaceTrack::CShot::GetCanDelete() {
  return m_bCanDelete;
}


//===================================================================================================


CRaceTrack::CEntity::CEntity(glm::vec2 vPos, glm::vec2 vVec, glm::vec4 vColor, unsigned uModelType) :
  m_vPos(vPos), m_vVec(vVec), m_vColor(vColor), m_fTemp(0.0f), m_uModelType(uModelType), m_bCanDelete(false), m_Model(NULL) {
  if (uModelType < 9) {
    this->m_uType = ET_NONE;
    return;
  }
  switch (uModelType) {
  default:
    this->m_uType = ET_NONE;
    return;
  case CModel::MT_DL_PART:
    this->m_fHealth = 1.0f;
    this->m_fValue = 100.0f;
    this->m_uType = ET_BONUS;
    break;
  case CModel::MT_DL_PART2:
    this->m_fHealth = 1.0f;
    this->m_fValue = 1000.0f;
    this->m_uType = ET_BONUS;
    break;
  case CModel::MT_BOMB:
    this->m_fHealth = 20.0f;
    this->m_fValue = 15.0f;
    this->m_uType = ET_ENEMY;
    break;
  case CModel::MT_HACK:
    this->m_fHealth = 30.0f;
    this->m_fValue = 30.0f;
    this->m_uType = ET_ENEMY;
    break;
  case CModel::MT_HACK2:
    this->m_fHealth = 50.0f;
    this->m_fValue = 50.0f;
    this->m_uType = ET_ENEMY;
    break;
  };
  this->m_Model = CModel::GetModel(uModelType);
}

bool CRaceTrack::CEntity::Engine(float fDT, float fRacerPosX, CRaceTrack::CShot **aShotList, const unsigned uShotCount) {
  if (m_bCanDelete)
    return false;

  this->m_vPos += this->m_vVec * fDT;
  unsigned i;
  for (i = 0; i < uShotCount; i++) {
    if (aShotList[i]->GetCanDelete())
      continue;
    if (m_uModelType != CModel::MT_BOMB && glm::distance(this->m_vPos, aShotList[i]->GetPos()) < 1.4f) {
      this->m_fHealth -= aShotList[i]->GetDamage();
      aShotList[i]->SetCanDelete(true);
      continue;
    }
  }
  if (m_fHealth <= 0.0f) {
    this->SetCanDelete(true);
    if (this->m_uModelType != CModel::MT_DL_PART || this->m_uModelType != CModel::MT_DL_PART2)
      return true;
    else return false;
  }
  switch (this->m_uType) {
  default:
  case ET_NONE:
    return false;
  case ET_BONUS:
    this->m_fTemp += fDT;
    break;
  case ET_ENEMY:
    switch (this->m_uModelType) {
    case CModel::MT_BOMB:
      this->m_fTemp += 70.0f * fDT;
      break;
    case CModel::MT_HACK:
      this->m_fTemp += fDT;
      if (m_fTemp > 0.4f) {
        m_fTemp = 0.0f;
        glm::vec2 vRPos = glm::vec2(fRacerPosX, 0.0f);
        this->m_vVec = glm::normalize(vRPos - this->m_vPos);
        if (this->m_vVec.y < 0.4f)
          this->m_vVec.y = 0.4f;
        this->m_vVec *= 30.0f;
      }
      break;
    case CModel::MT_HACK2:
      glm::vec2 vRPos = glm::vec2(fRacerPosX, 0.0f);
      this->m_vVec = glm::normalize(vRPos - this->m_vPos);
      this->m_fTemp = glm::degrees(glm::orientedAngle(m_vVec, glm::vec2(0.0f, 1.0f)));
      if (this->m_vVec.y < 0.4f)
        this->m_vVec.y = 0.4f;
      this->m_vVec *= 50.0f;
      break;
    };
    break;
  };
  return false;
}

void CRaceTrack::CEntity::Render() {
  if (m_bCanDelete)
    return;

  glPushMatrix();

  glTranslatef(m_vPos.x, 3.0f, m_vPos.y);
  switch (this->m_uModelType) {
  case CModel::MT_BOMB:
    glRotatef(-m_fTemp, 0.0f, 1.0f, 0.0f);
    break;
  case CModel::MT_HACK:
    glRotatef(m_fTemp, 0.0f, 1.0f, 0.0f);
    break;
  case CModel::MT_HACK2:
    glRotatef(m_fTemp, 0.0f, 1.0f, 0.0f);
    break;
  }
  glColor3fv(glm::value_ptr(m_vColor));
  m_Model->Render();

  glPopMatrix();
}

float CRaceTrack::CEntity::GetHealth() {
  return m_fHealth;
}

bool CRaceTrack::CEntity::GetCanDelete() {
  return m_bCanDelete;
}

unsigned CRaceTrack::CEntity::GetModelType() {
  return m_uModelType;
}

unsigned CRaceTrack::CEntity::GetType() {
  return m_uType;
}

float CRaceTrack::CEntity::GetValue() {
  return m_fValue;
}

glm::vec2 CRaceTrack::CEntity::GetPos() {
  return m_vPos;
}

void CRaceTrack::CEntity::SetCanDelete(bool bSet) {
  m_bCanDelete = bSet;
}

//===========================================
CRaceTrack::CRaceTrack() :
  m_pGUIScreen(nullptr),
  m_pGUIPoints(nullptr),
  m_pGUIPointsNeeded(nullptr),
  m_pGUILevelText(nullptr),
  m_pGUIHealthBar(nullptr),
  m_pRacer(nullptr),
  m_fMoveX(0.0f),
  m_fTime(0.0f),
  m_fDamage(15.0f),
  m_fIntroTime(0.0f),
  m_fGameOverTime(0.0f),
  m_fGameOverTime2(0.0f),
  m_uPoints(0),
  m_uFireCount(1),
  m_uDifLevel(DID_VERY_EASY),
  m_uNeedPoints(5000),
  m_uTrackState(TS_INTRO),
  m_IntroState(IS_STATE1),
  m_uGameOverCharCount(0),
  m_bGameOver(false),
  m_bGameRuning(false),
  m_strGameOver("GAME OVER") 
{
    m_EntityTypes[EID_DL] = new CEntityType("DownLoad", glm::vec2(0.0f, 50.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), CModel::MT_DL_PART);
    m_EntityTypes[EID_DL2] = new CEntityType("BigDownLoad", glm::vec2(0.0f, 40.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), CModel::MT_DL_PART2);
    m_EntityTypes[EID_BOMB] = new CEntityType("CRC Error", glm::vec2(0.0f, 50.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), CModel::MT_BOMB);
    m_EntityTypes[EID_HACK] = new CEntityType("H4X0R", glm::vec2(0.0f, 30.0f), glm::vec4(0.5f, 0.2f, 0.8f, 1.0f), CModel::MT_HACK);
    m_EntityTypes[EID_HACK2] = new CEntityType("L33T H4X0R", glm::vec2(0.0f, 50.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), CModel::MT_HACK2);

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

CRaceTrack::~CRaceTrack() {
  Free();

  for (std::map<Uint32, CEntityType*>::iterator it = m_EntityTypes.begin(); it != m_EntityTypes.end(); it++) {
    delete it->second;
  }
  m_EntityTypes.clear();

  for (std::map<Uint32, CDifficulty*>::iterator it = m_DifficultyLevels.begin(); it != m_DifficultyLevels.end(); it++) {
    delete it->second;
  }
  m_DifficultyLevels.clear();
}

const bool CRaceTrack::Init(CGUI* pGUI, const glm::vec2& screenSize) {
  if (!this->m_SpaceSky.Generate(800.0f, 800.0f, 40, 40, 0.0f))
    return false;
  if (!this->m_SpaceGround.Generate(800.0f, 800.0f, 40, 40, 0.0f)) {
    Free();
    return false;
  }



  m_pGUIScreen = new CGUIScreen(pGUI, screenSize);

  {
    glm::vec4 textColor(1.0f, 0.5f, 0.5f, 1.0f);

    m_pGUIPoints = new CGUITextControl(m_pGUIScreen, "POINTS: 0", textColor);
    m_pGUIScreen->AddControl(m_pGUIPoints, glm::vec2(10.0f, 10.0f));

    m_pGUIPointsNeeded = new CGUITextControl(m_pGUIScreen, "NEED POINTS: 0", textColor);
    m_pGUIScreen->AddControl(m_pGUIPointsNeeded, glm::vec2(10.0f, 30.0f));

    m_pGUILevelText = new CGUITextControl(m_pGUIScreen, "LEVEL: UNKNOWN", glm::vec4(1.0f));
    m_pGUIScreen->AddControl(m_pGUILevelText, glm::vec2(10.0f, 10.0f), CGUIScreen::IA_RIGHT | CGUIScreen::IA_TOP);

    m_pGUIHealthBar = new CGUIProgressBarControl(m_pGUIScreen, glm::vec2(200.0f, 20.0f), 0.0f, 100.0f, glm::vec4(1.0f, 0.0f, 0.0f, 0.6f));
    m_pGUIScreen->AddControl(m_pGUIHealthBar, glm::vec2(10.0f, 10.0f), CGUIScreen::IA_LEFT | CGUIScreen::IA_BOTTOM);
  }

  return true;
}

void CRaceTrack::Free() {
  delete m_pGUIScreen;
  m_pGUIScreen = nullptr;
  m_pGUIPoints = nullptr;
  m_pGUIPointsNeeded = nullptr;
  m_pGUILevelText = nullptr;
  m_pGUIHealthBar = nullptr;

  this->m_SpaceSky.Free();
  this->m_SpaceGround.Free();
  this->m_vMove = glm::vec2(0.0f);
  this->m_fMoveX = 0.0f;
  m_fTime = 0.0f;
  m_uPoints = 0;
  m_uDifLevel = DID_VERY_EASY;
  m_uFireCount = 1;
  m_fDamage = 15.0f;
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

void CRaceTrack::Render() {
  switch (m_uTrackState) {
  case TS_NONE:
    return;
  case TS_INTRO:
    Render_Intro();	return;
  case TS_GAME:
    Render_Track(); return;
  case TS_GAMEOVER:
    Render_GameOver(); return;
  };
}

void CRaceTrack::Update(const float timeDelta) {
  m_bGameRuning = false;
  switch (m_uTrackState) {
  case TS_NONE:
    break;

  case TS_INTRO:
    Engine_Intro(timeDelta); 
    break;

  case TS_GAME:
    m_bGameRuning = true;
    UpdateTrack(timeDelta); 
    UpdateGUI(timeDelta);
    break;

  case TS_GAMEOVER:
    Engine_GameOver(timeDelta); 
    break;
  }
}

void CRaceTrack::UpdateTrack(const float timeDelta) {
  if (m_pRacer == NULL)
    return;

  m_fTime += timeDelta;
  m_fFSQTime += timeDelta;
  m_fUpgTime += timeDelta;
  this->GenRandomObject();

  m_pRacer->Engine(timeDelta);

  this->m_vMove.x -= m_pRacer->GetVec().x;
  this->m_vMove.y += 120.0f * timeDelta;
  this->m_fMoveX += m_pRacer->GetVec().x;

  this->m_vMove = glm::mod(m_vMove, 20.0f);

  this->Engine_Entity(timeDelta);
  this->Engine_Shot(timeDelta);
  this->CheckDifLevel();

  if (m_pRacer->GetBitRate() <= 0.0f)
    m_uTrackState = TS_GAMEOVER;
}

void CRaceTrack::UpdateGUI(const float timeDelta) {
  m_pGUIPoints->SetText(CGUI::Format("POINTS: %u", m_uPoints));
  if (m_uDifLevel < DID_VERY_HARD) {
    m_pGUIPointsNeeded->SetVisible(true);
    m_pGUIPointsNeeded->SetText(CGUI::Format("NEED POINTS: %u", m_uNeedPoints));
  }
  else {
    m_pGUIPointsNeeded->SetVisible(false);
  }
  m_pGUILevelText->SetText("LEVEL: " + this->GetDifLevelString());
  m_pGUIHealthBar->SetValue(m_pRacer->GetBitRate());
}

void CRaceTrack::Engine_Intro(float fDT) {
  this->m_fIntroTime += fDT;
  switch (m_IntroState) {
  case IS_STATE1:
    if (m_fIntroTime > 0.3f) {
      m_IntroState = IS_STATE2;
      m_fIntroTime = 0.0f;
    }
    break;
  case IS_STATE2:
    if (m_fIntroTime > 0.7f) {
      m_IntroState = IS_STATE3;
      m_fIntroTime = 0.0f;
      m_vMove.y = -600.0f;
    }
    break;
  case IS_STATE3:
    this->m_vMove.y += 120.0f * fDT;
    if (this->m_vMove.y > 20.0f)
      this->m_vMove.y -= 20.0f;
    if (m_fIntroTime > 2.0f) {
      m_IntroState = IS_STATE4;
      m_fIntroTime = 0.0f;
    }
    break;
  case IS_STATE4:
    this->m_vMove.y += 120.0f * fDT;
    if (this->m_vMove.y > 20.0f)
      this->m_vMove.y -= 20.0f;
    if (m_fIntroTime > 3.0f) {
      m_IntroState = IS_ENDSTATE;
      m_fIntroTime = 0.0f;
    }
    break;
  case IS_ENDSTATE:
    this->m_vMove.y += 120.0f * fDT;
    if (this->m_vMove.y > 20.0f)
      this->m_vMove.y -= 20.0f;
    if (m_fIntroTime > 2.0f) {
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

void CRaceTrack::Render_Intro() {
  switch (m_IntroState) {
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
    this->m_SpaceSky.Render(glm::vec3(0.0f, 1.0f, 0.0f));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -20.0f * m_fIntroTime / 2.0f, 0.0f);
    this->m_SpaceGround.Render(glm::vec3(0.0f, 1.0f, 0.0f));
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
    this->m_SpaceSky.Render(glm::vec3(0.0f, 1.0f, 0.0f));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -20.0f, 0.0f);
    this->m_SpaceGround.Render(glm::vec3(0.0f, 1.0f, 0.0f));
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
    this->m_SpaceSky.Render(glm::vec3(0.0f, 1.0f, 0.0f));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -20.0f, 0.0f);
    this->m_SpaceGround.Render(glm::vec3(0.0f, 1.0f, 0.0f));
    glPopMatrix();

    glPopMatrix();

    glTranslatef(0.0f, -20.0f, 0.0f);
    m_pRacer->Render();

    glPopMatrix();
    break;
  case IS_SKIP:
    this->Render_Track();
    return;
  };
}


void CRaceTrack::Render_Track() {
  glPushMatrix();

  glTranslatef(0.0f, 12.0f, -12.0f);
  glRotatef(15.0f, 1.0f, 0.0f, 0.0f);

  glPushMatrix();
  glTranslatef(this->m_vMove.x, 0.0f, this->m_vMove.y);
  glTranslatef(0.0f, 20.0f, 0.0f);
  this->m_SpaceSky.Render(glm::vec3(0.0f, 1.0f, 0.0f));
  glTranslatef(0.0f, -40.0f, 0.0f);
  this->m_SpaceGround.Render(glm::vec3(0.0f, 1.0f, 0.0f));
  glPopMatrix();

  glTranslatef(0.0f, -20.0f, 0.0f);
  if (m_pRacer != NULL)
    m_pRacer->Render();

  glTranslatef(-m_fMoveX, 3.0f, 0.0);
  if (this->m_aShotList.size() > 0) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &this->m_avShotRenderList[0]);
    glColorPointer(3, GL_FLOAT, 0, &this->m_avShotRenderColorList[0]);

    glDrawArrays(GL_LINES, 0, (GLsizei)this->m_aShotList.size() * 2);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
  }

  size_t i;
  for (i = 0; i < this->m_aEntityList.size(); ++i)
    this->m_aEntityList[i]->Render();

  glPopMatrix();
}

void CRaceTrack::Engine_GameOver(float fDT) {
  m_fGameOverTime += 0.3f * fDT;
  if (m_fGameOverTime < 2.0f)
    m_fGameOverTime += 1.0f * fDT;
  else {
    m_fGameOverTime2 += 1.0f * fDT;
    if (m_uGameOverCharCount < unsigned(m_strGameOver.length())) {
      if (m_fGameOverTime2 > 0.2f) {
        m_uGameOverCharCount++;
        m_fGameOverTime2 = 0.0f;
      }
    }
    else {
      if (m_fGameOverTime2 > 2.0f)
        m_bGameOver = true;
    }
  }
}

void CRaceTrack::Render_GameOver() {
  if (m_fGameOverTime < 1.0f)
    this->Render_Track();
}

void CRaceTrack::SetRacer(CRacer *pRacer) {
  m_pRacer = pRacer;
}

void CRaceTrack::DeleteShot(size_t i) {
  if (i >= this->m_aShotList.size())
    return;

  delete this->m_aShotList[i];
  this->m_aShotList.erase(this->m_aShotList.begin() + i);
  this->m_avShotRenderList.pop_back();
  this->m_avShotRenderList.pop_back();
  this->m_avShotRenderColorList.pop_back();
  this->m_avShotRenderColorList.pop_back();
}

void CRaceTrack::FireWeapon() {
  float fSpeed = 50.0f;
  unsigned i;
  glm::vec2 vStPos = glm::vec2(m_fMoveX, -2.0f);
  glm::vec2 vVec;
  for (i = 0; i < m_uFireCount; ++i) {
    if(m_uFireCount == 1)
      vVec = glm::vec2(0.0f, -1.0f);
    else
      vVec = glm::rotate(glm::vec2(0.0f, -1.0f), glm::radians(-6.0f * float(m_uFireCount - 1) / 2.0f + float(i) * 6.0f));

    this->m_aShotList.push_back(new CShot(vVec * fSpeed, vStPos, glm::vec3(0.0f, 0.0f, 1.0f), m_fDamage));
    this->m_avShotRenderList.push_back(glm::vec3());
    this->m_avShotRenderList.push_back(glm::vec3());
    this->m_avShotRenderColorList.push_back(glm::vec3());
    this->m_avShotRenderColorList.push_back(glm::vec3());
  }
}

void CRaceTrack::Clear() {
  int i;
  for (i = int(this->m_aShotList.size() - 1); i >= 0; --i) {
    this->DeleteShot(i);
  }
  for (i = int(this->m_aEntityList.size() - 1); i >= 0; --i) {
    delete this->m_aEntityList[i];
    this->m_aEntityList.erase(this->m_aEntityList.begin() + i);
  }
}

void CRaceTrack::RenderGUI(CGUI *GUI) {
  glm::vec2 screenSize(640.0f, 480.0f);

  if (m_uTrackState == TS_GAMEOVER) {
    if (m_fGameOverTime < 1.0f) {
      GUI->RenderQuadFullScreen(screenSize, glm::vec4(1.0f, 1.0f, 1.0f, m_fGameOverTime));
    }
    else {
      if (m_fGameOverTime < 2.0f) {
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
  if (m_uTrackState != TS_GAME)
    return;

  this->m_pGUIScreen->Render();

  if (this->m_fUpgTime < this->m_fUpgTimeOut) {
    glPushMatrix();
    glm::vec4 color(1.0f, 1.0f, 1.0f, (m_fUpgTimeOut - m_fUpgTime) / m_fUpgTimeOut);
    glScalef(2.0f, 2.0f, 2.0f);
    GUI->Print(glm::vec2(120.0f, 35.0f), color, "UPGRADE");
    glPopMatrix();
  }
  if (this->m_fFSQTime < this->m_fFSQTimeOut) {
    glm::vec4 color(m_vFSQColor.x, m_vFSQColor.y, m_vFSQColor.z, ((m_fFSQTimeOut - m_fFSQTime) / m_fFSQTimeOut) * 0.5f);
    GUI->RenderQuadFullScreen(screenSize, color);
  }
}

unsigned CRaceTrack::GetDifLevel() {
  return m_uDifLevel;
}

void CRaceTrack::SetDifLevel(unsigned uDifLevel) {
  m_uDifLevel = uDifLevel;
  if (m_uDifLevel > DID_HOLY_SHIT)
    m_uDifLevel = DID_VERY_HARD;
}

const glm::vec2 CRaceTrack::CreateEntityPosition() {
  float randF = float(rand() % 200 + 1 - 100);
  return glm::vec2(randF / 100.0f * 30.0f + m_fMoveX + (100.0f * this->m_pRacer->GetVec().x), -80.0f);
}

void CRaceTrack::AddEntity(const EntityId entityId) {
  if (entityId == EID_UNKNOWN)
    return;

  CEntityType* pEntity = this->m_EntityTypes[entityId];

  this->AddEntity(pEntity->GetVec(), pEntity->GetColor(), (CModel::MODEL_TYPE)pEntity->GetModelId());
}

void CRaceTrack::AddEntity(const glm::vec2& vec, const glm::vec3& color, const CModel::MODEL_TYPE type) {
  CEntity* pEntity = new CEntity(this->CreateEntityPosition(), vec, glm::vec4(color, 1.0f), type);
  this->m_aEntityList.push_back(pEntity);
}

void CRaceTrack::AddEntity_DL() {
  this->AddEntity(glm::vec2(0.0f, 50.0f), glm::vec3(1.0f, 1.0f, 0.0f), CModel::MT_DL_PART);
}

void CRaceTrack::AddEntity_DL2() {
  this->AddEntity(glm::vec2(0.0f, 40.0f), glm::vec3(1.0f, 1.0f, 0.0f), CModel::MT_DL_PART2);
}

void CRaceTrack::AddEntity_BOMB() {
  this->AddEntity(glm::vec2(0.0f, 50.0f), glm::vec3(1.0f, 0.0f, 1.0f), CModel::MT_BOMB);
}

void CRaceTrack::AddEntity_HACK() {
  this->AddEntity(glm::vec2(0.0f, 30.0f), glm::vec3(0.5f, 0.2f, 0.8f), CModel::MT_HACK);
}

void CRaceTrack::AddEntity_HACK2() {
  this->AddEntity(glm::vec2(0.0f, 50.0f), glm::vec3(1.0f, 0.0f, 0.0f), CModel::MT_HACK2);
}

void CRaceTrack::GenRandomObject() {
  CDifficulty* pDiff = this->m_DifficultyLevels[m_uDifLevel];

  if (m_fTime < pDiff->GetSpawnTime())
    return;
  m_fTime = 0.0f;

  Uint32 chance = rand() % 100;
  Uint32 lower = 0;
  for (std::map<Uint32, Uint32>::const_iterator it = pDiff->GetEntityChanceMap().begin(); it != pDiff->GetEntityChanceMap().end(); it++) {
    Uint32 upper = lower + it->second;

    if (lower < chance && upper >= chance) {
      this->AddEntity((EntityId)it->first);
      break;
    }

    lower = upper;
  }
}

void CRaceTrack::Engine_Entity(float fDT) {
  int i;
  for (i = int(this->m_aEntityList.size() - 1); i >= 0; --i) {
    if (this->m_aShotList.size() != 0) {
      if (this->m_aEntityList[i]->Engine(fDT, m_fMoveX, &this->m_aShotList[0], (unsigned)this->m_aShotList.size())) {
        switch (this->m_aEntityList[i]->GetModelType()) {
        case CModel::MT_HACK:
          m_uPoints += 500;
          break;
        case CModel::MT_HACK2:
          m_uPoints += 2000;
          if (m_uDifLevel == DID_HOLY_SHIT)
            this->m_pRacer->ModBitRate(1.0f);
          break;
        }
      }
    }
    else this->m_aEntityList[i]->Engine(fDT, m_fMoveX, NULL, 0);

    if (this->m_aEntityList[i]->GetCanDelete()) {
      delete this->m_aEntityList[i];
      this->m_aEntityList.erase(this->m_aEntityList.begin() + i);
      continue;
    }

    glm::vec3 vPos = glm::vec3(this->m_aEntityList[i]->GetPos().x, 0.0f, this->m_aEntityList[i]->GetPos().y);

    if (glm::distance(glm::vec3(m_fMoveX, 0.0f, 0.0f), vPos) < 2.0f) {
      this->m_aEntityList[i]->SetCanDelete(true);
      switch (this->m_aEntityList[i]->GetModelType()) {
      case CModel::MT_BOMB:
      case CModel::MT_HACK:
      case CModel::MT_HACK2:
        this->m_pRacer->ModBitRate(-this->m_aEntityList[i]->GetValue());
        this->SetFSQ(0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
        break;
      case CModel::MT_DL_PART:
      case CModel::MT_DL_PART2:
        this->m_uPoints += unsigned(this->m_aEntityList[i]->GetValue());
        this->m_pRacer->ModBitRate(20.0f);
        this->SetFSQ(0.8f, glm::vec3(0.0f, 1.0f, 0.0));
      }
    }
    if (this->m_aEntityList[i]->GetPos().y > 4.0f) {
      this->m_aEntityList[i]->SetCanDelete(true);
    }
  }
}

void CRaceTrack::Engine_Shot(float fDT) {
  int i;
  for (i = int(this->m_aShotList.size() - 1); i >= 0; --i) {
    this->m_aShotList[i]->Engine(fDT);
    if (this->m_aShotList[i]->GetPos().y < -100.0f) {
      this->DeleteShot((size_t)i);
      continue;
    }
    if (this->m_aShotList[i]->GetCanDelete())
      this->DeleteShot((size_t)i);
  }
  for (i = 0; i < int(this->m_aShotList.size()); ++i)
    this->m_aShotList[i]->SetRender(&this->m_avShotRenderList[i * 2], &this->m_avShotRenderColorList[i * 2]);
}

void CRaceTrack::ResetGame() {
  m_uPoints = 0;
  m_uDifLevel = DID_VERY_EASY;
  m_uNeedPoints = m_DifficultyLevels[m_uDifLevel]->GetPointsNeeded();
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
  this->m_pRacer->Free();
  this->m_pRacer->Init(CModel::MT_HTTP20);
  Clear();
}

void CRaceTrack::CheckDifLevel() {
  if (m_uNeedPoints == 0)
    return;

  if (m_uPoints < m_uNeedPoints)
    return;

  m_uDifLevel++;
  m_uNeedPoints = m_DifficultyLevels[m_uDifLevel]->GetPointsNeeded();
  m_uFireCount += 1;
  m_fDamage += 1.5f;

  if (m_pRacer->GetModel()->GetModelType() != this->GetLevelModelType()) {
    m_pRacer->Free();
    m_pRacer->Init(this->GetLevelModelType());
  }

  this->SetUpgScreen(10.0f);
}

std::string CRaceTrack::GetDifLevelString() {
  CDifficulty* pDiff = m_DifficultyLevels[m_uDifLevel];
  return pDiff->GetName();
}

unsigned CRaceTrack::GetPoints() {
  return m_uPoints;
}

void CRaceTrack::SetPoints(unsigned uPoints) {
  m_uPoints = uPoints;
}

unsigned CRaceTrack::GetLevelModelType() {
  switch (m_uDifLevel) {
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
  m_IntroState = IS_SKIP;
}

bool CRaceTrack::IsGameOver() {
  return m_bGameOver;
}

bool CRaceTrack::IsGameRuning() {
  return m_bGameRuning;
}

