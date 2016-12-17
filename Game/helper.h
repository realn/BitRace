#pragma once

#include <sstream>
#include <string>

namespace helper {
  template<typename _Type>
  inline const bool inrange(const _Type& x, const _Type& min, const _Type& max) {
    return x >= min && x <= max;
  }

  template<typename _Type>
  inline const std::string tostr(const _Type& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
  }

  template<typename _Type>
  inline const bool fromstr(const std::string& value, _Type& outResult) {
    std::stringstream ss;
    ss << value;
    ss >> outResult;
    return (bool)ss;
  }

  template<typename _Type>
  inline const _Type fromstr(const std::string& value) {
    _Type result;
    if(fromstr(value, result))
      return result;
    return _Type();
  }

  template<typename _Type>
  inline void deleteobj(_Type*& pObj) {
    if(pObj) {
      delete pObj;
      pObj = nullptr;
    }
  }

  extern const std::string format(const std::string text, ...);
}