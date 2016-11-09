#include "Racer.h"

float CRacer::s_fConstHeight = 2.5f;
float CRacer::s_fMaxBitRate = 100.0f;
float CRacer::s_fMaxRotation = 75.0f;

CRacer::CRacer() :
  m_cModel(NULL),
  m_fRotation(0.0f),
  m_fBitRate(100.0f),
  m_fXSpeed(60.0f),
  m_fZSpeed(50.0f) {

}

CRacer::~CRacer() {
  Free();
}

void CRacer::Free() {
  m_cModel = NULL;
  m_fRotation = 0.0f;
  m_fBitRate = 100.0f;
  m_fXSpeed = 60.0f;
  m_fZSpeed = 50.0f;
  m_vVec = 0.0f;
}

bool CRacer::Init(UINT uModelType) {
  Free();

  switch (uModelType) {
  case CModel::MT_HTTP10:
    this->m_fZSpeed = 20.0f;
    this->m_fXSpeed = 40.0f;
    break;

  case CModel::MT_HTTP20:
    this->m_fZSpeed = 25.0f;
    this->m_fXSpeed = 40.0f;
    break;

  case CModel::MT_P2PGNU:
    this->m_fZSpeed = 30.0f;
    this->m_fXSpeed = 50.0f;
    break;

  case CModel::MT_P2PGNU2:
    this->m_fZSpeed = 35.0f;
    this->m_fXSpeed = 50.0f;
    break;

  case CModel::MT_P2PFT:
    this->m_fZSpeed = 40.0f;
    this->m_fXSpeed = 50.0f;
    break;

  case CModel::MT_P2PFT20:
    this->m_fZSpeed = 50.0f;
    this->m_fXSpeed = 55.0f;
    break;

  case CModel::MT_P2PEDK2K:
    this->m_fZSpeed = 55.0f;
    this->m_fXSpeed = 60.0f;
    break;

  case CModel::MT_P2PBT:
    this->m_fZSpeed = 70.0f;
    this->m_fXSpeed = 70.0f;
    break;
  default:
    return false;
  };
  m_cModel = CModel::GetModel(uModelType);

  return true;
}

void CRacer::Engine(float fDT) {
  if (fabsf(this->m_fRotation) > 0.0f)
    this->m_fRotation -= 120.0f * (this->m_fRotation / this->s_fMaxRotation) * fDT;
  if (fabsf(this->m_fRotation) > this->s_fMaxRotation)
    this->m_fRotation = this->s_fMaxRotation * (m_fRotation / fabs(m_fRotation));

  this->m_vVec.X = -this->m_fXSpeed * (this->m_fRotation / this->s_fMaxRotation) * fDT;
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
  if (m_fRotation > this->s_fMaxRotation)
    m_fRotation = this->s_fMaxRotation;
  if (m_fRotation < -this->s_fMaxRotation)
    m_fRotation = -this->s_fMaxRotation;
}

void CRacer::SetRotation(float fRotation) {
  m_fRotation = fRotation;
}

void CRacer::SetColor(DWORD dwColor) {
  m_dwColor = dwColor;
}

vec3 CRacer::GetVec() {
  return m_vVec;
}

float CRacer::GetBitRate() {
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