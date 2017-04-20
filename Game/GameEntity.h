#ifndef __BITRACE_GAMEENTITY_H__
#define __BITRACE_GAMEENTITY_H__

#include <glm/fwd.hpp>
#include <CBStr/Defines.h>
#include <vector>
#include <map>

#include <CBXml/Serialize.h>

class CModel;
enum class ModelType;

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
  glm::vec2	mVec;
  glm::vec4 mColor;
  float mSpeed;
  float mDamage;
  bool mDeleted;

public:
  CProjectile(const glm::vec2& pos,
              const glm::vec2& vec,
              const glm::vec4& color,
              const float speed,
              const float damage);

  const glm::vec2& GetPos() const;
  const glm::vec2& GetVec() const;
  const float GetDamage() const;
  const bool IsDeleted() const;

  void Update(const float timeDelta);
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

class CGameEntityType {
public:
  cb::string Name;
  EntityType Type;
  glm::vec2 Speed;
  glm::vec4 Color;
  ModelType ModelType;
  float MaxHealth;
  float Damage;
  float AIPause;
  bool IgnoreProjectiles;

public:
  CGameEntityType();
};
typedef std::map<cb::string, CGameEntityType> GameEntityTypeMapT;

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
  bool mIgnoreProjectiles;
  bool mDeleted;

public:
  CGameEntity(const CGameEntityType& type,
              const glm::vec2& pos = glm::vec2(0.0f),
              const float rotAngle = 0.0f);

  const float	GetValue() const;
  const float	GetHealth() const;
  const float GetDamage() const;
  const EntityType	GetType() const;
  const glm::vec2&	GetPos() const;
  const glm::vec2   GetDir() const;
  const glm::vec2   GetVec() const;
  const bool IsDeleted() const;

  const bool	Update(const float timeDelta,
                     const float racerPosX,
                     ProjectileVectorT& projectiles);
  void	Render(const glm::mat4& transform) const;

  void Delete();
};
typedef std::vector<CGameEntity*> GameEntityVectorT;

extern const cb::string toStr(const EntityType type);
extern const bool fromStr(const cb::string& text, EntityType& outType);

CB_DEFINEXMLREAD(CGameEntityType);
CB_DEFINEXMLWRITE(CGameEntityType);
CB_DEFINEXMLREAD(GameEntityTypeMapT);
CB_DEFINEXMLWRITE(GameEntityTypeMapT);

#endif // !__BITRACE_GAMEENTITY_H__

