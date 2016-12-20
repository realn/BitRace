#pragma once

#include <sstream>
#include <string>
#include <map>
#include <set>
#include <vector>

class CFile;

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

  template<typename _Key, typename _Value>
  inline void deletemap(std::map<_Key, _Value>& valmap) {
    for(std::map<_Key, _Value>::iterator it = valmap.begin(); it != valmap.end(); it++) {
      delete it->second;
    }
    valmap.clear();
  }

  template<typename _Type>
  inline void deleteset(std::set<_Type>& valset) {
    for(std::set<_Type>::iterator it = valset.begin(); it != valset.end(); it++) {
      delete *it;
    }
    valset.clear();
  }

  template<typename _Type>
  inline void deletevector(std::vector<_Type>& valvec) {
    for(std::vector<_Type>::iterator it = valvec.begin(); it != valvec.end(); it++) {
      delete *it;
    }
    valvec.clear();
  }

  extern const std::string format(const std::string text, ...);
  extern void writestr(CFile& file, const std::string& text);
  extern void readstr(CFile& file, std::string& outText);
}
