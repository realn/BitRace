#include "RaceTrack.h"

#define GLEW_STATIC
#include <GL/glew.h>

CRaceTrack::CShot::CShot(vec2 vVec, vec2 vStartPos, vec3 vColor, float fDamage) :
  m_vVec(vVec), m_vPos(vStartPos), m_vColor(vColor), m_fDamage(fDamage), m_bCanDelete(false) {

}

void CRaceTrack::CShot::Engine(float fDT) {
  if (m_bCanDelete)
    return;

  m_vPos += m_vVec * fDT;
}

void CRaceTrack::CShot::SetRender(vec3 *vpLine, vec3 *vpColor) {
  if (m_bCanDelete)
    return;

  vec2 t = m_vVec;
  t.Normalize();
  t *= 3.0f;
  t += m_vPos;
  vpLine[0] = vec3(t.X, 0.0f, t.Y);
  vpLine[1] = vec3(m_vPos.X, 0.0f, m_vPos.Y);

  vpColor[0] = m_vColor;
  vpColor[1] = m_vColor;
}
vec2 CRaceTrack::CShot::GetPos() {
  return m_vPos;
}

vec2 CRaceTrack::CShot::GetVec() {
  return m_vVec;
}

float CRaceTrack::CShot::GetDamage() {
  return m_fDamage;
}

void CRaceTrack::CShot::SetCanDelete(bool bSet) {
  m_bCanDelete = bSet;
}

bool CRaceTrack::CShot::GetCanDelete() {
  return m_bCanDelete;
}

//===========================================
CRaceTrack::CEntity::CEntity(vec2 vPos, vec2 vVec, vec3 vColor, unsigned uModelType) :
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
    if (m_uModelType != CModel::MT_BOMB && vec2::LengthSq(this->m_vPos, aShotList[i]->GetPos()) < 1.4f * 1.4f) {
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
        vec2 vRPos = vec2(fRacerPosX, 0.0f);
        this->m_vVec = vRPos - this->m_vPos;
        this->m_vVec.Normalize();
        if (this->m_vVec.Y < 0.4f)
          this->m_vVec.Y = 0.4f;
        this->m_vVec *= 30.0f;
      }
      break;
    case CModel::MT_HACK2:
      vec2 vRPos = vec2(fRacerPosX, 0.0f);
      this->m_vVec = vRPos - this->m_vPos;
      this->m_vVec.Normalize();
      if (this->m_vVec.Y < 0.4f)
        this->m_vVec.Y = 0.4f;
      this->m_vVec *= 50.0f;
      this->m_fTemp = m_vVec.GetAngle();
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

  glTranslatef(m_vPos.X, 3.0f, m_vPos.Y);
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
  glColor3fv(m_vColor.ToFloat());
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

vec2 CRaceTrack::CEntity::GetPos() {
  return m_vPos;
}

void CRaceTrack::CEntity::SetCanDelete(bool bSet) {
  m_bCanDelete = bSet;
}

//===========================================
CRaceTrack::CRaceTrack() :
  m_pRacer(NULL),
  m_fMoveX(0.0f),
  m_fTime(0.0f),
  m_fDamage(15.0f),
  m_fIntroTime(0.0f),
  m_fGameOverTime(0.0f),
  m_fGameOverTime2(0.0f),
  m_uPoints(0),
  m_uFireCount(1),
  m_uDifLevel(DL_VERY_EASY),
  m_uNeedPoints(5000),
  m_uTrackState(TS_INTRO),
  m_unsignedroState(IS_STATE1),
  m_uGameOverCharCount(0),
  m_bGameOver(false),
  m_bGameRuning(false),
  m_strGameOver("GAME OVER") {

}

CRaceTrack::~CRaceTrack() {
  Free();
}

bool CRaceTrack::Init() {
  if (!this->m_SpaceSky.Generate(800.0f, 800.0f, 40, 40, 0.0f))
    return false;
  if (!this->m_SpaceGround.Generate(800.0f, 800.0f, 40, 40, 0.0f)) {
    Free();
    return false;
  }

  return true;
}

