#include "stdafx.h"
#include "GameObjectEventXml.h"
#include "GameObjectEvent.h"
#include "GameObject.h"
#include "XmlTypes.h"

#include <CBStr/stringex.h>

static const cb::string XML_GAMEOBJEVTACTION_ACTION = L"Type";
static const cb::string XML_GAMEOBJEVTACTION_TARGET = L"Target";
static const cb::string XML_GAMEOBJEVTACTION_VALUE = L"Value";

CB_DEFINEXMLRW(CGameObjectEventAction) {
  RWAttribute(XML_GAMEOBJEVTACTION_TARGET, mObject.Target);
  RWAttribute(XML_GAMEOBJEVTACTION_VALUE, mObject.Value);
  return RWAttribute(XML_GAMEOBJEVTACTION_ACTION, mObject.Action);
}

static const cb::string XML_GAMEOBJEVENT_TRIGGER = L"Trigger";
static const cb::string XML_GAMEOBJEVENT_SENDERIS = L"SenderIs";
static const cb::string XML_GAMEOBJEVENT_ACTION = L"Action";

CB_DEFINEXMLRW(CGameObjectEvent) {
  RWAttribute(XML_GAMEOBJEVENT_SENDERIS, mObject.SenderIs);
  return
    RWAttribute(XML_GAMEOBJEVENT_TRIGGER, mObject.Trigger) &&
    RWNodeList(mObject.Actions, XML_GAMEOBJEVENT_ACTION);
}