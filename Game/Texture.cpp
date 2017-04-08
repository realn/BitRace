#include "stdafx.h"
#include "Texture.h"

CTexture::CTexture(const Uint32 target, 
                   const glm::vec2 & size, 
                   const Uint32 format)
  : mId(0)
  , mTarget(target)
  , mFormat(format)
  , mSize(size)
{
  glGenTextures(1, &mId); 
}

CTexture::~CTexture() {
  glDeleteTextures(1, &mId);
}

const Uint32 CTexture::GetId() const {
  return mId;
}

const Uint32 CTexture::GetTarget() const {
  return mTarget;
}

const Uint32 CTexture::GetFormat() const {
  return mFormat;
}

void CTexture::Bind(const Uint32 unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(mTarget, mId);
}

void CTexture::Load(const Uint32 format, const Uint32 type, const void * pData) {}

void CTexture::Load(const Uint32 format, const cb::bytevector & data) {}

void CTexture::UnBind(const CTexture & texture, const Uint32 unit) {}

void CTexture::UnBind(const Uint32 target, const Uint32 unit) {}
