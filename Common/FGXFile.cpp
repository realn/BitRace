#include "FGXFile.h"
#include "Files.h"

CFGXFile::CFGXFile() {
  memset(&this->m_Header, 0, sizeof(FGXHEADER));
}

CFGXFile::~CFGXFile() {
  this->Free();
}

bool CFGXFile::Load(const std::string filename) {
  CFile file;

  if (!file.Open(filename, "rb"))
    return false;

  this->Free();

  file.Read(&this->m_Header, sizeof(FGXHEADER));
  this->m_Data.resize(m_Header.IMAGEWIDTH * m_Header.IMAGEHEIGHT * m_Header.IMAGEDEPTH);
  file.Read(&this->m_Data[0], sizeof(Byte), this->m_Data.size());

  file.Close();
  return true;
}

void CFGXFile::Free() {
  memset(&this->m_Header, 0, sizeof(FGXHEADER));
  this->m_Data.clear();
}

bool CFGXFile::IsLoaded() const {
  return !this->m_Data.empty();
}

bool CFGXFile::IsValid() const {
  return 
    strncmp(this->m_Header.FILEID, FGX_FILEID, 3) == 0 && 
    this->m_Header.FILEVERSION == FGX_VERSION &&
    this->IsLoaded();
}

const glm::ivec2 CFGXFile::GetSize() const {
  return glm::ivec2(this->m_Header.IMAGEWIDTH, this->m_Header.IMAGEHEIGHT);
}

const unsigned CFGXFile::GetImgDepth() const {
  return this->m_Header.IMAGEDEPTH;
}

const CFGXFile::CData & CFGXFile::GetData() const {
  return this->m_Data;
}
