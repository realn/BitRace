#include "stdafx.h"
#include "RaceTrack.h"
#include "GLDefines.h"
#include "GUI.h"
#include "MeshFunctions.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

//CRaceTrack::CShot::CShot(glm::vec2 vVec, glm::vec2 vStartPos, glm::vec3 vColor, float fDamage) :
//  m_vVec(vVec), m_vPos(vStartPos), m_vColor(vColor), m_fDamage(fDamage), m_bCanDelete(false) {
//
//}
//
//void CRaceTrack::CShot::Engine(float fDT) {
//  if(m_bCanDelete)
//    return;
//
//  m_vPos += m_vVec * fDT;
//}
//
//void CRaceTrack::CShot::SetRender(glm::vec3 *vpLine, glm::vec3 *vpColor) {
//  if(m_bCanDelete)
//    return;
//
//  glm::vec2 t = glm::normalize(m_vVec) * 3.0f + m_vPos;
//  vpLine[0] = glm::vec3(t.x, 0.0f, t.y);
//  vpLine[1] = glm::vec3(m_vPos.x, 0.0f, m_vPos.y);
//
//  vpColor[0] = m_vColor;
//  vpColor[1] = m_vColor;
//}
//
//glm::vec2 CRaceTrack::CShot::GetPos() const {
//  return m_vPos;
//}
//
//glm::vec2 CRaceTrack::CShot::GetVec() const {
//  return m_vVec;
//}
//
//float CRaceTrack::CShot::GetDamage() const {
//  return m_fDamage;
//}
//
//void CRaceTrack::CShot::SetCanDelete(bool bSet) {
//  m_bCanDelete = bSet;
//}
//
//bool CRaceTrack::CShot::GetCanDelete() {
//  return m_bCanDelete;
//}
//
////===========================================
//CRaceTrack::CEntity::CEntity(glm::vec2 vPos, glm::vec2 vVec, glm::vec3 vColor, const ModelType modelType) :
//  m_vPos(vPos), m_vVec(vVec), m_vColor(vColor), m_fTemp(0.0f), mModelType(modelType), m_bCanDelete(false), m_Model(NULL) {
//  if((Uint32)mModelType < 9) {
//    this->m_uType = ET_NONE;
//    return;
//  }
//  switch(mModelType) {
//  default:
//    this->m_uType = ET_NONE;
//    return;
//  case ModelType::MT_DL_PART:
//    this->m_fHealth = 1.0f;
//    this->m_fValue = 100.0f;
//    this->m_uType = ET_BONUS;
//    break;
//  case ModelType::MT_DL_PART2:
//    this->m_fHealth = 1.0f;
//    this->m_fValue = 1000.0f;
//    this->m_uType = ET_BONUS;
//    break;
//  case ModelType::MT_BOMB:
//    this->m_fHealth = 20.0f;
//    this->m_fValue = 15.0f;
//    this->m_uType = ET_ENEMY;
//    break;
//  case ModelType::MT_HACK:
//    this->m_fHealth = 30.0f;
//    this->m_fValue = 30.0f;
//    this->m_uType = ET_ENEMY;
//    break;
//  case ModelType::MT_HACK2:
//    this->m_fHealth = 50.0f;
//    this->m_fValue = 50.0f;
//    this->m_uType = ET_ENEMY;
//    break;
//  };
//  this->m_Model = CModelRepository::Instance.GetModel(mModelType);
//}
//
//bool CRaceTrack::CEntity::Engine(float fDT, float fRacerPosX, CRaceTrack::CShot **aShotList, const unsigned uShotCount) {
//  if(m_bCanDelete)
//    return false;
//
//  this->m_vPos += this->m_vVec * fDT;
//  unsigned i;
//  for(i = 0; i < uShotCount; i++) {
//    if(aShotList[i]->GetCanDelete())
//      continue;
//    if(mModelType != ModelType::MT_BOMB && glm::distance(this->m_vPos, aShotList[i]->GetPos()) < 1.4f) {
//      this->m_fHealth -= aShotList[i]->GetDamage();
//      aShotList[i]->SetCanDelete(true);
//      continue;
//    }
//  }
//  if(m_fHealth <= 0.0f) {
//    this->SetCanDelete(true);
//    if(mModelType != ModelType::MT_DL_PART || mModelType != ModelType::MT_DL_PART2)
//      return true;
//    else return false;
//  }
//  switch(this->m_uType) {
//  default:
//  case ET_NONE:
//    return false;
//  case ET_BONUS:
//    this->m_fTemp += fDT;
//    break;
//  case ET_ENEMY:
//    switch((ModelType)mModelType) {
//    case ModelType::MT_BOMB:
//      this->m_fTemp += 70.0f * fDT;
//      break;
//    case ModelType::MT_HACK:
//      this->m_fTemp += fDT;
//      if(m_fTemp > 0.4f) {
//        m_fTemp = 0.0f;
//        glm::vec2 vRPos = glm::vec2(fRacerPosX, 0.0f);
//        this->m_vVec = glm::normalize(vRPos - this->m_vPos);
//        if(this->m_vVec.y < 0.4f)
//          this->m_vVec.y = 0.4f;
//        this->m_vVec *= 30.0f;
//      }
//      break;
//    case ModelType::MT_HACK2:
//      glm::vec2 vRPos = glm::vec2(fRacerPosX, 0.0f);
//      this->m_vVec = glm::normalize(vRPos - this->m_vPos);
//      this->m_fTemp = glm::degrees(glm::orientedAngle(m_vVec, glm::vec2(0.0f, 1.0f)));
//      if(this->m_vVec.y < 0.4f)
//        this->m_vVec.y = 0.4f;
//      this->m_vVec *= 50.0f;
//      break;
//    };
//    break;
//  };
//  return false;
//}
//
//void CRaceTrack::CEntity::Render(const glm::mat4& transform) const {
//  if(m_bCanDelete)
//    return;
//
//  glm::vec3 rotVec(1.0f, 0.0f, 0.0f);
//  float rotAngle = 0.0f;
//  switch(mModelType) {
//  case ModelType::MT_BOMB:
//    rotAngle = m_fTemp;
//    rotVec = glm::vec3(0.0f, -1.0f, 0.0f);
//    break;
//  case ModelType::MT_HACK:
//    rotAngle = m_fTemp;
//    rotVec = glm::vec3(0.0f, 1.0f, 0.0f);
//    break;
//  case ModelType::MT_HACK2:
//    rotAngle = m_fTemp;
//    rotVec = glm::vec3(0.0f, 1.0f, 0.0f);
//    break;
//  }
//
//  glm::mat4 mat = transform *
//    glm::translate(glm::vec3(m_vPos.x, 3.0f, m_vPos.y)) *
//    glm::rotate(glm::radians(m_fTemp), rotVec);
//
//  glColor3fv(glm::value_ptr(m_vColor));
//  glLoadMatrixf(glm::value_ptr(mat));
//  m_Model->Render(glm::vec4(m_vColor, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
//}
//
//float CRaceTrack::CEntity::GetHealth() {
//  return m_fHealth;
//}
//
//bool CRaceTrack::CEntity::GetCanDelete() {
//  return m_bCanDelete;
//}
//
//const ModelType CRaceTrack::CEntity::GetModelType() const {
//  return mModelType;
//}
//
//unsigned CRaceTrack::CEntity::GetType() {
//  return m_uType;
//}
//
//float CRaceTrack::CEntity::GetValue() {
//  return m_fValue;
//}
//
//glm::vec2 CRaceTrack::CEntity::GetPos() {
//  return m_vPos;
//}
//
//void CRaceTrack::CEntity::SetCanDelete(bool bSet) {
//  m_bCanDelete = bSet;
//}

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
  if(!this->m_SpaceSky.Generate(800.0f, 800.0f, 40, 40, 0.0f))
    return false;
  if(!this->m_SpaceGround.Generate(800.0f, 800.0f, 40, 40, 0.0f)) {
    Free();
    return false;
  }

  return true;
}

