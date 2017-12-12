#include "stdafx.h"
#include "InputBindingsXml.h"
#include <SDL.h>

static const std::map<InputMouseEventId, cb::string> gMouseEventIdMap = {
  {InputMouseEventId::AxisX, L"AxisX"},
  {InputMouseEventId::AxisY, L"AxisY"},
  {InputMouseEventId::AxisZ, L"AxisZ"},
  {InputMouseEventId::ButtonL, L"ButtonLeft"},
  {InputMouseEventId::ButtonM, L"ButtonMiddle"},
  {InputMouseEventId::ButtonR, L"ButtonRight"},
  {InputMouseEventId::ButtonX1, L"ButtonX1"},
  {InputMouseEventId::ButtonX2, L"ButtonX2"},
  {InputMouseEventId::ButtonX3, L"ButtonX3"},
};

static const std::map<InputDeviceType, cb::string> gDeviceTypeMap = {
  {InputDeviceType::Keyboard, L"Keyboard"},
  {InputDeviceType::Mouse, L"Mouse"},
  {InputDeviceType::GamePad, L"GamePad"},
  {InputDeviceType::Touch, L"Touch"},
};

const cb::string toStr(const SDL_Scancode code) {
  cb::charvector text = cb::utf8vec(SDL_GetScancodeName(code));
  return cb::fromUtf8(text);
}

const cb::string toStr(const InputMouseEventId value) {
  return cb::templToStr(gMouseEventIdMap, value, L"ButtonLeft");
}

const cb::string toStr(const InputDeviceType value) {
  return cb::templToStr(gDeviceTypeMap, value, L"Keyboard");
}

const bool fromStr(const cb::string& text, SDL_Scancode& outValue) {
  cb::charvector strCode = cb::toUtf8(text);
  outValue = SDL_GetScancodeFromName(cb::utf8ptr(strCode));
  return outValue != SDL_SCANCODE_UNKNOWN;
}

const bool fromStr(const cb::string& text, InputMouseEventId& outValue) {
  return cb::templFromStr(gMouseEventIdMap, text, outValue);
}

const bool fromStr(const cb::string& text, InputDeviceType& outValue) {
  return cb::templFromStr(gDeviceTypeMap, text, outValue);
}


static const cb::string XML_BINDINGS_BINDING_DEVICETYPE = L"DeviceType";
static const cb::string XML_BINDINGS_BINDING_DEVICEID = L"DeviceId";
static const cb::string XML_BINDINGS_BINDING_EVENTID = L"EventId";
static const cb::string XML_BINDINGS_BINDING_TARGET = L"Target";

CB_DEFINEXMLWRITE(CInputBindings::CBinding) {
  if(!SetAttribute(XML_BINDINGS_BINDING_DEVICETYPE, mObject.DeviceType) ||
     !SetAttribute(XML_BINDINGS_BINDING_TARGET, mObject.Target)) {
    return false;
  }
  if(mObject.DeviceId >= 0) {
    SetAttribute(XML_BINDINGS_BINDING_DEVICEID, mObject.DeviceId);
  }

  bool result;
  switch(mObject.DeviceType) {
  case InputDeviceType::Keyboard: 
    result = SetAttribute(XML_BINDINGS_BINDING_EVENTID, (SDL_Scancode)mObject.EventId);
    break;

  case InputDeviceType::Mouse:
    result = SetAttribute(XML_BINDINGS_BINDING_EVENTID, (InputMouseEventId)mObject.EventId);
    break;

  default:
    result = false;
    break;
  }

  return result;
}

CB_DEFINEXMLREAD(CInputBindings::CBinding) {
  if(!GetAttribute(XML_BINDINGS_BINDING_DEVICETYPE, mObject.DeviceType) ||
     !GetAttribute(XML_BINDINGS_BINDING_TARGET, mObject.Target)) {
    return false;
  }
  if(!GetAttribute(XML_BINDINGS_BINDING_DEVICEID, mObject.DeviceId)) {
    mObject.DeviceId = INPUT_ALL_DEVICES;
  }

  SDL_Scancode code;
  InputMouseEventId mouseId;
  switch(mObject.DeviceType) {
  case InputDeviceType::Keyboard: 
    if(GetAttribute(XML_BINDINGS_BINDING_EVENTID, code)) {
      mObject.EventId = (Uint32)code;
      return true;
    }
    return false;

  case InputDeviceType::Mouse:
    if(GetAttribute(XML_BINDINGS_BINDING_EVENTID, mouseId)) {
      mObject.EventId = (Uint32)mouseId;
      return true;
    }
    return false;

  default:
    return false;
  }
}

static const cb::string XML_BINDINGS_BINDING_ELEM = L"Bind";

CB_DEFINEXMLWRITE(CInputBindings) {
  const CInputBindings::BindVecT& bindList = mObject.GetAllBindings();
  return SetNodeList(bindList, XML_BINDINGS_BINDING_ELEM);
}

CB_DEFINEXMLREAD(CInputBindings) {
  CInputBindings::BindVecT bindList;
  if(!GetNodeList(bindList, XML_BINDINGS_BINDING_ELEM)) {
    return false;
  }

  for(const CInputBindings::CBinding& bind : bindList) {
    mObject.Bind(bind.Target, bind.DeviceType, bind.DeviceId, bind.EventId);
  }
  return true;
}
