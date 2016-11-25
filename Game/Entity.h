#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class CModel;
class CModelRepository;
class CLevel;

enum EntityId {
  EID_UNKNOWN = 0,

  // PLAYER
  EID_HTTP10,
  EID_HTTP20,
  EID_P2PGNU,
  EID_P2PGNU2,
  EID_P2PFT,
  EID_P2PFT20,
  EID_P2PEDK2K,
  EID_P2PBT,

  // BONUS
  EID_DL,
  EID_DL2,

  // ENEMY
  EID_BOMB,
  EID_HACK,
  EID_HACK2
};

enum EntityGroup {
  EG_NONE = 0,
  EG_PLAYER,
  EG_ENEMY,
  EG_OTHER
};

class CEntityType {
private:
  std::string m_Name;
  EntityGroup m_Group;
  glm::vec2 m_Speed;
  glm::vec4 m_Color;
  Uint32  m_ModelId;
  Uint32  m_Points;
  float m_RotSpeed;
  float m_MaxHealth;
  float m_Damage;


public:
  CEntityType(const std::string name,
              const EntityGroup group,
              const glm::vec2& speed,
              const glm::vec4& color,
              const Uint32 modelId,
              const Uint32 points,
              const float rotSpeed,
              const float MaxHealth,
              const float Damage);
  ~CEntityType();

  const std::string GetName() const;
  const EntityGroup GetGroup() const;
  const glm::vec2 GetSpeed() const;
  const glm::vec4 GetColor() const;
  const Uint32 GetModelId() const;
  const Uint32 GetPoints() const;
  const float GetRotSpeed() const;
  const float GetMaxHealth() const;
  const float GetDamage() const;
};


class CEntity {
private:
  CLevel* m_pLevel;
  EntityId  m_EntityId;
  CEntityType*  m_pType;
  CModel*	m_pModel;
  glm::vec2	m_Pos;
  glm::vec2	m_Vec;
  float m_Rotation;
  float	m_Health;
  bool	m_Deleted;
public:
  CEntity(CLevel* pLevel, 
          const glm::vec2& pos, 
          const EntityId entityId, 
          CEntityType* pType, CModelRepository* pModelRepo);
  ~CEntity();

  void Update(const float timeDelta);
  void Render(const glm::mat4& transform);
  void Delete();

  void ModRotation(const float value);
  void SetRotation(const float value);
  void ModHealth(const float value);
  void SetHealth(const float value);

  EntityId  GetId() const;
  CEntityType*  GetType() const;
  const float	GetHealth() const;
  const glm::vec2	GetPos() const;
  const bool IsDeleted() const;
};
