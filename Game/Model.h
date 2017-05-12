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

class IFileSystem;

class CModelData {
public:
  typedef std::vector<Uint16> indvector;

  enum class RenderMode {
    Triangles = 0,
    Lines = 1,
  };
  class CVertex {
  public:
    glm::vec3 Pos;

    CVertex(const glm::vec3& pos = glm::vec3());
  };
  class CSubset {
  public:
    RenderMode Mode;
    indvector Indices;

    CSubset(const RenderMode mode = RenderMode::Triangles);
  };
  typedef std::vector<CVertex> VertexVectorT;
  typedef std::map<cb::string, CSubset> SubsetMapT;

  VertexVectorT Vertices;
  SubsetMapT  Subsets;
  ModelType Type;

  CModelData();
  ~CModelData();

  const bool Save(IFileSystem& fs, const cb::string& filePath) const;
  const bool Load(IFileSystem& fs, const cb::string& filePath);
};

class CModel {
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
  typedef std::map<cb::string, CModel*> ModelMapT;

  ModelMapT mModelMap;
  IFileSystem* mFileSystem;

public:
  CModelRepository(IFileSystem* pFileSystem);
  ~CModelRepository();

  CModel* GetModel(const cb::string& filename);
  void Clear();
};

extern const cb::string toStr(const ModelType type);
extern const bool fromStr(const cb::string& text, ModelType& outType);
