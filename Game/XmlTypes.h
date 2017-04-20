#ifndef __BITRACE_XMLTYPES_H__
#define __BITRACE_XMLTYPES_H__

#include <glm/fwd.hpp>
#include <CBStr/Defines.h>

namespace cb {
  extern const string toStr(const glm::vec2& value);
  extern const string toStr(const glm::vec3& value);
  extern const string toStr(const glm::vec4& value);

  extern const bool fromStr(const string& text, glm::vec2& outValue);
  extern const bool fromStr(const string& text, glm::vec3& outValue);
  extern const bool fromStr(const string& text, glm::vec4& outValue);
}

#endif // !__BITRACE_XMLTYPES_H__