void CRaceTrack::Free() {
  this->m_SpaceSky.Free();
  this->m_SpaceGround.Free();
  this->m_vMove = 0.0f;
  this->m_fMoveX = 0.0f;
  m_fTime = 0.0f;
  m_uPoints = 0;
  m_uDifLevel = DL_VERY_EASY;
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

void CRaceTrack::Engine(float fDT) {
  m_bGameRuning = false;
  switch (m_uTrackState) {
  case TS_NONE:
    return;
  case TS_INTRO:
    Engine_Intro(fDT); return;
  case TS_GAME:
    m_bGameRuning = true;
    Engine_Track(fDT); return;
  case TS_GAMEOVER:
    Engine_GameOver(fDT); return;
  }
}

void CRaceTrack::Engine_Intro(float fDT) {
  this->m_fIntroTime += fDT;
  switch (m_unsignedroState) {
  case IS_STATE1:
    if (m_fIntroTime > 0.3f) {
      m_unsignedroState = IS_STATE2;
      m_fIntroTime = 0.0f;
    }
    break;
  case IS_STATE2:
    if (m_fIntroTime > 0.7f) {
      m_unsignedroState = IS_STATE3;
      m_fIntroTime = 0.0f;
      m_vMove.Y = -600.0f;
    }
    break;
  case IS_STATE3:
    this->m_vMove.Y += 120.0f * fDT;
    if (this->m_vMove.Y > 20.0f)
      this->m_vMove.Y -= 20.0f;
    if (m_fIntroTime > 2.0f) {
      m_unsignedroState = IS_STATE4;
      m_fIntroTime = 0.0f;
    }
    break;
  case IS_STATE4:
    this->m_vMove.Y += 120.0f * fDT;
    if (this->m_vMove.Y > 20.0f)
      this->m_vMove.Y -= 20.0f;
    if (m_fIntroTime > 3.0f) {
      m_unsignedroState = IS_ENDSTATE;
      m_fIntroTime = 0.0f;
    }
    break;
  case IS_ENDSTATE:
    this->m_vMove.Y += 120.0f * fDT;
    if (this->m_vMove.Y > 20.0f)
      this->m_vMove.Y -= 20.0f;
    if (m_fIntroTime > 2.0f) {
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

void CRaceTrack::Render_Intro() {
  switch (m_unsignedroState) {
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
    glTranslatef(0.0f, 0.0f, this->m_vMove.Y);

    glPushMatrix();
    glTranslatef(0.0f, 20.0f * m_fIntroTime / 2.0f, 0.0f);
    this->m_SpaceSky.Render(vec3(0.0f, 1.0f, 0.0f));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -20.0f * m_fIntroTime / 2.0f, 0.0f);
    this->m_SpaceGround.Render(vec3(0.0f, 1.0f, 0.0f));
    glPopMatrix();
    glPopMatrix();
    break;
  case IS_STATE4:
    glPushMatrix();
    glTranslatef(0.0f, 12.0f * m_fIntroTime / 3.0f, 0.0f);
    glRotatef(15.0f * m_fIntroTime / 3.0f, 1.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, this->m_vMove.Y);

    glPushMatrix();
    glTranslatef(0.0f, 20.0f, 0.0f);
    this->m_SpaceSky.Render(vec3(0.0f, 1.0f, 0.0f));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -20.0f, 0.0f);
    this->m_SpaceGround.Render(vec3(0.0f, 1.0f, 0.0f));
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
    glTranslatef(0.0f, 0.0f, this->m_vMove.Y);

    glPushMatrix();
    glTranslatef(0.0f, 20.0f, 0.0f);
    this->m_SpaceSky.Render(vec3(0.0f, 1.0f, 0.0f));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -20.0f, 0.0f);
    this->m_SpaceGround.Render(vec3(0.0f, 1.0f, 0.0f));
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

void CRaceTrack::Engine_Track(float fDT) {
  if (m_pRacer == NULL)
    return;

  m_fTime += fDT;
  m_fFSQTime += fDT;
  m_fUpgTime += fDT;
  this->GenRandomObject();

  m_pRacer->Engine(fDT);

  this->m_vMove.X -= m_pRacer->GetVec().X;
  this->m_vMove.Y += 120.0f * fDT;
  this->m_fMoveX += m_pRacer->GetVec().X;

  if (this->m_vMove.Y > 20.0f)
    this->m_vMove.Y -= 20.0f;

  if (this->m_vMove.X > 20.0f)
    this->m_vMove.X -= 20.0f;
  if (this->m_vMove.X < -20.0f)
    this->m_vMove.X += 20.0f;

  this->Engine_Entity(fDT);
  this->Engine_Shot(fDT);
  this->CheckDifLevel();

  if (m_pRacer->GetBitRate() <= 0.0f)
    m_uTrackState = TS_GAMEOVER;
}

void CRaceTrack::Render_Track() {
  glPushMatrix();

  glTranslatef(0.0f, 12.0f, -12.0f);
  glRotatef(15.0f, 1.0f, 0.0f, 0.0f);

  glPushMatrix();
  glTranslatef(this->m_vMove.X, 0.0f, this->m_vMove.Y);
  glTranslatef(0.0f, 20.0f, 0.0f);
  this->m_SpaceSky.Render(vec3(0.0f, 1.0f, 0.0f));
  glTranslatef(0.0f, -40.0f, 0.0f);
  this->m_SpaceGround.Render(vec3(0.0f, 1.0f, 0.0f));
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
  vec2 vStPos = vec2(m_fMoveX, -2.0f);
  vec2 vVec;
  for (i = 0; i < m_uFireCount; ++i) {
    if (m_uFireCount == 1)
      vVec = vec2(0.0f, -1.0f);
    else
      vVec.SetAngle(-6.0f * float(m_uFireCount - 1) / 2.0f + float(i) * 6.0f, -1.0f);

    this->m_aShotList.push_back(new CShot(vVec * fSpeed, vStPos, vec3(0.0f, 0.0f, 1.0f), m_fDamage));
    this->m_avShotRenderList.push_back(vec3());
    this->m_avShotRenderList.push_back(vec3());
    this->m_avShotRenderColorList.push_back(vec3());
    this->m_avShotRenderColorList.push_back(vec3());
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
  if (m_uTrackState == TS_GAMEOVER) {
    if (m_fGameOverTime < 1.0f) {
      glColor4f(1.0f, 1.0f, 1.0f, m_fGameOverTime);
      GUI->RenderFSQuad(glm::vec2(640.0f, 480.0f));
    }
    else {
      if (m_fGameOverTime < 2.0f) {
        float a = 1.0f - (m_fGameOverTime - 1.0f);
        glColor3f(a, a, a);
        GUI->RenderFSQuad(glm::vec2(640.0f, 480.0f));
      }
      else {
        glColor3f(0.0f, 0.0f, 0.0f);
        GUI->RenderFSQuad(glm::vec2(640.0f, 480.0f));
        glColor3f(0.0f, 1.0f, 0.0f);
        GUI->Print(270.0f, 230.0f,
                   m_strGameOver.substr(0, m_uGameOverCharCount) + "_");
      }
    }
    return;
  }
  if (m_uTrackState != TS_GAME)
    return;
  glColor4f(0.4f, 0.4f, 1.0f, 0.6f);
  GUI->RenderProgressBar(vec2(8.0f, 3.0f), vec2(400.0f, 40.0f), 100.0f);
  glColor3f(1.0f, 0.5f, 0.5f);
  GUI->Print(10.0f, 5.0f, "POINTS: %u", m_uPoints);
  if (m_uDifLevel < DL_VERY_HARD)
    GUI->Print(200.0f, 5.0f, "NEED POINTS: %u", m_uNeedPoints);
  glColor3f(1.0f, 1.0f, 1.0f);
  GUI->Print(10.0f, 22.0f, "LEVEL: %s", this->GetDifLevelString().c_str());
  glColor4f(1.0f, 0.0f, 0.0f, 0.6f);
  GUI->RenderProgressBar(vec2(20.0f, 450.0f), vec2(200.0f, 20.0f), this->m_pRacer->GetBitRate());
  if (this->m_fUpgTime < this->m_fUpgTimeOut) {
    glPushMatrix();
    glColor4f(1.0f, 1.0f, 1.0f, (m_fUpgTimeOut - m_fUpgTime) / m_fUpgTimeOut);
    glScalef(2.0f, 2.0f, 2.0f);
    GUI->Print(120.0f, 35.0f, "UPGRADE");
    glPopMatrix();
  }
  if (this->m_fFSQTime < this->m_fFSQTimeOut) {
    glColor4f(m_vFSQColor.X, m_vFSQColor.Y, m_vFSQColor.Z, ((m_fFSQTimeOut - m_fFSQTime) / m_fFSQTimeOut) * 0.5f);
    GUI->RenderFSQuad(glm::vec2(640.0f, 480.0f));
  }
}

unsigned CRaceTrack::GetDifLevel() {
  return m_uDifLevel;
}

void CRaceTrack::SetDifLevel(unsigned uDifLevel) {
  m_uDifLevel = uDifLevel;
  if (m_uDifLevel > DL_HOLY_SHIT)
    m_uDifLevel = DL_VERY_HARD;
}

void CRaceTrack::AddEntity_DL() {
  this->m_aEntityList.push_back(
    new CEntity(vec2(float(rand() % 200 + 1 - 100) / 100.0f * 30.0f + m_fMoveX + (100.0f * this->m_pRacer->GetVec().X), -80.0f),
                vec2(0.0f, 50.0f), vec3(1.0f, 1.0f, 0.0f), CModel::MT_DL_PART));
}

void CRaceTrack::AddEntity_DL2() {
  this->m_aEntityList.push_back(
    new CEntity(vec2(float(rand() % 200 + 1 - 100) / 100.0f * 30.0f + m_fMoveX + (100.0f * this->m_pRacer->GetVec().X), -80.0f),
                vec2(0.0f, 40.0f), vec3(1.0f, 1.0f, 0.0f), CModel::MT_DL_PART2));
}

void CRaceTrack::AddEntity_BOMB() {
  this->m_aEntityList.push_back(
    new CEntity(vec2(float(rand() % 200 + 1 - 100) / 100.0f * 30.0f + m_fMoveX + (100.0f * this->m_pRacer->GetVec().X), -80.0f),
                vec2(0.0f, 50.0f), vec3(1.0f, 0.0f, 1.0f), CModel::MT_BOMB));
}

void CRaceTrack::AddEntity_HACK() {
  this->m_aEntityList.push_back(
    new CEntity(vec2(float(rand() % 200 + 1 - 100) / 100.0f * 30.0f + m_fMoveX + (100.0f * this->m_pRacer->GetVec().X), -80.0f),
                vec2(0.0f, 30.0f), vec3(0.5f, 0.2f, 0.8f), CModel::MT_HACK));
}

void CRaceTrack::AddEntity_HACK2() {
  this->m_aEntityList.push_back(
    new CEntity(vec2(float(rand() % 200 + 1 - 100) / 100.0f * 30.0f + m_fMoveX + (100.0f * this->m_pRacer->GetVec().X), -80.0f),
                vec2(0.0f, 50.0f), vec3(1.0f, 0.0f, 0.0f), CModel::MT_HACK2));
}

void CRaceTrack::GenRandomObject() {
  if (m_fTime < 0.6f && m_uDifLevel == DL_VERY_EASY)
    return;
  if (m_fTime < 0.3f && (m_uDifLevel == DL_EASY || m_uDifLevel == DL_MEDIUM))
    return;
  if (m_fTime < 0.22f && m_uDifLevel == DL_HARD)
    return;
  if (m_fTime < 0.13f && m_uDifLevel == DL_VERY_HARD)
    return;
  if (m_fTime < 0.08f && m_uDifLevel == DL_HOLY_SHIT)
    return;
  m_fTime = 0.0f;

  int r = rand() % 100;
  switch (m_uDifLevel) {
  case DL_VERY_EASY:
    if (r < 60)
      this->AddEntity_BOMB();
    if (r >= 60 && r < 90)
      this->AddEntity_DL();
    if (r >= 90)
      this->AddEntity_HACK();
    break;
  case DL_EASY:
    if (r < 50)
      this->AddEntity_BOMB();
    if (r >= 50 && r < 70)
      this->AddEntity_DL();
    if (r >= 70)
      this->AddEntity_HACK();
    break;
  case DL_MEDIUM:
    if (r < 40)
      this->AddEntity_BOMB();
    if (r >= 40 && r < 80)
      this->AddEntity_HACK();
    if (r >= 80 && r < 90)
      this->AddEntity_DL();
    if (r >= 90)
      this->AddEntity_DL2();
    break;
  case DL_HARD:
    if (r < 70)
      this->AddEntity_BOMB();
    if (r >= 70 && r < 80)
      this->AddEntity_HACK();
    if (r >= 80 && r < 90)
      this->AddEntity_HACK2();
    if (r >= 90 && r < 95)
      this->AddEntity_DL();
    if (r >= 90)
      this->AddEntity_DL2();
    break;
  case DL_VERY_HARD:
    if (r < 70)
      this->AddEntity_BOMB();
    if (r >= 70 && r < 95)
      this->AddEntity_HACK2();
    if (r >= 95)
      this->AddEntity_DL2();
    break;
  case DL_HOLY_SHIT:
    if (r < 70)
      this->AddEntity_BOMB();
    else this->AddEntity_HACK2();
    break;
  };
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
          if (m_uDifLevel == DL_HOLY_SHIT)
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

    vec3 vPos = vec3(this->m_aEntityList[i]->GetPos().X, 0.0f, this->m_aEntityList[i]->GetPos().Y);

    if (vec3::LengthSq(vec3(m_fMoveX, 0.0f, 0.0f), vPos) < 2.0f * 2.0f) {
      this->m_aEntityList[i]->SetCanDelete(true);
      switch (this->m_aEntityList[i]->GetModelType()) {
      case CModel::MT_BOMB:
      case CModel::MT_HACK:
      case CModel::MT_HACK2:
        this->m_pRacer->ModBitRate(-this->m_aEntityList[i]->GetValue());
        this->SetFSQ(0.5f, vec3(1.0f, 0.0f, 0.0f));
        break;
      case CModel::MT_DL_PART:
      case CModel::MT_DL_PART2:
        this->m_uPoints += unsigned(this->m_aEntityList[i]->GetValue());
        this->m_pRacer->ModBitRate(20.0f);
        this->SetFSQ(0.8f, vec3(0.0f, 1.0f, 0.0));
      }
    }
    if (this->m_aEntityList[i]->GetPos().Y > 4.0f) {
      this->m_aEntityList[i]->SetCanDelete(true);
    }
  }
}

void CRaceTrack::Engine_Shot(float fDT) {
  int i;
  for (i = int(this->m_aShotList.size() - 1); i >= 0; --i) {
    this->m_aShotList[i]->Engine(fDT);
    if (this->m_aShotList[i]->GetPos().Y < -100.0f) {
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
  m_uNeedPoints = 5000;
  m_uDifLevel = DL_VERY_EASY;
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
  if (m_uDifLevel == DL_HOLY_SHIT)
    return;

  if (m_uPoints < m_uNeedPoints)
    return;

  m_uDifLevel++;
  if (m_uDifLevel == DL_VERY_HARD)
    m_uNeedPoints = 1000000;
  else m_uNeedPoints = m_uNeedPoints + m_uNeedPoints * 2;
  m_uFireCount += 1;
  m_fDamage += 1.5f;
  if (m_pRacer->GetModel()->GetModelType() != this->GetLevelModelType()) {
    m_pRacer->Free();
    m_pRacer->Init(this->GetLevelModelType());
  }
  this->SetUpgScreen(10.0f);
}

std::string CRaceTrack::GetDifLevelString() {
  switch (m_uDifLevel) {
  case DL_VERY_EASY:
    return "VERY EASY";
  case DL_EASY:
    return "EASY";
  case DL_MEDIUM:
    return "MEDIUM";
  case DL_HARD:
    return "HARD";
  case DL_VERY_HARD:
    return "VERY HARD";
  case DL_HOLY_SHIT:
    return "HOLY SHIT!!!";
  }
  return "";
}

unsigned CRaceTrack::GetPoints() {
  return m_uPoints;
}

void CRaceTrack::SetPoints(unsigned uPoints) {
  m_uPoints = uPoints;
}

unsigned CRaceTrack::GetLevelModelType() {
  switch (m_uDifLevel) {
  case DL_VERY_EASY:
    return CModel::MT_HTTP20;
  case DL_EASY:
    return CModel::MT_P2PGNU2;
  case DL_MEDIUM:
    return CModel::MT_P2PFT20;
  case DL_HARD:
    return CModel::MT_P2PEDK2K;
  case DL_VERY_HARD:
  case DL_HOLY_SHIT:
    return CModel::MT_P2PBT;
  };
  return CModel::MT_HTTP20;
}

void CRaceTrack::SetFSQ(float fTimeOut, vec3 vColor) {
  this->m_fFSQTime = 0.0f;
  this->m_fFSQTimeOut = fTimeOut;
  this->m_vFSQColor = vColor;
}

void CRaceTrack::SetUpgScreen(float fTimeOut) {
  this->m_fUpgTime = 0.0f;
  this->m_fUpgTimeOut = fTimeOut;
  this->SetFSQ(fTimeOut / 4.0f, vec3(1.0f, 1.0f, 0.0f));
}

void CRaceTrack::SkipIntro() {
  m_unsignedroState = IS_SKIP;
}

bool CRaceTrack::IsGameOver() {
  return m_bGameOver;
}

bool CRaceTrack::IsGameRuning() {
  return m_bGameRuning;
}