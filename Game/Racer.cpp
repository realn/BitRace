#include "Racer.h"
#include "Model.h"

#include <glm/gtc/type_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

float CRacer::s_ConstHeight = 2.5f;
float CRacer::s_MaxBitRate = 100.0f;
float CRacer::s_MaxRotation = 75.0f;

CRacer::CRacer(const Uint32 modelId, CModelRepository* pModelRepo) :
  m_pModel(NULL),
  m_ModelId(modelId),
  m_Rotation(0.0f),
  m_BitRate(100.0f),
  m_Speed(60.0f, 0.0f, 50.0f),
  m_Vec(0.0f, 0.0f, 1.0f),
  m_Color(1.0f)
{
  switch(modelId) {
  default:
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
  };

  m_pModel = pModelRepo->Get((CModel::ModelId)modelId);
}

CRacer::~CRacer() {
}

void CRacer::Update(const float timeDelta) {
  if (glm::abs(this->m_Rotation) > 0.0f)
    this->m_Rotation -= 120.0f * (this->m_Rotation / this->s_MaxRotation) * timeDelta;
  if (glm::abs(this->m_Rotation) > this->s_MaxRotation)
    this->m_Rotation = this->s_MaxRotation * glm::sign(m_Rotation);

  this->m_Vec.x = -this->m_Speed.x * (this->m_Rotation / this->s_MaxRotation) * timeDelta;
}

void CRacer::Render() {
  glPushMatrix();
  glTranslatef(0.0f, s_ConstHeight, 0.0f);
  glRotatef(m_Rotation, 0.0f, 0.0f, 1.0f);
  //m_pModel->Render(m_Color);
  glPopMatrix();
}

void CRacer::ModRotation(const float rotation) {
  m_Rotation -= rotation;
  m_Rotation = glm::clamp(this->m_Rotation, -s_MaxRotation, s_MaxRotation);
}

void CRacer::SetRotation(const float rotation) {
  m_Rotation = rotation;
}

void CRacer::SetColor(const glm::vec4& color) {
  m_Color = color;
}

const glm::vec3 CRacer::GetVec() const {
  return m_Vec;
}

const float CRacer::GetBitRate() const {
  return m_BitRate;
}

void CRacer::ModBitRate(const float bitRate) {
  m_BitRate = glm::clamp(m_BitRate + bitRate, 0.0f, s_MaxBitRate);
}

CModel* CRacer::GetModel() const {
  return m_pModel;
}