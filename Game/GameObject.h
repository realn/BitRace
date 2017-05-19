#ifndef __BITRACE_GAMEOBJECT_H__
#define __BITRACE_GAMEOBJECT_H__

#include <SDL_types.h>
#include <glm/fwd.hpp>

enum class GameObjectType;
enum class GameEventTrigger;

class CGameObjectEvent;

class CGameObject {
public:
  typedef std::vector<CGameObjectEvent> EventVectT;

protected:
  GameObjectType mType;
  glm::vec2 mPos;
  glm::vec2 mDir;
  glm::vec2 mSpeed;
  glm::vec4 mColor;
  bool mIsDeleted;

public:
  CGameObject(const GameObjectType type,
              const glm::vec2& pos,
              const glm::vec2& dir,
              const glm::vec2& speed,
              const glm::vec4& color);
  virtual ~CGameObject();

  void Delete();

  const GameObjectType GetType() const;
  const glm::vec2& GetPos() const;
  const glm::vec2& GetDir() const;
  const glm::vec2& GetSpeed() const;
  const glm::vec2 GetVec() const;
  const glm::vec4& GetColor() const;
  const bool IsDeleted() const;

  virtual const EventVectT GetEvents() const;
  virtual const EventVectT GetEvents(const GameEventTrigger trigger, 
                                     const GameObjectType senderType) const;
};

extern const cb::string toStr(const GameObjectType value);
extern const bool fromStr(const cb::string& text, GameObjectType& outValue);

#endif // !__BITRACE_GAMEOBJECT_H__

