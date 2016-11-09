#include "Space.h"

extern PFNGLBINDBUFFERARBPROC		glBindBufferARB;
extern PFNGLDELETEBUFFERSARBPROC	glDeleteBuffersARB;
extern PFNGLGENBUFFERSARBPROC		glGenBuffersARB;
extern PFNGLISBUFFERARBPROC			glIsBufferARB;
extern PFNGLBUFFERDATAARBPROC		glBufferDataARB;

extern bool	g_bVBOEnabled;

CSpace::CSpace() {}

CSpace::~CSpace() {
  Free();
};

void CSpace::Free() {
  m_afVertex.clear();
  m_auIndex.clear();
  if (g_bVBOEnabled)
    if (glIsBufferARB(m_uVBOVertex))
      glDeleteBuffersARB(1, &m_uVBOVertex);
};

bool CSpace::Generate(float fWidth, float fHeight, unsigned int uCountX, unsigned int uCountY, float fY) {
  float fSizeX = fWidth / float(uCountX++);
  float fSizeY = fHeight / float(uCountY++);

  float fStartX = 0.0f - (fWidth / 2.0f);
  float fStartY = 0.0f - (fHeight / 2.0f);

  m_afVertex.resize(uCountX * uCountY * 3);
  size_t i;
  for (i = 0; i < m_afVertex.size(); i += 3) {
    unsigned int w = unsigned int(i / 3) % uCountX;
    unsigned int h = unsigned int(i / 3) / uCountY;

    m_afVertex[i] = fStartX + float(w) * fSizeX;
    m_afVertex[i + 1] = fY;
    m_afVertex[i + 2] = fStartY + float(h) * fSizeY;
  };

  if (g_bVBOEnabled) {
    glGenBuffersARB(1, &m_uVBOVertex);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_uVBOVertex);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB,
                    m_afVertex.size() * sizeof(float), &m_afVertex[0], GL_STATIC_DRAW_ARB);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
  }

  unsigned int X, Y;
  for (Y = 0; Y < uCountY; ++Y) {
    for (X = 0; X < uCountX; ++X) {
      unsigned int ind = Y * uCountX + X;
      if (X < uCountX - 1) {
        m_auIndex.push_back(ind);
        m_auIndex.push_back(ind + 1);
      }
      if (Y < uCountY - 1) {
        m_auIndex.push_back(ind);
        m_auIndex.push_back(ind + uCountX);
      }
    }
  }

  this->m_afQuadVertex[0] = -fWidth / 2.0f;
  this->m_afQuadVertex[1] = fY;
  this->m_afQuadVertex[2] = -fHeight / 2.0f;

  this->m_afQuadVertex[3] = fWidth / 2.0f;
  this->m_afQuadVertex[4] = fY;
  this->m_afQuadVertex[5] = -fHeight / 2.0f;

  this->m_afQuadVertex[6] = fWidth / 2.0f;
  this->m_afQuadVertex[7] = fY;
  this->m_afQuadVertex[8] = fHeight / 2.0f;

  this->m_afQuadVertex[9] = -fWidth / 2.0f;
  this->m_afQuadVertex[10] = fY;
  this->m_afQuadVertex[11] = fHeight / 2.0f;

  this->m_auQuadIndex[0] = 0;
  this->m_auQuadIndex[1] = 1;
  this->m_auQuadIndex[2] = 2;
  this->m_auQuadIndex[3] = 2;
  this->m_auQuadIndex[4] = 3;
  this->m_auQuadIndex[5] = 0;

  return true;
};

void CSpace::Render(vec3 vColor) {
  if (m_afVertex.size() == 0 || m_auIndex.size() == 0)
    return;

  glEnableClientState(GL_VERTEX_ARRAY);
  glColor3fv(vColor.ToFloat());
  if (g_bVBOEnabled) {
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_uVBOVertex);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
  }
  else glVertexPointer(3, GL_FLOAT, 0, &m_afVertex[0]);
  glDrawElements(GL_LINES, (GLsizei)m_auIndex.size(), GL_UNSIGNED_INT, &m_auIndex[0]);
  if (g_bVBOEnabled)
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

  glEnable(GL_BLEND);
  glColor4f(vColor.X, vColor.Y, vColor.Z, 0.2f);
  glVertexPointer(3, GL_FLOAT, 0, m_afQuadVertex);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, m_auQuadIndex);
  glDisable(GL_BLEND);
  glDisableClientState(GL_VERTEX_ARRAY);
}