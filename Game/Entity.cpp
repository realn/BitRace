#include "Entity.h"
#include "Model.h"
#include "Level.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>


CEntityType::CEntityType(const std::string name,
                                     const EntityGroup group,
                                     const glm::vec2 & speed,
                                     const glm::vec4 & color,
                                     const Uint32 modelId,
                                     const Uint32 points,
                                     const float rotSpeed,
                                     const float maxHealth,
                                     const float damage) :
  m_Name(name),
  m_Group(group),
  m_Speed(speed),
  m_Color(color),
  m_ModelId(modelId),
  m_Points(points),
  m_RotSpeed(rotSpeed),
  m_MaxHealth(maxHealth),
  m_Damage(damage) {}

CEntityType::~CEntityType() {}

const std::string CEntityType::GetName() const {
  return m_Name;
}

const EntityGroup CEntityType::GetGroup() const {
  return m_Group;
}

const glm::vec2 CEntityType::GetSpeed() const {
  return m_Speed;
}

const glm::vec4 CEntityType::GetColor() const {
  return m_Color;
}

const Uint32 CEntityType::GetModelId() const {
  return m_ModelId;
}

const Uint32 CEntityType::GetPoints() const {
  return m_Points;
}

const float CEntityType::GetRotSpeed() const {
  return m_RotSpeed;
}

const float CEntityType::GetMaxHealth() const {
  return m_MaxHealth;
}

const float CEntityType::GetDamage() const {
  return m_Damage;
}


//===================================================================================================


CEntity::CEntity(CLevel* pLevel, glm::vec2 pos, const EntityId entityId, CEntityType* pType, CModelRepository* pModelRepo) :
  m_pLevel(pLevel),
  m_EntityId(entityId),
  m_pType(pType),
  m_pModel(nullptr),
  m_Pos(pos),
  m_Vec(0.0f, 1.0f),
  m_Rotation(0.0f),
  m_Health(0.0f),
  m_Deleted(false) 
{
  m_pModel = pModelRepo->Get((CModel::ModelId)m_pType->GetModelId());
  m_Health = m_pType->GetMaxHealth();
}

CEntity::~CEntity() {

}

void CEntity::Update(const float timeDelta) {
  if (m_Deleted)
    return;

  if (m_Health <= 0.0f) {
    this->Delete();
    return;
  }

  m_Pos += this->m_Vec * m_pType->GetSpeed() * timeDelta;

  float rotSpeed = this->m_pType->GetRotSpeed();

  switch (m_pType->GetGroup()) {
  default:
  case EG_NONE:
    return;

  case EG_PLAYER:
    if (glm::abs(this->m_Rotation) > 0.0f)
      m_Rotation -= rotSpeed * (m_Rotation / rotSpeed) * timeDelta;
    m_Rotation = glm::clamp(m_Rotation, -rotSpeed, rotSpeed);
    m_Vec.x = -(m_Rotation / rotSpeed);
    return;

  case EG_OTHER:
    m_Rotation = glm::mod(m_Rotation + rotSpeed * timeDelta, 360.0f);
    return;

  case EG_ENEMY:
    {
      glm::vec2 racerPos = glm::vec2(m_pLevel->GetPlayer()->GetPos().x, 0.0f);
      switch(m_EntityId) {
      default:
        return;
      case EID_BOMB:
        m_Rotation = glm::mod(m_Rotation + m_pType->GetRotSpeed() * timeDelta, 360.0f);
        return;

      case EID_HACK:
        this->m_Vec = glm::normalize(racerPos - m_Pos);
        this->m_Rotation = glm::degrees(glm::orientedAngle(m_Vec, glm::vec2(0.0f, 1.0f)));
        this->m_Vec.y = glm::max(m_Vec.y, 0.4f);
        return;

      case EID_HACK2:
        this->m_Vec = glm::normalize(racerPos - m_Pos);
        this->m_Rotation = glm::degrees(glm::orientedAngle(m_Vec, glm::vec2(0.0f, 1.0f)));
        this->m_Vec.y = glm::max(m_Vec.y, 0.4f);
        return;
      }
    }
    return;
  }
}

void CEntity::Render(const glm::mat4& transform) {
  if (m_Deleted)
    return;

  glm::mat4 mat = transform;

  if (m_pType->GetGroup() == EG_PLAYER) {
    mat *=
      glm::rotate(glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
  }
  else {
    const float playerPosX = this->m_pLevel->GetPlayer()->GetPos().x;
    mat *=
      glm::translate(glm::vec3(m_Pos.x - playerPosX, 0.0f, m_Pos.y)) *
      glm::rotate(glm::radians(m_Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
  }

  m_pModel->Render(mat, m_pType->GetColor());
}

void CEntity::Delete() {
  m_Deleted = true;
}

void CEntity::ModRotation(const float value) {
  m_Rotation += value;
}

void CEntity::SetRotation(const float value) {
  m_Rotation = value;
}

void CEntity::ModHealth(const float value) {
  m_Health = glm::clamp(m_Health + value, 0.0f, m_pType->GetMaxHealth());
}

void CEntity::SetHealth(const float value) {
  m_Health = glm::clamp(value, 0.0f, m_pType->GetMaxHealth());
}

const float CEntity::GetHealth() const {
  return m_Health;
}

const glm::vec2 CEntity::GetPos() const {
  return m_Pos;
}

const bool CEntity::IsDeleted() const {
  return m_Deleted;
}

EntityId CEntity::GetId() const {
  return m_EntityId;
}

CEntityType * CEntity::GetType() const {
  return m_pType;
}
