#include "stdafx.h"
#include "GraphicBuffer.h"

CGraphicBuffer::CGraphicBuffer(const Uint32 target, const Uint32 usage)
  : mId(0)
  , mTarget(target)
  , mUsage(usage)
{
  glGenBuffers(1, &mId);
}

CGraphicBuffer::~CGraphicBuffer() {
  if(mId != 0) {
    glDeleteBuffers(1, &mId);
    mId = 0;
  }
}

void CGraphicBuffer::Bind() const {
  glBindBuffer(mTarget, mId);
}

void CGraphicBuffer::Load(const Uint32 size, const void * pData) {
  glBindBuffer(mTarget, mId);
  glBufferData(mTarget, size, pData, mUsage);
  glBindBuffer(mTarget, 0);
}

void CGraphicBuffer::UnBind(const CGraphicBuffer & buffer) {
  glBindBuffer(buffer.mTarget, 0);
}

void CGraphicBuffer::UnBind(const Uint32 target) {
  glBindBuffer(target, 0);
}
