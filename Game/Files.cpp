#include "stdafx.h"
#include "Files.h"
#include <SDL.h>
#include <vector>

CFile::CFile() :
  m_pFile(nullptr) {}

CFile::CFile(const std::string& filename, const char * flags) : 
  m_pFile(nullptr) 
{
  Open(filename, flags);
}

CFile::~CFile() {
  Close();
}

const bool CFile::Open(const std::string& filename, const char * flags) {
  Close();
  m_pFile = SDL_RWFromFile(filename.c_str(), flags);
  return m_pFile != nullptr;
}

const bool CFile::Close() {
  if(m_pFile) {
    SDL_RWclose(m_pFile);
    m_pFile = nullptr;
    return true;
  }
  return false;
}

const Uint32 CFile::Write(const void* pData, const Uint32 size, const Uint32 number) {
  return SDL_RWwrite(m_pFile, pData, size, number);
}

const Uint32 CFile::Write(const std::string & text) {
  return Write(reinterpret_cast<const Uint8*>(text.c_str()), sizeof(char), text.length());
}

const Uint32 CFile::WriteLine(const std::string& line) {
  return Write(line + "\n");
}

const Uint32 CFile::Read(void* pData, const Uint32 size, Uint32 number) {
  return SDL_RWread(m_pFile, pData, size, number);
}

const Uint32 CFile::Read(std::string & outText) {
  Uint32 len = (Uint32)GetSize() / sizeof(char);
  if(len == 0)
    return 0;

  outText.clear();
  outText.resize(len);
  return Read(reinterpret_cast<Uint8*>(&outText[0]), sizeof(char), len);
}

const Uint32 CFile::ReadLine(std::string & outLine, const Uint32 bufferSize) {
  Uint64 pos = GetPos();
  Uint32 size = (GetSize() - pos) > bufferSize ? bufferSize : (Uint32)(GetSize() - pos);
  std::string buffer;
  buffer.resize(size);
  Uint32 read = Read(&buffer[0], sizeof(char), buffer.size());
  if(read == 0)
    return 0;

  outLine.clear();
  Uint32 offset = buffer.find('\n');
  if(offset == std::string::npos) {
    outLine = buffer;
    return read;
  }

  outLine = buffer.substr(0, offset);
  if(*outLine.rbegin() == '\r')
    outLine = outLine.substr(0, outLine.length() - 1);

  Seek(pos + (Uint64)offset + 1, RW_SEEK_SET);
  return offset + 1;
}

const Uint64 CFile::GetSize() const {
  return SDL_RWsize(m_pFile);
}

const Uint64 CFile::GetPos() const {
  return SDL_RWtell(m_pFile);
}

const Uint64 CFile::Seek(const Uint64 offset, const Sint32 origin) {
  return SDL_RWseek(m_pFile, offset, origin);
}

const bool CFile::IsOpened() const {
  return m_pFile != nullptr;
}

const bool CFile::IsEndOfFile() const {
  return GetPos() >= GetSize();
}

const bool CFile::FileExists(const std::string& filename) {
  CFile file(filename, "r");
  return file.IsOpened();
}

const std::string CFile::ReadTextFile(const std::string& filename) {
  CFile file(filename, "r");
  if(!file.IsOpened())
    return std::string();

  std::string result;
  file.Read(result);
  return result;
}
