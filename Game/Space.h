#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <vector>

class CSpace {
private:
  unsigned				m_uVBOVertex;
  std::vector<glm::vec3>	m_afVertex;
  std::vector<Uint16>	m_auIndex;
  glm::vec3	m_afQuadVertex[4];
  unsigned	m_auQuadIndex[6];

public:
  CSpace();
  ~CSpace();

  void Free();
  bool Generate(float fWidth, float fHeight, unsigned int uCountX, unsigned int uCountY, float fY);
  void Render(glm::vec3 vColor) const;
};