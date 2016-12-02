#pragma once

#include "Files.h"
#include <string>
#include <sstream>
#include <vector>

class CIniFile {
protected:
  std::vector<std::string> Lines;
  std::string FileName;

  std::string CIniFile::GetVar(std::string str);
  std::string CIniFile::GetValue(std::string str);
  int CIniFile::FindSection(std::string str);
  int CIniFile::FindVar(std::string Section, std::string Var);
public:
  CIniFile();
  CIniFile(std::string filename);
  ~CIniFile();
  bool CIniFile::Open(std::string filename);
  bool CIniFile::Close();
  void CIniFile::WriteBool(std::string Section, std::string Var, bool value);
  void CIniFile::WriteFloat(std::string Section, std::string Var, float value);
  void CIniFile::WriteInt(std::string Section, std::string Var, int value);
  void CIniFile::WriteString(std::string Section, std::string Var, std::string value);
  bool CIniFile::ReadBool(std::string Section, std::string Var, bool DefValue);
  float CIniFile::ReadFloat(std::string Section, std::string Var, float DefValue);
  int CIniFile::ReadInt(std::string Section, std::string Var, int DefValue);
  std::string CIniFile::ReadString(std::string Section, std::string Var, std::string DefValue);
  bool CIniFile::Opened();
};