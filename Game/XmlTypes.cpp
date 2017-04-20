#include "stdafx.h"
#include "XmlTypes.h"

#include <CBXml/Serialize.h>
#include <CBStr/StringEx.h>

namespace cb {
  const cb::string toStr(const glm::vec2 & value) {
    return cb::format(L"{0}:{1}", value.x, value.y);
  }

  const cb::string toStr(const glm::vec3 & value) {
    return cb::format(L"{0}:{1}:{2}", value.x, value.y, value.z);
  }

  const cb::string toStr(const glm::vec4 & value) {
    return cb::format(L"{0}:{1}:{2}:{3}", value.x, value.y, value.z, value.w);
  }

  const bool fromStr(const cb::string & text, glm::vec2 & outValue) {
    cb::strvector list = cb::split(text, L":");
    if(list.size() != 2)
      return false;

    return
      cb::fromStr(list[0], outValue.x) &&
      cb::fromStr(list[1], outValue.y);
  }

  const bool fromStr(const cb::string & text, glm::vec3 & outValue) {
    cb::strvector list = cb::split(text, L":");
    if(list.size() != 3)
      return false;

    return
      cb::fromStr(list[0], outValue.x) &&
      cb::fromStr(list[1], outValue.y) &&
      cb::fromStr(list[2], outValue.z);
  }

  const bool fromStr(const cb::string & text, glm::vec4 & outValue) {
    cb::strvector list = cb::split(text, L":");
    if(list.size() != 4)
      return false;

    return
      cb::fromStr(list[0], outValue.x) &&
      cb::fromStr(list[1], outValue.y) &&
      cb::fromStr(list[2], outValue.z) &&
      cb::fromStr(list[3], outValue.w);
  }
}

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

