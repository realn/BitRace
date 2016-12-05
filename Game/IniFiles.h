#pragma once

#include <SDL_types.h>
#include <string>
#include <vector>
#include <map>

class CIniFile {
private:
  typedef std::map<std::string, std::string> valuemap;
  class CSection {
  public:
    valuemap Values;

    CSection() {}
    CSection(const std::string& name, const std::string& value) {
      Values[name] = value;
    }
  };
  typedef std::map<std::string, CSection> sectionmap;
  sectionmap m_Sections;

public:
  CIniFile();
  CIniFile(const std::string& filename);
  ~CIniFile();

  const bool Load(const std::string& filename);
  const bool Save(const std::string& filename);
  void Clear();

  void Write(const std::string& section, const std::string& var, const std::string& value);
  void Write(const std::string& section, const std::string& var, const bool value);
  void Write(const std::string& section, const std::string& var, const float value);
  void Write(const std::string& section, const std::string& var, const Sint32 value);

  const std::string Read(const std::string& section, const std::string& var, const std::string& defValue) const;
  const bool    Read(const std::string& section, const std::string& var, const bool defValue) const;
  const float   Read(const std::string& section, const std::string& var, const float defValue) const;
  const Sint32  Read(const std::string& section, const std::string& var, const Sint32 defValue) const;

private:
  static const std::string GetVar(const std::string& str);
  static const std::string GetValue(const std::string& str);
};
