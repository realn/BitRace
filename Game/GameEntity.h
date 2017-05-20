#ifndef __BITRACE_GAMEENTITY_H__
#define __BITRACE_GAMEENTITY_H__

#include <glm/fwd.hpp>
#include <CBStr/Defines.h>
#include <vector>
#include <map>

#include "GameObject.h"
#include "GameObjectEvent.h"
#include "GameActor.h"

class CModel;
class CModelRepository;
class IFileSystem;

enum class EntityType {
  None = 0,
  Item,
  Obstacle,
  Enemy
};

class CGameEntityType {
public:
  typedef std::map<cb::string, CGameEntityType> TypeMapT;
  typedef std::vector<CGameObjectEvent> EventVecT;

  cb::string Name;
  EntityType Type;
  glm::vec2 Speed;
  glm::vec4 Color;
  cb::string ModelFile;
  float MaxHealth;
  float AIPause;
  float RotSpeed;
  bool IgnoreProjectiles;
  EventVecT Events;

public:
  CGameEntityType();

  static const bool Save(const TypeMapT& typeMap,
                         IFileSystem& fs,
                         const cb::string& filepath);
  static const bool Load(TypeMapT& typeMap,
                         IFileSystem& fs,
                         const cb::string& filepath);
};

class CGameEntity 
  : public CGameActor
{
public:
  typedef std::vector<CGameObjectEvent> EventVecT;

private:
  EntityType mEntityType;
  float mAIPause;
  float mAITime;
  float mRotAngle;
  float mRotSpeed;
  bool mIgnoreProjectiles;
  EventVecT mEvents;

public:
  CGameEntity(const CGameEntityType& type,
              const glm::vec2& pos = glm::vec2(0.0f),
              const float rotAngle = 0.0f);
  virtual ~CGameEntity();

  const EntityType	GetEntityType() const;
  const bool GetIgnoreProjectiles() const;
  const EventVecT GetEvents() const override;

  void Update(const glm::vec2& playerVec, const float timeDelta);
  void Render(const glm::mat4& transform) const;
};
typedef std::vector<CGameEntity*> GameEntityVectorT;

extern const cb::string toStr(const EntityType type);

extern const bool fromStr(const cb::string& text, EntityType& outType);

#endif // !__BITRACE_GAMEENTITY_H__

