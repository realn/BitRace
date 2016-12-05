#pragma once

#include <SDL_rwops.h>
#include <string>

class CFile {
protected:
  SDL_RWops* m_pFile;

public:
  CFile();
  CFile(const std::string& filename, const char *flags = "rb");
  ~CFile();

  const bool Open(const std::string& filename, const char *flags = "rb");

  const bool Close();

  const Uint32 Write(const void *pData, const Uint32 size, const Uint32 number = 1);
  const Uint32 Write(const std::string& text);
  const Uint32 WriteLine(const std::string& line);

  const Uint32 Read(void *pData, const Uint32 size, const Uint32 number = 1);
  const Uint32 Read(std::string& outText);
  const Uint32 ReadLine(std::string& outLine, const Uint32 bufferSize = 1024);

  const Uint64 GetPos() const;
  const Uint64 GetSize() const;

  const Uint64 CFile::Seek(const Uint64 offset, const Sint32 origin);
  const bool CFile::IsOpened() const;

  const bool CFile::IsEndOfFile() const;

  static const bool FileExists(const std::string& filename);

  static const std::string ReadTextFile(const std::string& filename);
};