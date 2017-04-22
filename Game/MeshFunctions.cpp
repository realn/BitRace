#include "stdafx.h"
#include "MeshFunctions.h"
#include "GLDefines.h"

void RenderVectorList(const Uint32 mode, const Uint32 vFloatNum, 
                      const void * vptr, const Uint32 size) {
  glVertexPointer(vFloatNum, GL_FLOAT, 0, vptr);

  glEnableClientState(GL_VERTEX_ARRAY);
  glDrawArrays(mode, 0, size);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void RenderVectorList(const Uint32 mode, const Uint32 vFloatNum, 
                      const void * vptr, const ind16vector & indices) {
  glVertexPointer(vFloatNum, GL_FLOAT, 0, vptr);

  glEnableClientState(GL_VERTEX_ARRAY);
  glDrawElements(mode, indices.size(), GL_UNSIGNED_SHORT, cb::vectorptr(indices));
  glDisableClientState(GL_VERTEX_ARRAY);
}

void RenderVectorList(const Uint32 mode, const vec3vector & vecList) {
  RenderVectorList(mode, 3, cb::vectorptr(vecList), vecList.size());
}

void RenderVectorList(const Uint32 mode, const vec3vector & vecList, 
                      const ind16vector & indices) {
  RenderVectorList(mode, 3, cb::vectorptr(vecList), indices);
}

void RenderTexVectorList(const Uint32 mode, 
                         const Uint32 vFloatNum, const void * vptr, 
                         const Uint32 texFloatNum, const void * texptr, 
                         const Uint32 size) {
  glVertexPointer(vFloatNum, GL_FLOAT, 0, vptr);
  glTexCoordPointer(texFloatNum, GL_FLOAT, 0, texptr);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glDrawArrays(mode, 0, size);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void RenderTexVectorList(const Uint32 mode, 
                         const Uint32 vFloatNum, const void * vptr, 
                         const Uint32 texFloatNum, const void * texptr, 
                         const ind16vector & indices) {
  glVertexPointer(vFloatNum, GL_FLOAT, 0, vptr);
  glTexCoordPointer(texFloatNum, GL_FLOAT, 0, texptr);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glDrawElements(mode, indices.size(), GL_UNSIGNED_SHORT, cb::vectorptr(indices));
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void RenderTexVectorList(const Uint32 mode, const vec3vector & vecList, 
                         const vec2vector & texList) {
  RenderTexVectorList(mode,
                      3, cb::vectorptr(vecList),
                      2, cb::vectorptr(texList),
                      vecList.size());
}

void RenderTexVectorList(const Uint32 mode, const vec3vector & vecList, 
                         const vec2vector & texList, ind16vector & indices) {
  RenderTexVectorList(mode,
                      3, cb::vectorptr(vecList),
                      2, cb::vectorptr(texList),
                      indices);
}

void RenderQuad(const glm::vec2 & pos, const glm::vec2 & size) {
  vec3vector vlist = {
    glm::vec3(pos, 0.0f),
    glm::vec3(pos.x + size.x, pos.y, 0.0f),
    glm::vec3(pos + size, 0.0f),
    glm::vec3(pos.x, pos.y + size.y, 0.0f)
  };
  ind16vector indices = {
    0, 1, 2, 0, 2, 3
  };
  RenderVectorList(GL_TRIANGLES, vlist, indices);
}

void RenderTexQuad(const glm::vec2 & pos, const glm::vec2 & size, const glm::vec2 & tpos, const glm::vec2 & tsize) {
  vec3vector vlist = {
    glm::vec3(pos, 0.0f),
    glm::vec3(pos.x + size.x, pos.y, 0.0f),
    glm::vec3(pos + size, 0.0f),
    glm::vec3(pos.x, pos.y + size.y, 0.0f)
  };
  vec2vector tlist = {
    tpos,
    glm::vec2(tpos.x + tsize.x, tpos.y),
    tpos + tsize,
    glm::vec2(tpos.x, tpos.y + tsize.y)
  };
  ind16vector indices = {
    0, 1, 2, 0, 2, 3
  };
  RenderTexVectorList(GL_TRIANGLES, vlist, tlist, indices);
}
