#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>

#define	MODELTYPE_COUNT	14

class CModel {
public:
  enum ModelId {
    MT_NONE = 0,
    MT_HTTP10,
    MT_HTTP20,
    MT_P2PGNU,
    MT_P2PGNU2,
    MT_P2PFT,
    MT_P2PFT20,
    MT_P2PEDK2K,
    MT_P2PBT,
    MT_DL_PART,
    MT_DL_PART2,
    MT_BOMB,
    MT_HACK,
    MT_HACK2
  };

private:
  class CMesh;

  ModelId m_ModelId;
  std::string m_Name;

  Uint32  m_VertexBufferId;
  Uint32  m_IndexBufferId;
  Uint32  m_IndexBufferLinesId;
  
  Uint32  m_NumberOfTriangles;
  Uint32  m_NumberOfLines;

public:
  CModel(const ModelId modelId);
  ~CModel();

  void Render(const glm::mat4& transform, const glm::vec4& lineColor, const glm::vec4& polyColor = glm::vec4(glm::vec3(0.0f), 1.0f));

  const ModelId GetId() const;
  const std::string GetName() const;
};

class CModelRepository {
private:
  std::map<Uint32, CModel*> m_Models;

public:
  CModelRepository();
  ~CModelRepository();

  CModel* Get(const CModel::ModelId modelId);
  void Clear();
};
