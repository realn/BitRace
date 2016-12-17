#include "stdafx.h"
#include "Space.h"

#include <glm/gtc/type_ptr.hpp>

CSpace::CSpace(const glm::vec2& size, const glm::uvec2& lineNum, const float y) {
  glm::uvec2 lines = lineNum + glm::uvec2(1);

  glm::vec2 step = size / glm::vec2(lineNum);

  glm::vec2 start = glm::vec2(0.0f) - (size / 2.0f);

  m_afVertex.resize(lines.x * lines.y);
  for(Uint32 i = 0; i < m_afVertex.size(); i++) {
    Uint32 w = i % lines.x;
    Uint32 h = i / lines.y;

    m_afVertex[i].x = start.x + float(w) * step.x;
    m_afVertex[i].y = y;
    m_afVertex[i].z = start.y + float(h) * step.y;
  };

  glGenBuffersARB(1, &m_uVBOVertex);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_uVBOVertex);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB,
                  m_afVertex.size() * sizeof(glm::vec3), &m_afVertex[0], GL_STATIC_DRAW_ARB);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

  for(Uint32 Y = 0; Y < lines.y; Y++) {
    for(Uint32 X = 0; X < lines.x; X++) {
      Uint16 ind = Y * lines.x + X;
      if(X < lines.x - 1) {
        m_auIndex.push_back(ind);
        m_auIndex.push_back(ind + 1);
      }
      if(Y < lines.x - 1) {
        m_auIndex.push_back(ind);
        m_auIndex.push_back(ind + lines.x);
      }
    }
  }

  this->m_afQuadVertex[0].x = -size.x / 2.0f;
  this->m_afQuadVertex[0].y = y;
  this->m_afQuadVertex[0].z = -size.y / 2.0f;

  this->m_afQuadVertex[1].x = size.x / 2.0f;
  this->m_afQuadVertex[1].y = y;
  this->m_afQuadVertex[1].z = -size.y / 2.0f;

  this->m_afQuadVertex[2].x = size.x / 2.0f;
  this->m_afQuadVertex[2].y = y;
  this->m_afQuadVertex[2].z = size.y / 2.0f;

  this->m_afQuadVertex[3].x = -size.x / 2.0f;
  this->m_afQuadVertex[3].y = y;
  this->m_afQuadVertex[3].z = size.y / 2.0f;

  this->m_auQuadIndex[0] = 0;
  this->m_auQuadIndex[1] = 1;
  this->m_auQuadIndex[2] = 2;
  this->m_auQuadIndex[3] = 2;
  this->m_auQuadIndex[4] = 3;
  this->m_auQuadIndex[5] = 0;
}

CSpace::~CSpace() {
  m_afVertex.clear();
  m_auIndex.clear();
  if(glIsBufferARB(m_uVBOVertex))
    glDeleteBuffersARB(1, &m_uVBOVertex);
};

void CSpace::Render(const glm::mat4& transform, const glm::vec3& vColor) {
  if (m_afVertex.size() == 0 || m_auIndex.size() == 0)
    return;

  glLoadMatrixf(glm::value_ptr(transform));

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