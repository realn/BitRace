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
  CSpace(const glm::vec2& size, const glm::uvec2& lineNum, const float y);
  ~CSpace();

  void Render(const glm::mat4& transform, const glm::vec3& vColor);
};