void CRaceTrack::Free() {
  this->m_SpaceSky.Free();
  this->m_SpaceGround.Free();
  this->m_vMove = glm::vec2(0.0f);
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
      m_SpaceSky.Render(mat, glm::vec3(0.0f, 1.0f, 0.0f));

      mat = baseMat *
        glm::translate(glm::vec3(0.0f, -20.0f * m_fIntroTime / 2.0f, 0.0f));
      m_SpaceGround.Render(mat, glm::vec3(0.0f, 1.0f, 0.0f));
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
      m_SpaceSky.Render(mat, glm::vec3(0.0f, 1.0f, 0.0f));

      mat = baseMat *
        glm::translate(glm::vec3(0.0f, -20.0f, 0.0f));
      m_SpaceGround.Render(mat, glm::vec3(0.0f, 1.0f, 0.0f));
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
      m_SpaceSky.Render(skyMat, glm::vec3(0.0f, 1.0f, 0.0f));

      glm::mat4 groundMat = moveMat *
        glm::translate(glm::vec3(0.0f, -20.0f, 0.0f));
      m_SpaceGround.Render(groundMat, glm::vec3(0.0f, 1.0f, 0.0f));

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
  this->GenRandomObject();

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
    m_SpaceSky.Render(spaceMat, glm::vec3(0.0f, 1.0f, 0.0f));

    spaceMat *= glm::translate(glm::vec3(0.0f, -40.0f, 0.0f));
    m_SpaceGround.Render(spaceMat, glm::vec3(0.0f, 1.0f, 0.0f));
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

//void CRaceTrack::DeleteShot(size_t i) {
//  if(i >= this->m_aShotList.size())
//    return;
//
//  delete this->m_aShotList[i];
//  this->m_aShotList.erase(this->m_aShotList.begin() + i);
//  this->m_avShotRenderList.pop_back();
//  this->m_avShotRenderList.pop_back();
//  this->m_avShotRenderColorList.pop_back();
//  this->m_avShotRenderColorList.pop_back();
//}

void CRaceTrack::FireWeapon() {
  float fSpeed = 50.0f;
  unsigned i;
  glm::vec2 vStPos = glm::vec2(m_fMoveX, -2.0f);
  glm::vec2 vVec;
  for(i = 0; i < m_uFireCount; ++i) {
    if(m_uFireCount == 1)
      vVec = glm::vec2(0.0f, -1.0f);
    else
      vVec = glm::rotate(glm::vec2(0.0f, -1.0f), glm::radians(-6.0f * float(m_uFireCount - 1) / 2.0f + float(i) * 6.0f));

    mProjectiles.push_back(CProjectile(vStPos, vVec, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), fSpeed, m_fDamage));

    //this->m_avShotRenderList.push_back(glm::vec3());
    //this->m_avShotRenderList.push_back(glm::vec3());
    //this->m_avShotRenderColorList.push_back(glm::vec3());
    //this->m_avShotRenderColorList.push_back(glm::vec3());
  }
}

