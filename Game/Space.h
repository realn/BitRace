#pragma once

#include "../Common/Vector.h"
#include <vector>

class CSpace {
private:
  unsigned				m_uVBOVertex;
  std::vector<float>	m_afVertex;
  std::vector<unsigned>	m_auIndex;
  float	m_afQuadVertex[12];
  unsigned	m_auQuadIndex[6];

public:
  CSpace();
  ~CSpace();

  void Free();
  bool Generate(float fWidth, float fHeight, unsigned int uCountX, unsigned int uCountY, float fY);
  void Render(vec3 vColor);
};