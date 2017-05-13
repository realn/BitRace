#include "stdafx.h"
#include "Racer.h"
#include "GLDefines.h"

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

bool CRacer::Init(unsigned uModelType, CModelRepository& modelRepo, const cb::string& filename) {
  Free();

  switch ((ModelType)uModelType) {
  case ModelType::MT_HTTP10:
    this->m_Speed = glm::vec3(40.0f, 0.0f, 20.0f);
    break;

  case ModelType::MT_HTTP20:
    this->m_Speed = glm::vec3(40.0f, 0.0f, 25.0f);
    break;

  case ModelType::MT_P2PGNU:
    this->m_Speed = glm::vec3(50.0f, 0.0f, 30.0f);
    break;

  case ModelType::MT_P2PGNU2:
    this->m_Speed = glm::vec3(50.0f, 0.0f, 35.0f);
    break;

  case ModelType::MT_P2PFT:
    this->m_Speed = glm::vec3(50.0f, 0.0f, 40.0f);
    break;

  case ModelType::MT_P2PFT20:
    this->m_Speed = glm::vec3(55.0f, 0.0f, 50.0f);
    break;

  case ModelType::MT_P2PEDK2K:
    this->m_Speed = glm::vec3(60.0f, 0.0f, 55.0f);
    break;

  case ModelType::MT_P2PBT:
    this->m_Speed = glm::vec3(70.0f, 0.0f, 70.0f);
    break;

  default:
    return false;
  };

  m_cModel = modelRepo.GetModel(filename);

  return true;
}

void CRacer::Engine(float fDT) {
  if (glm::abs(this->m_fRotation) > 0.0f)
    this->m_fRotation -= 120.0f * (this->m_fRotation / this->s_fMaxRotation) * fDT;
  if (glm::abs(this->m_fRotation) > this->s_fMaxRotation)
    this->m_fRotation = this->s_fMaxRotation * (m_fRotation / fabs(m_fRotation));

  this->m_vVec.x = -this->m_Speed.x * (this->m_fRotation / this->s_fMaxRotation);
}

void CRacer::Render(const glm::mat4& transform) const {
  glm::mat4 mat =
    transform *
    glm::rotate(glm::radians(m_fRotation), glm::vec3(0.0f, 0.0f, 1.0f));

  glLoadMatrixf(glm::value_ptr(mat));
  m_cModel->Render(mColor, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void CRacer::ModRotation(float fRotation) {
  m_fRotation -= fRotation;
  m_fRotation = glm::clamp(this->m_fRotation, -s_fMaxRotation, s_fMaxRotation);
}

void CRacer::SetRotation(float fRotation) {
  m_fRotation = fRotation;
}

void CRacer::SetColor(const glm::vec4& color) {
  mColor = color;
}

glm::vec3 CRacer::GetVec() const {
  return glm::vec3(m_vVec.x, 0.0f, 120.0f);
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