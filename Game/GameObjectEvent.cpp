#include "stdafx.h"
#include "GameObjectEvent.h"
#include "GameObject.h"

CGameObjectEventAction::CGameObjectEventAction() 
  : Action(GameEventActionType::None)
  , Target(GameEventActionTarget::None)
  , Value(0.0f)
{}

CGameObjectEvent::CGameObjectEvent() 
  : Trigger(GameEventTrigger::None)
  , SenderIs(GameObjectType::Unknown)
{}

static std::map<GameEventTrigger, cb::string> gEventTriggerNameMap = {
  {GameEventTrigger::None, L"None"},
  {GameEventTrigger::OnCollision, L"Collision"},
  {GameEventTrigger::OnDeath, L"Death"}
};

static std::map<GameEventActionType, cb::string> gEventActionTypeNameMap = {
  {GameEventActionType::None, L"None"},
  {GameEventActionType::AddPoints, L"AddPoints"},
  {GameEventActionType::RemPoints, L"RemPoints"},
  {GameEventActionType::Damage, L"Damage"},
  {GameEventActionType::Heal, L"Heal"},
  {GameEventActionType::Kill, L"Kill"}
};

static std::map<GameEventActionTarget, cb::string> gEventActionTargetNameMap = {
  {GameEventActionTarget::None, L"None"},
  {GameEventActionTarget::This, L"This"},
  {GameEventActionTarget::Sender, L"Sender"}
};

const cb::string toStr(const GameEventTrigger value) {
  return cb::templToStr(gEventTriggerNameMap, value, L"None");
}

const cb::string toStr(const GameEventActionType value) {
  return cb::templToStr(gEventActionTypeNameMap, value, L"None");
}

const cb::string toStr(const GameEventActionTarget value) {
  return cb::templToStr(gEventActionTargetNameMap, value, L"None");
}

const bool fromStr(const cb::string & text, GameEventTrigger & outValue) {
  return cb::templFromStr(gEventTriggerNameMap, text, outValue);
}

const bool fromStr(const cb::string & text, GameEventActionType & outValue) {
  return cb::templFromStr(gEventActionTypeNameMap, text, outValue);
}

const bool fromStr(const cb::string & text, GameEventActionTarget & outValue) {
  return cb::templFromStr(gEventActionTargetNameMap, text, outValue);
}


