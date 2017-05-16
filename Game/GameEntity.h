#ifndef __BITRACE_GAMEENTITY_H__
#define __BITRACE_GAMEENTITY_H__

#include <glm/fwd.hpp>
#include <CBStr/Defines.h>
#include <vector>
#include <map>

class CModel;
class CModelRepository;
class IFileSystem;

class CProjectileVertex {
public:
  glm::vec3 StartPos;
  glm::vec4 StartColor;
  glm::vec3 EndPos;
  glm::vec4 EndColor;
};
typedef std::vector<CProjectileVertex> ProjectileVertexVectorT;

class CProjectile {
private:
  glm::vec2 mPos;
  glm::vec2	mDir;
  glm::vec4 mColor;
  float mSpeed;
  float mDamage;
  bool mDeleted;

public:
  CProjectile(const glm::vec2& pos,
              const glm::vec2& dir,
              const glm::vec4& color,
              const float speed,
              const float damage);

  const glm::vec2& GetPos() const;
  const glm::vec2& GetDir() const;
  const float GetSpeed() const;
  const float GetDamage() const;
  const float GetLength() const;
  const bool IsDeleted() const;

  void Update(const glm::vec2& playerVec, const float timeDelta);
  void UpdateRender(CProjectileVertex& outVertex);

  void Delete();
};
typedef std::vector<CProjectile> ProjectileVectorT;

enum class EntityType {
  ET_NONE = 0,
  ET_ITEM,
  ET_OBSTACLE,
  ET_ENEMY
};

enum class EntityEvent {
  EE_ONDEATH = 0,
  EE_ONENTITYCOLLISION,
  EE_ONPROJECTILECOLLISION,
};

enum class EntityEventAction {
  EEA_NONE = 0,
  EEA_KILLSELF,
  EEA_KILLENTITY,
  EEA_ADDPOINTS,
  EEA_REMPOINTS,
  EEA_DAMAGESELF,
  EEA_DAMAGEENTITY,
  EEA_HEALSELF,
  EEA_HEALENTITY,
};

typedef std::vector<EntityEventAction> EntityEventActionVecT;
typedef std::map<EntityEvent, EntityEventActionVecT> EntityEventMapT;

class CGameEntityType {
public:
  typedef std::map<cb::string, CGameEntityType> TypeMapT;

  cb::string Name;
  EntityType Type;
  glm::vec2 Speed;
  glm::vec4 Color;
  cb::string ModelFile;
  float MaxHealth;
  float Damage;
  float AIPause;
  float RotSpeed;
  bool IgnoreProjectiles;
  Uint32 Points;
  EntityEventMapT EventMap;

public:
  CGameEntityType();

  static const bool Save(const TypeMapT& typeMap,
                         IFileSystem& fs,
                         const cb::string& filepath);
  static const bool Load(TypeMapT& typeMap,
                         IFileSystem& fs,
                         const cb::string& filepath);
};

class CGameEntity {
private:
  cb::string mName;
  EntityType mType;
  glm::vec2 mPos;
  glm::vec2 mSpeed;
  glm::vec4 mColor;
  CModel* mModel;
  float mMaxHealth;
  float mHealth;
  float mDamage;
  float mAIPause;
  float mAITime;
  float mRotAngle;
  float mRotSpeed;
  float mCollRadius;
  bool mIgnoreProjectiles;
  bool mDeleted;
  Uint32 mPoints;
  EntityEventMapT mEventMap;

public:
  CGameEntity(const CGameEntityType& type,
              CModelRepository& modelRepo,
              const glm::vec2& pos = glm::vec2(0.0f),
              const float rotAngle = 0.0f);

  void ModHealth(const float value);

  const float	GetValue() const;
  const float	GetHealth() const;
  const float GetDamage() const;
  const float GetCollRadius() const;
  const EntityType	GetType() const;
  const glm::vec2&	GetPos() const;
  const glm::vec2   GetDir() const;
  const glm::vec2   GetVec() const;
  const bool GetIgnoreProjectiles() const;
  const bool IsDeleted() const;
  const Uint32 GetPoints() const;
  const EntityEventMapT& GetEventMap() const;

  void Update(const glm::vec2& playerVec, const float timeDelta);
  void Render(const glm::mat4& transform) const;

  void Delete();
};
typedef std::vector<CGameEntity*> GameEntityVectorT;

extern const cb::string toStr(const EntityType type);
extern const cb::string toStr(const EntityEvent event);
extern const cb::string toStr(const EntityEventAction action);

extern const bool fromStr(const cb::string& text, EntityType& outType);
extern const bool fromStr(const cb::string& text, EntityEvent& outEvent);
extern const bool fromStr(const cb::string& text, EntityEventAction& outAction);

#endif // !__BITRACE_GAMEENTITY_H__

