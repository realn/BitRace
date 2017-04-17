#ifndef __BITRACE_TEXTURE_H__
#define __BITRACE_TEXTURE_H__

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <CBIO/Defines.h>

#include "GLDefines.h"

class CTextureParams {
public:
  Uint32 MagFilter;
  Uint32 MinFilter;
  Uint32 MipmapFilter;
  Uint32 WrapR;
  Uint32 WrapS;
  bool Mipmaps;

  CTextureParams();
};

class CTexture {
private:
  Uint32 mId;
  Uint32 mTarget;
  Uint32 mFormat;
  glm::uvec2 mSize;
  CTextureParams mParams;
  mutable bool mParamsChanged;

public:
  CTexture(const Uint32 target, 
           const glm::uvec2& size, 
           const Uint32 format, 
           const CTextureParams& parms = CTextureParams());
  ~CTexture();

  void SetParams(const CTextureParams& params);

  const Uint32 GetId() const;
  const Uint32 GetTarget() const;
  const Uint32 GetFormat() const;
  const CTextureParams& GetParams() const;

  void Bind(const Uint32 unit = 0) const;

  void Load(const Uint32 format, const Uint32 type, const void* pData);
  void Load(const Uint32 format, const cb::bytevector& data);

  static void UnBind(const CTexture& texture, const Uint32 unit = 0);
  static void UnBind(const Uint32 target, const Uint32 unit = 0);

private:
  void SetUpParams() const;
};

#endif // !__BITRACE_TEXTURE_H__