void CRaceTrack::Clear() {
  for(GameEntityVectorT::iterator it = mEntities.begin(); it != mEntities.end(); it++) {
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
  if(m_uDifLevel < DL_VERY_HARD)
    gui.Print(glm::vec2(200.0f, 5.0f), "NEED POINTS: %u", m_uNeedPoints);
  glColor3f(1.0f, 1.0f, 1.0f);
  gui.Print(glm::vec2(10.0f, 22.0f), "LEVEL: %s", GetDifLevelString().c_str());
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

unsigned CRaceTrack::GetDifLevel() {
  return m_uDifLevel;
}

void CRaceTrack::SetDifLevel(unsigned uDifLevel) {
  m_uDifLevel = uDifLevel;
  if(m_uDifLevel > DL_HOLY_SHIT)
    m_uDifLevel = DL_VERY_HARD;
}

const glm::vec2 CRaceTrack::CreateEntityPosition() {
  float randF = float(rand() % 200 + 1 - 100);
  return glm::vec2(randF / 100.0f * 30.0f + m_fMoveX + (100.0f * this->m_pRacer->GetVec().x), -80.0f);
}

void CRaceTrack::AddEntity(const CGameEntityType& type) {
  CGameEntity* pEntity = new CGameEntity(type, CreateEntityPosition());
  mEntities.push_back(pEntity);
}

void CRaceTrack::AddEntity_DL() {
  CGameEntityType type;
  type.Name = L"DownloadPart";
  type.Color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
  type.Damage = -20.0f;
  type.ModelType = ModelType::MT_DL_PART;
  type.Speed = glm::vec2(0.0f, 50.0f);
  type.Type = EntityType::ET_ITEM;

  AddEntity(type);
}

void CRaceTrack::AddEntity_DL2() {
  CGameEntityType type;
  type.Name = L"Big DownloadPart";
  type.Color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
  type.Damage = -20.0f;
  type.ModelType = ModelType::MT_DL_PART2;
  type.Speed = glm::vec2(0.0f, 40.0f);
  type.Type = EntityType::ET_ITEM;

  AddEntity(type);
}

void CRaceTrack::AddEntity_BOMB() {
  CGameEntityType type;
  type.Name = L"CRC ERR B0MB";
  type.Color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
  type.Damage = 20.0f;
  type.MaxHealth = 20.0f;
  type.ModelType = ModelType::MT_BOMB;
  type.IgnoreProjectiles = true;
  type.Speed = glm::vec2(0.0f, 50.0f);
  type.Type = EntityType::ET_OBSTACLE;

  AddEntity(type);
}

void CRaceTrack::AddEntity_HACK() {
  CGameEntityType type;
  type.Name = L"H4X0R";
  type.Color = glm::vec4(0.5f, 0.2f, 0.8f, 1.0f);
  type.Damage = 20.0f;
  type.MaxHealth = 30.0f;
  type.ModelType = ModelType::MT_HACK;
  type.Speed = glm::vec2(0.0f, 30.0f);
  type.Type = EntityType::ET_ENEMY;
  type.AIPause = 0.4f;

  AddEntity(type);
}

void CRaceTrack::AddEntity_HACK2() {
  CGameEntityType type;
  type.Name = L"733T H4X0R";
  type.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  type.Damage = 50.0f;
  type.MaxHealth = 50.0f;
  type.ModelType = ModelType::MT_HACK2;
  type.Speed = glm::vec2(0.0f, 50.0f);
  type.Type = EntityType::ET_ENEMY;
  type.AIPause = 0.05f;

  AddEntity(type);
}

void CRaceTrack::GenRandomObject() {
  if(m_fTime < 0.6f && m_uDifLevel == DL_VERY_EASY)
    return;
  if(m_fTime < 0.3f && (m_uDifLevel == DL_EASY || m_uDifLevel == DL_MEDIUM))
    return;
  if(m_fTime < 0.22f && m_uDifLevel == DL_HARD)
    return;
  if(m_fTime < 0.13f && m_uDifLevel == DL_VERY_HARD)
    return;
  if(m_fTime < 0.08f && m_uDifLevel == DL_HOLY_SHIT)
    return;
  m_fTime = 0.0f;

  int r = rand() % 100;
  switch(m_uDifLevel) {
  case DL_VERY_EASY:
    if(r < 60)
      this->AddEntity_BOMB();
    if(r >= 60 && r < 90)
      this->AddEntity_DL();
    if(r >= 90)
      this->AddEntity_HACK();
    break;
  case DL_EASY:
    if(r < 50)
      this->AddEntity_BOMB();
    if(r >= 50 && r < 70)
      this->AddEntity_DL();
    if(r >= 70)
      this->AddEntity_HACK();
    break;
  case DL_MEDIUM:
    if(r < 40)
      this->AddEntity_BOMB();
    if(r >= 40 && r < 80)
      this->AddEntity_HACK();
    if(r >= 80 && r < 90)
      this->AddEntity_DL();
    if(r >= 90)
      this->AddEntity_DL2();
    break;
  case DL_HARD:
    if(r < 70)
      this->AddEntity_BOMB();
    if(r >= 70 && r < 80)
      this->AddEntity_HACK();
    if(r >= 80 && r < 90)
      this->AddEntity_HACK2();
    if(r >= 90 && r < 95)
      this->AddEntity_DL();
    if(r >= 90)
      this->AddEntity_DL2();
    break;
  case DL_VERY_HARD:
    if(r < 70)
      this->AddEntity_BOMB();
    if(r >= 70 && r < 95)
      this->AddEntity_HACK2();
    if(r >= 95)
      this->AddEntity_DL2();
    break;
  case DL_HOLY_SHIT:
    if(r < 70)
      this->AddEntity_BOMB();
    else this->AddEntity_HACK2();
    break;
  };
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
  m_pRacer->Free();
  m_pRacer->Init((Uint32)ModelType::MT_HTTP20);
  Clear();
}

void CRaceTrack::CheckDifLevel() {
  if(m_uDifLevel == DL_HOLY_SHIT)
    return;

  if(m_uPoints < m_uNeedPoints)
    return;

  m_uDifLevel++;
  if(m_uDifLevel == DL_VERY_HARD)
    m_uNeedPoints = 1000000;
  else m_uNeedPoints = m_uNeedPoints + m_uNeedPoints * 2;
  m_uFireCount += 1;
  m_fDamage += 1.5f;
  if(m_pRacer->GetModel()->GetType() != this->GetLevelModelType()) {
    m_pRacer->Free();
    m_pRacer->Init((Uint32)this->GetLevelModelType());
  }
  this->SetUpgScreen(10.0f);
}

const std::string CRaceTrack::GetDifLevelString() const {
  switch(m_uDifLevel) {
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

const ModelType CRaceTrack::GetLevelModelType() const {
  switch(m_uDifLevel) {
  case DL_VERY_EASY:
    return ModelType::MT_HTTP20;
  case DL_EASY:
    return ModelType::MT_P2PGNU2;
  case DL_MEDIUM:
    return ModelType::MT_P2PFT20;
  case DL_HARD:
    return ModelType::MT_P2PEDK2K;
  case DL_VERY_HARD:
  case DL_HOLY_SHIT:
    return ModelType::MT_P2PBT;
  };
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
      if(m_uDifLevel == DL_HOLY_SHIT)
        this->m_pRacer->ModBitRate(1.0f);
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

    if((*it)->GetPos().y > 4.0f) {
      (*it)->Delete();
    }

    it++;
  }
}

void CRaceTrack::UpdateProjectiles(const float timeDelta) {
  {
    ProjectileVectorT::iterator it = mProjectiles.begin();
    while(it != mProjectiles.end()) {
      it->Update(timeDelta);

      if(it->IsDeleted() || it->GetPos().y < -100.0f) {
        it = mProjectiles.erase(it);
      }
      else {
        it++;
      }
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

  size_t vertSize = sizeof(CProjectileVertex) / 2;

  const glm::vec3* vertPtr = cb::vectorcastptr<glm::vec3>(mProjectileVertices);
  const glm::vec4* colorPtr = reinterpret_cast<const glm::vec4*>(vertPtr + 1);

  glVertexPointer(3, GL_FLOAT, vertSize, vertPtr);
  glColorPointer(4, GL_FLOAT, vertSize, colorPtr);

  glDrawArrays(GL_LINES, 0, (GLsizei)mProjectileVertices.size() * 2);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}
