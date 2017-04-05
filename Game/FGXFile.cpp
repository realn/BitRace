#include "stdafx.h"
#include "FGXFile.h"

#include <CBIO/File.h>

CFGXFile::CFGXFile() {
  memset(&mHeader, 0, sizeof(FGXHEADER));
}

CFGXFile::~CFGXFile() {
  Free();
}

bool CFGXFile::Load(const cb::string filepath) {
  cb::ifstream file(filepath, std::ios::binary | std::ios::in);

  if (!file.is_open())
    return false;

  Free();

  file.read(cb::byteptr(mHeader), sizeof(FGXHEADER));
  mData.resize(mHeader.IMAGEWIDTH * mHeader.IMAGEHEIGHT * mHeader.IMAGEDEPTH);
  file.read(cb::vectorptr(mData), mData.size());

  return true;
}

void CFGXFile::Free() {
  memset(&mHeader, 0, sizeof(FGXHEADER));
  this->mData.clear();
}

bool CFGXFile::IsLoaded() const {
  return !mData.empty();
}

bool CFGXFile::IsValid() const {
  return 
    strncmp(mHeader.FILEID, FGX_FILEID, 3) == 0 && 
    mHeader.FILEVERSION == FGX_VERSION &&
    IsLoaded();
}

const glm::ivec2 CFGXFile::GetSize() const {
  return glm::ivec2(mHeader.IMAGEWIDTH, mHeader.IMAGEHEIGHT);
}

const unsigned CFGXFile::GetImgDepth() const {
  return mHeader.IMAGEDEPTH;
}

const cb::bytevector & CFGXFile::GetData() const {
  return mData;
}
