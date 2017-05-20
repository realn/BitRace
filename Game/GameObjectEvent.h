#ifndef __BITRACE_GAMEOBJECTEVENT_H__
#define __BITRACE_GAMEOBJECTEVENT_H__

#include <SDL_types.h>
#include <glm/fwd.hpp>
#include <vector>

#include <CBStr/Defines.h>

#include "GameObjectDefines.h"

enum class GameObjectType;

class CGameObjectEventAction {
public:
  GameEventActionType   Action;
  GameEventActionTarget Target;
  float Value;

  CGameObjectEventAction();
  CGameObjectEventAction(const GameEventActionType type,
                         const GameEventActionTarget target,
                         const float value = 0.0f);
};

class CGameObjectEvent {
public:
  typedef std::vector<CGameObjectEventAction> ActionVectorT;

public:
  GameEventTrigger Trigger;
  GameObjectType SenderIs;
  ActionVectorT Actions;

  CGameObjectEvent();
  CGameObjectEvent(const GameEventTrigger trigger,
                   const GameObjectType senderIs,
                   const ActionVectorT& actions = ActionVectorT());
};

extern const cb::string toStr(const GameEventTrigger value);
extern const cb::string toStr(const GameEventActionType value);
extern const cb::string toStr(const GameEventActionTarget value);

extern const bool fromStr(const cb::string& text, GameEventTrigger& outValue);
extern const bool fromStr(const cb::string& text, GameEventActionType& outValue);
extern const bool fromStr(const cb::string& text, GameEventActionTarget& outValue);

#endif // !__BITRACE_GAMEOBJECTEVENT_H__

