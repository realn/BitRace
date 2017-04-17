#ifndef __BITRACE_GRAPHICBUFFER_H__
#define __BITRACE_GRAPHICBUFFER_H__

#include <SDL_types.h>
#include <CBIO/Defines.h>

class CGraphicBuffer {
private:
  Uint32 mId;
  Uint32 mTarget;
  Uint32 mUsage;

public:
  CGraphicBuffer(const Uint32 target, const Uint32 usage);
  ~CGraphicBuffer();

  void Bind() const;
  void UnBind() const;

  void Load(const Uint32 size, const void* pData);
  template<typename _Type>
  void Load(const std::vector<_Type>& data) {
    Load(sizeof(_Type) * data.size(), cb::vectorptr(data));
  }
  void SubLoad(const Uint32 offset, const Uint32 size, const void* pData);

  static void UnBind(const CGraphicBuffer& buffer);
  static void UnBind(const Uint32 target);
};

#endif // !__BITRACE_GRAPHICBUFFER_H__

