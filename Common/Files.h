#pragma once

#include <stdio.h>
#include <io.h>
#include <string>

class CFile {
protected:
  FILE* fp;

public:
  CFile() : fp(NULL) {}
  CFile(std::string filename, char *flags = "rb");
  ~CFile();

  bool Open(std::string filename, char *flags = "rb");

  bool Close();

  unsigned int CFile::Write(void *Data, unsigned int Size, unsigned int Count = 1);

  unsigned int CFile::Read(void *Data, unsigned int Size, unsigned int Count = 1);

  unsigned int CFile::WriteStr(std::string str);

  unsigned int CFile::WriteStr(std::wstring str);

  std::string CFile::ReadStr(unsigned int BufSize);

  std::wstring CFile::ReadWStr(unsigned int BufSize);

  __int64 CFile::Length();

  bool CFile::Seek(__int64 offset, int origin);

  FILE* CFile::GetPointer();

  bool CFile::Opened();

  bool CFile::EndOfFile();

  bool CFile::Flush();

  static bool FileExists(std::string filename);

  static std::string ReadTextFile(std::string filename);
};