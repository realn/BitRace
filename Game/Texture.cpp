#include "stdafx.h"
#include "Texture.h"

const Uint32 GetTexFilter(const Uint32 filter, 
                          const Uint32 mipmapFilter, 
                          const bool mipmapEnabled) {
  if(mipmapEnabled) {
    switch(filter) {
    case GL_LINEAR_MIPMAP_LINEAR:
    case GL_LINEAR_MIPMAP_NEAREST:
    case GL_NEAREST_MIPMAP_LINEAR:
    case GL_NEAREST_MIPMAP_NEAREST:
      return filter;

    case GL_LINEAR:
      if(mipmapFilter == GL_LINEAR) return GL_LINEAR_MIPMAP_LINEAR;
      if(mipmapFilter == GL_NEAREST) return GL_LINEAR_MIPMAP_NEAREST;
      break;

    case GL_NEAREST:
      if(mipmapFilter == GL_LINEAR) return GL_NEAREST_MIPMAP_LINEAR;
      if(mipmapFilter == GL_NEAREST) return GL_NEAREST_MIPMAP_NEAREST;
      break;

    default:
      return filter;;
    }
  }
  else {
    switch(filter) {
    case GL_LINEAR_MIPMAP_LINEAR:
    case GL_LINEAR_MIPMAP_NEAREST:
      return GL_LINEAR;

    case GL_NEAREST_MIPMAP_LINEAR:
    case GL_NEAREST_MIPMAP_NEAREST:
      return GL_NEAREST;

    default:
      return filter;
    }
  }
  return GL_NEAREST;
}

CTextureParams::CTextureParams()
  : MagFilter(GL_LINEAR)
  , MinFilter(GL_LINEAR)
  , MipmapFilter(GL_LINEAR)
  , WrapR(GL_CLAMP)
  , WrapS(GL_CLAMP)
  , Mipmaps(true) {}


CTexture::CTexture(const Uint32 target, 
                   const glm::uvec2 & size, 
                   const Uint32 format,
                   const CTextureParams& params)
  : mId(0)
  , mTarget(target)
  , mFormat(format)
  , mSize(size)
  , mParams(params)
  , mParamsChanged(true)
{
  glGenTextures(1, &mId); 
  glBindTexture(mTarget, mId);

}

CTexture::~CTexture() {
  glDeleteTextures(1, &mId);
}

void CTexture::SetParams(const CTextureParams & params) {
  mParams = params;
  mParamsChanged = true;
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

const CTextureParams & CTexture::GetParams() const {
  return mParams;
}

void CTexture::Bind(const Uint32 unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(mTarget, mId);
  SetUpParams();
}

void CTexture::Load(const Uint32 format, const Uint32 type, const void * pData) {
  bool buildMipmaps = mParams.Mipmaps && mParamsChanged;

  glBindTexture(mTarget, mId);
  SetUpParams();
  glTexImage2D(mTarget, 0, mFormat, mSize.x, mSize.y, 0, format, type, pData);
  if(buildMipmaps) {
    glGenerateMipmap(mTarget);
  }
  glBindTexture(mTarget, 0);
}

void CTexture::Load(const Uint32 format, const cb::bytevector & data) {
  Load(format, GL_UNSIGNED_BYTE, cb::vectorptr(data));
}

void CTexture::UnBind(const Uint32 unit) const {
  UnBind(*this, unit);
}

void CTexture::UnBind(const CTexture & texture, const Uint32 unit) {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(texture.GetTarget(), 0);
}

void CTexture::UnBind(const Uint32 target, const Uint32 unit) {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(target, 0);
}

void CTexture::SetUpParams() const {
  if(!mParamsChanged)
    return;

  glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER,
                  GetTexFilter(mParams.MinFilter, mParams.MipmapFilter, false));
  glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER,
                  GetTexFilter(mParams.MagFilter, mParams.MipmapFilter, mParams.Mipmaps));

  glTexParameteri(mTarget, GL_TEXTURE_WRAP_R, mParams.WrapR);
  glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, mParams.WrapS);

  mParamsChanged = false;
}

