#include "IniFiles.h"

#include <CBIO/File.h>

CIniFile::CIniFile() {}

CIniFile::~CIniFile() {
  Close();
}

const bool CIniFile::Open(const cb::string& filepath) {
  mFilePath = filepath;

  cb::string text = cb::readtextfileutf8(filepath);
  if(text.empty())
    return false;

  size_t pos = 0;
  size_t endpos = text.find(L'\n');
  while(pos != cb::string::npos) {
    mLines.push_back(cb::substrpos(text, pos, endpos));

    pos = endpos;
    if(pos != cb::string::npos) {
      pos++;
      endpos = text.find(L'\n', pos);
    }
  }

  return true;
}

const bool CIniFile::Close() {
  if (mFilePath.empty()) {
    return false;
  }

  if (mLines.size() == 0) {
    mFilePath = cb::string();
    return true;
  }

  cb::string text = cb::join(mLines, L"\n");
  if(!cb::writetextfileutf8(mFilePath, text)) {
    return false;
  }

  mFilePath = cb::string();
  mLines.clear();

  return true;
}

const bool CIniFile::IsOpened() const {
  if(mFilePath.empty())
    return false;
  return true;
}

void CIniFile::WriteString(const cb::string& section, 
                           const cb::string& var, 
                           const cb::string& value) {
  cb::string tag = var + L"=" + value;

  cb::strvector::iterator it = FindVar(section, var);
  if(it != mLines.end()) {
    (*it) = tag;
    return;
  }

  it = FindSection(section);
  if(it != mLines.end()) {
    it++;
    mLines.insert(it, tag);
    return;
  }

  mLines.push_back(L"[" + section + L"]");
  mLines.push_back(tag);
}

const cb::string CIniFile::ReadString(const cb::string& section, 
                                      const cb::string& var, 
                                      const cb::string& defaultValue) 
{
  if (mLines.empty())
    return defaultValue;

  cb::strvector::iterator it = FindVar(section, var);
  if(it == mLines.end())
    return defaultValue;

  return GetValue(*it);
}

const cb::string CIniFile::GetVar(const cb::string& str) {
  size_t pos = str.find(L'=');
  if(pos == std::string::npos)
    return cb::string();
  return str.substr(0, pos);
}

const cb::string CIniFile::GetValue(const cb::string& str) {
  size_t pos = str.find(L'=');
  if(pos == std::string::npos)
    return cb::string();
  return str.substr(pos + 1);
}

cb::strvector::iterator CIniFile::FindSection(const cb::string& str) {
  const cb::string sectionTag = L"[" + str + L"]";
  for(cb::strvector::iterator it = mLines.begin(); it != mLines.end(); it++) {
    if(*it == sectionTag) {
      return it;
    }
  }
  return mLines.end();
}

cb::strvector::iterator CIniFile::FindVar(const cb::string& section, const cb::string& var) {
  cb::strvector::iterator it = FindSection(section);
  if(it == mLines.end())
    return mLines.end();

  for(it++; it != mLines.end(); it++) {
    const cb::string& line = *it;

    if(line[0] == L'[')
      return mLines.end();

    if(GetVar(line) == var)
      return it;
  }

  return mLines.end();
}
