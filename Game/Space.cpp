#include "stdafx.h"
#include "Space.h"
#include "GLDefines.h"

#include <glm/gtc/type_ptr.hpp>

CSpace::CSpace() {}

CSpace::~CSpace() {
  Free();
};

void CSpace::Free() {
  m_afVertex.clear();
  m_auIndex.clear();
  if(glIsBufferARB(m_uVBOVertex))
    glDeleteBuffersARB(1, &m_uVBOVertex);
};

bool CSpace::Generate(float fWidth, float fHeight, unsigned int uCountX, unsigned int uCountY, float fY) {
  unsigned linesX = uCountX + 1;
  unsigned linesY = uCountY + 1;

  float fSizeX = fWidth / float(uCountX);
  float fSizeY = fHeight / float(uCountY);

  float fStartX = 0.0f - (fWidth / 2.0f);
  float fStartY = 0.0f - (fHeight / 2.0f);

  m_afVertex.resize(linesX * linesY);
  for(unsigned i = 0; i < m_afVertex.size(); i++) {
    unsigned w = i % linesX;
    unsigned h = i / linesY;

    m_afVertex[i].x = fStartX + float(w) * fSizeX;
    m_afVertex[i].y = fY;
    m_afVertex[i].z = fStartY + float(h) * fSizeY;
  };

  glGenBuffersARB(1, &m_uVBOVertex);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_uVBOVertex);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB,
                  m_afVertex.size() * sizeof(glm::vec3), &m_afVertex[0], GL_STATIC_DRAW_ARB);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

  for(unsigned Y = 0; Y < linesY; Y++) {
    for(unsigned X = 0; X < linesX; X++) {
      Uint16 ind = Y * linesX + X;
      if(X < linesX - 1) {
        m_auIndex.push_back(ind);
        m_auIndex.push_back(ind + 1);
      }
      if(Y < linesY - 1) {
        m_auIndex.push_back(ind);
        m_auIndex.push_back(ind + linesX);
      }
    }
  }

  this->m_afQuadVertex[0].x = -fWidth / 2.0f;
  this->m_afQuadVertex[0].y = fY;
  this->m_afQuadVertex[0].z = -fHeight / 2.0f;

  this->m_afQuadVertex[1].x = fWidth / 2.0f;
  this->m_afQuadVertex[1].y = fY;
  this->m_afQuadVertex[1].z = -fHeight / 2.0f;

  this->m_afQuadVertex[2].x = fWidth / 2.0f;
  this->m_afQuadVertex[2].y = fY;
  this->m_afQuadVertex[2].z = fHeight / 2.0f;

  this->m_afQuadVertex[3].x = -fWidth / 2.0f;
  this->m_afQuadVertex[3].y = fY;
  this->m_afQuadVertex[3].z = fHeight / 2.0f;

  this->m_auQuadIndex[0] = 0;
  this->m_auQuadIndex[1] = 1;
  this->m_auQuadIndex[2] = 2;
  this->m_auQuadIndex[3] = 2;
  this->m_auQuadIndex[4] = 3;
  this->m_auQuadIndex[5] = 0;

  return true;
};

void CSpace::Render(glm::vec3 vColor) {
  if(m_afVertex.size() == 0 || m_auIndex.size() == 0)
    return;

  glEnableClientState(GL_VERTEX_ARRAY);
  glColor3fv(glm::value_ptr(vColor));
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_uVBOVertex);
  glVertexPointer(3, GL_FLOAT, 0, NULL);
  glDrawElements(GL_LINES, (GLsizei)m_auIndex.size(), GL_UNSIGNED_SHORT, &m_auIndex[0]);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

  glEnable(GL_BLEND);
  glColor4f(vColor.x, vColor.y, vColor.z, 0.2f);
  glVertexPointer(3, GL_FLOAT, 0, m_afQuadVertex);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, m_auQuadIndex);
  glDisable(GL_BLEND);
  glDisableClientState(GL_VERTEX_ARRAY);
}