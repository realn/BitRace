#pragma once

#include <vector>

#define	MODELTYPE_COUNT	14

class CModel {
private:
  unsigned				m_uVBOVertex;

  std::vector<float>	m_afVertex;
  std::vector<unsigned>	m_auIndexTriangles;
  std::vector<unsigned>	m_auIndexLines;
  std::string			m_strModelName;
  unsigned				m_uModelType;

  void AddVertex(float x, float y, float z);
  void AddTriangle(unsigned v1, unsigned v2, unsigned v3);
  void CreateIndexLines();

public:
  enum MODEL_TYPE {
    MT_NONE = 0,
    MT_HTTP10 = 1,
    MT_HTTP20 = 2,
    MT_P2PGNU = 3,
    MT_P2PGNU2 = 4,
    MT_P2PFT = 5,
    MT_P2PFT20 = 6,
    MT_P2PEDK2K = 7,
    MT_P2PBT = 8,
    MT_DL_PART = 9,
    MT_DL_PART2 = 10,
    MT_BOMB = 11,
    MT_HACK = 12,
    MT_HACK2 = 13
  };

  CModel();
  ~CModel();

  void Free();
  bool Generate(unsigned uModelType);
  void Render();
  unsigned GetModelType();

  static	bool	InitModels();
  static	void	FreeModels();
  static	CModel*	GetModel(unsigned uModelType);
};