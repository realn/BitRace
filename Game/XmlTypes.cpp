#include "stdafx.h"
#include "XmlTypes.h"

#include  <CBXml/Serialize.h>

static const cb::string XML_VEC_X = L"X";
static const cb::string XML_VEC_Y = L"Y";

CB_DEFINEXMLWRITE(glm::uvec2) {
  return
    SetAttribute(XML_VEC_X, mObject.x) &&
    SetAttribute(XML_VEC_Y, mObject.y);
}

CB_DEFINEXMLREAD(glm::uvec2) {
  return
    GetAttribute(XML_VEC_X, mObject.x) &&
    GetAttribute(XML_VEC_Y, mObject.y);
}

CB_DEFINEXMLWRITE(glm::vec2) {
  return
    SetAttribute(XML_VEC_X, mObject.x) &&
    SetAttribute(XML_VEC_Y, mObject.y);
}

CB_DEFINEXMLREAD(glm::vec2) {
  return
    GetAttribute(XML_VEC_X, mObject.x) &&
    GetAttribute(XML_VEC_Y, mObject.y);
}
