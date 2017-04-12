#include "stdafx.h"
#include "MeshFunctions.h"
#include "GLDefines.h"

void RenderVectorList(const Uint32 mode, const vec3vector & vecList) {
  glVertexPointer(3, GL_FLOAT, 0, cb::vectorptr(vecList));

  glEnableClientState(GL_VERTEX_ARRAY);
  glDrawArrays(mode, 0, vecList.size());
  glDisableClientState(GL_VERTEX_ARRAY);
}
