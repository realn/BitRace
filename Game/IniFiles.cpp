#include "stdafx.h"
#include "IniFiles.h"
#include "Files.h"
#include "helper.h"

#include <sstream>

CIniFile::CIniFile() {}

CIniFile::CIniFile(const std::string& filename) {
  Load(filename);
}

CIniFile::~CIniFile() {
  Clear();
}

const bool CIniFile::Load(const std::string & filename) {
  CFile file(filename, "r");
  if(!file.IsOpened())
    return false;

  Clear();
  std::string currentSection;
  std::string line;
  while(!file.IsEndOfFile()) {
    file.ReadLine(line);
    if(line.empty())
      continue;

    if(line[0] == '[' && *line.rbegin() == ']') {
      currentSection = line.substr(1, line.length() - 2);
      if(m_Sections.find(currentSection) == m_Sections.end())
        m_Sections[currentSection] = CSection();
      continue;
    }

    if(currentSection.empty())
      continue;

    CSection& section = m_Sections[currentSection];

    std::string name = GetVar(line);
    std::string val = GetValue(line);

    if(!name.empty() && !val.empty())
      section.Values[name] = val;
  }

  return true;
}

const bool CIniFile::Save(const std::string & filename) {
  CFile file(filename, "w");
  if(!file.IsOpened())
    return false;

  for(sectionmap::iterator it = m_Sections.begin(); it != m_Sections.end(); it++) {
    file.WriteLine("[" + it->first + "]");

    for(valuemap::iterator itv = it->second.Values.begin(); itv != it->second.Values.end(); itv++) {
      file.WriteLine(itv->first + "=" + itv->second);
    }
  }

  return false;
}

void CIniFile::Clear() {
  m_Sections.clear();
}

void CIniFile::Write(const std::string & section, const std::string & var, const std::string & value) {
  sectionmap::iterator it = m_Sections.find(section);
  if(it == m_Sections.end()) {
    m_Sections[section] = CSection(var, value);
    return;
  }
  it->second.Values[var] = value;
}

void CIniFile::Write(const std::string & section, const std::string & var, const bool value) {
  Write(section, var, std::string(value ? "True" : "False"));
}

void CIniFile::Write(const std::string & section, const std::string & var, const float value) {
  std::stringstream ss;
  ss << value;
  Write(section, var, ss.str());
}

void CIniFile::Write(const std::string & section, const std::string & var, const Sint32 value) {
  std::stringstream ss;
  ss << value;
  Write(section, var, ss.str());
}

const std::string CIniFile::Read(const std::string & section, const std::string & var, const std::string & defValue) const {
  sectionmap::const_iterator it = m_Sections.find(section);
  if(it == m_Sections.end())
    return defValue;

  valuemap::const_iterator vit = it->second.Values.find(var);
  if(vit == it->second.Values.end())
    return defValue;

  return vit->second;
}

const bool CIniFile::Read(const std::string & section, const std::string & var, const bool defValue) const {
  return Read(section, var, std::string(defValue ? "True" : "False")) == "True";
}

const float CIniFile::Read(const std::string & section, const std::string & var, const float defValue) const {
  std::string value = Read(section, var, std::string());
  if(value.empty())
    return defValue;

  float result;
  if(!helper::fromstr(value, result))
    return defValue;
  return result;
}

const Sint32 CIniFile::Read(const std::string & section, const std::string & var, const Sint32 defValue) const {
  std::string value = Read(section, var, std::string());
  if(value.empty())
    return defValue;

  Sint32 result;
  if(!helper::fromstr(value, result))
    return defValue;
  return result;
}

const std::string CIniFile::GetVar(const std::string& str) {
  size_t pos = str.find('=');
  if(pos == std::string::npos)
    return str;
  return str.substr(0, pos);
}

const std::string CIniFile::GetValue(const std::string& str) {
  size_t pos = str.find('=');
  if(pos == std::string::npos)
    return str;
  return str.substr(pos + 1);
}
