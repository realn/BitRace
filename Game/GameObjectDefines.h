#ifndef __BITRACE_GAMEOBJECTEVENTDEFINES_H__
#define __BITRACE_GAMEOBJECTEVENTDEFINES_H__

enum class GameObjectType {
  Unknown = 0,
  Player,
  Entity,
  Projectile,
  All
};

enum class GameEventTrigger {
  None = 0,
  OnCollision,
  OnDeath,
};

enum class GameEventActionType {
  None = 0,
  AddPoints,
  RemPoints,
  Heal,
  Damage,
  Kill,
};

enum class GameEventActionTarget {
  None = 0,
  This,
  Sender,
  Player
};

class CGameObject;

class IGameObjectEventObserver {
public:
  virtual ~IGameObjectEventObserver() {}

  virtual void TriggerEvent(const GameEventTrigger triggerType,
                            CGameObject& thisObj,
                            CGameObject* pSenderObj = nullptr) = 0;
};


#endif // !__BITRACE_GAMEOBJECTEVENTDEFINES_H__

