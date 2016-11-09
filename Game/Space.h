#pragma once

#include "../Common/GLDevice.h"
#include "../Common/Vector.h"
#include <vector>

class CSpace
{
private:
	UINT				m_uVBOVertex;
	std::vector<float>	m_afVertex;
	std::vector<UINT>	m_auIndex;
	float	m_afQuadVertex[12];
	UINT	m_auQuadIndex[6];

public:
	CSpace();
	~CSpace();

	void Free();
	bool Generate( float fWidth, float fHeight, unsigned int uCountX, unsigned int uCountY, float fY );
	void Render( vec3 vColor );
};