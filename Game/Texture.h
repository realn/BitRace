#ifndef __BITRACE_TEXTURE_H__
#define __BITRACE_TEXTURE_H__

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <CBIO/Defines.h>

#include "GLDefines.h"

class CTexture {
private:
  Uint32 mId;
  Uint32 mTarget;
  Uint32 mFormat;
  glm::vec2 mSize;

public:
  CTexture(const Uint32 target, const glm::vec2& size, const Uint32 format);
  ~CTexture();

  const Uint32 GetId() const;
  const Uint32 GetTarget() const;
  const Uint32 GetFormat() const;

  void Bind(const Uint32 unit = 0) const;

  void Load(const Uint32 format, const Uint32 type, const void* pData);
  void Load(const Uint32 format, const cb::bytevector& data);

  static void UnBind(const CTexture& texture, const Uint32 unit = 0);
  static void UnBind(const Uint32 target, const Uint32 unit = 0);
};

#endif // !__BITRACE_TEXTURE_H__

