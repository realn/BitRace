#pragma once

#include <CBStr/StringEx.h>

class CIniFile {
private:
  cb::strvector mLines;
  cb::string mFilePath;

public:
  CIniFile();
  ~CIniFile();

  const bool Open(const cb::string& filepath);
  const bool Close();
  const bool IsOpened() const;

  void WriteString(const cb::string& section, 
                   const cb::string& var, 
                   const cb::string& value);

  template<typename _Type>
  void Write(const cb::string& section, 
             const cb::string& var, 
             const _Type& value) 
  {
    WriteString(section, var, cb::toStr(value));
  }

  const cb::string ReadString(const cb::string& section, 
                              const cb::string& var, 
                              const cb::string& defaultValue);

  template<typename _Type>
  const _Type Read(const cb::string& section,
                   const cb::string& var,
                   const _Type& defaultValue) 
  {
    cb::string val = ReadString(section, var, cb::string());
    if(val.empty())
      return defaultValue;

    _Type result;
    if(!cb::fromStr(val, result))
      return defaultValue;

    return result;
  }


private:
  const cb::string GetVar(const cb::string& str);
  const cb::string GetValue(const cb::string& str);

  cb::strvector::iterator FindSection(const cb::string& str);
  cb::strvector::iterator FindVar(const cb::string& section, const cb::string& var);
};
