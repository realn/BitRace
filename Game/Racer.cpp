#include "Racer.h"

#define GLEW_STATIC
#include <GL/glew.h>

float CRacer::s_fConstHeight = 2.5f;
float CRacer::s_fMaxBitRate = 100.0f;
float CRacer::s_fMaxRotation = 75.0f;

CRacer::CRacer() :
  m_cModel(NULL),
  m_fRotation(0.0f),
  m_fBitRate(100.0f),
  m_Speed(60.0f, 0.0f, 50.0f) 
{

}

CRacer::~CRacer() {
  Free();
}

void CRacer::Free() {
  m_cModel = NULL;
  m_fRotation = 0.0f;
  m_fBitRate = 100.0f;
  m_Speed = glm::vec3(60.0f, 0.0f, 50.0f);
  m_vVec = glm::vec3(0.0f);
}

bool CRacer::Init(unsigned uModelType) {
  Free();

  switch (uModelType) {
  case CModel::MT_HTTP10:
    this->m_Speed = glm::vec3(40.0f, 0.0f, 20.0f);
    break;

  case CModel::MT_HTTP20:
    this->m_Speed = glm::vec3(40.0f, 0.0f, 25.0f);
    break;

  case CModel::MT_P2PGNU:
    this->m_Speed = glm::vec3(50.0f, 0.0f, 30.0f);
    break;

  case CModel::MT_P2PGNU2:
    this->m_Speed = glm::vec3(50.0f, 0.0f, 35.0f);
    break;

  case CModel::MT_P2PFT:
    this->m_Speed = glm::vec3(50.0f, 0.0f, 40.0f);
    break;

  case CModel::MT_P2PFT20:
    this->m_Speed = glm::vec3(55.0f, 0.0f, 50.0f);
    break;

  case CModel::MT_P2PEDK2K:
    this->m_Speed = glm::vec3(60.0f, 0.0f, 55.0f);
    break;

  case CModel::MT_P2PBT:
    this->m_Speed = glm::vec3(70.0f, 0.0f, 70.0f);
    break;

  default:
    return false;
  };

  m_cModel = CModel::GetModel(uModelType);

  return true;
}

void CRacer::Engine(float fDT) {
  if (glm::abs(this->m_fRotation) > 0.0f)
    this->m_fRotation -= 120.0f * (this->m_fRotation / this->s_fMaxRotation) * fDT;
  if (glm::abs(this->m_fRotation) > this->s_fMaxRotation)
    this->m_fRotation = this->s_fMaxRotation * (m_fRotation / fabs(m_fRotation));

  this->m_vVec.x = -this->m_Speed.x * (this->m_fRotation / this->s_fMaxRotation) * fDT;
}

void CRacer::Render() {
  glPushMatrix();
  glTranslatef(0.0f, s_fConstHeight, 0.0f);
  glRotatef(m_fRotation, 0.0f, 0.0f, 1.0f);
  glColor4ubv((GLubyte*)&m_dwColor);
  m_cModel->Render();
  glPopMatrix();
}

void CRacer::ModRotation(float fRotation) {
  m_fRotation -= fRotation;
  m_fRotation = glm::clamp(this->m_fRotation, -s_fMaxRotation, s_fMaxRotation);
}

void CRacer::SetRotation(float fRotation) {
  m_fRotation = fRotation;
}

void CRacer::SetColor(unsigned dwColor) {
  m_dwColor = dwColor;
}

glm::vec3 CRacer::GetVec() const {
  return m_vVec;
}

float CRacer::GetBitRate() const {
  return m_fBitRate;
}

void CRacer::ModBitRate(float fBitRate) {
  if (m_fBitRate <= s_fMaxBitRate)
    m_fBitRate += fBitRate;
  if (m_fBitRate > s_fMaxBitRate)
    m_fBitRate = s_fMaxBitRate;
}

CModel* CRacer::GetModel() {
  return m_cModel;
}