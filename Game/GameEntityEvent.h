#ifndef __BITRACE_GAMEENTITYEVENT_H__
#define __BITRACE_GAMEENTITYEVENT_H__

class CGameProjectile;
class CGameEntity;
class CGamePlayer;

class CGameEntityEventCondtion {
public:
  enum class CondObject {
    ThisEntity,
    OtherObject,
  };
  enum class CondOperator {
    Is = 0,
    IsNot = 1,
  };
  enum class CondState {
    None,
    Entity,
    Projectile,
    Enemy,
    Player,
    Alive,
    Dead,
  };

  CondObject Object;
  CondOperator Operator;
  CondState State;

  CGameEntityEventCondtion();
};

class CGameEntityEventAction {
public:
  enum class ActionType {
    Damage,
    Heal,
    AddPoints,
    RemPoints,
    Kill,
  };
  enum class ActionTarget {

  };
};

class CGameEntityOnEvent {
public:
  enum class EventType {
    OnSpawn = 0,
    OnDeath,
    OnCollision,
  };
};


#endif // !__BITRACE_GAMEENTITYEVENT_H__

