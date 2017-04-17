#pragma once

#include <vector>
#include <SDL_types.h>
#include "GraphicBuffer.h"

#define	MODELTYPE_COUNT	14

enum class ModelType {
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
  MT_HACK2 = 13,

  MODELTYPE_NUMBER
};

class CModelData {
public:
  typedef std::vector<glm::vec3> vec3vector;
  typedef std::vector<Uint16> indvector;

  vec3vector Vertices;
  indvector TriangleIndices;
  indvector LineIndices;
  ModelType Type;

  CModelData();

  const bool Generate(const ModelType type);

  void AddVertex(const float x, const float y, const float z);
  void AddTriangle(const Uint16 v1, const Uint16 v2, const Uint16 v3);
  void CreateIndexLines();
};

class CModel {
public:

private:
  CGraphicBuffer mVertexBuffer;
  CGraphicBuffer mIndexBuffer;
  ModelType mType;
  Uint32 mTriangleIndicesNumber;
  Uint32 mLineIndicesNumber;

public:
  CModel();
  ~CModel();

  const ModelType GetType() const;

  const bool Load(CModelData& data);
  void Free();

  void Render(const glm::vec4& lineColor, const glm::vec4& triColor);
};

class CModelRepository {
private:
  typedef std::map<ModelType, CModel*> ModelMapT;

  ModelMapT mModelMap;
public:
  CModelRepository();
  ~CModelRepository();

  CModel* GetModel(const ModelType type);
  void Clear();

  static CModelRepository Instance;
};
