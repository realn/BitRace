#include "Model.h"

#include <glm/gtc/type_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

class CModel::CMesh {
private:
  std::string  m_Name;
  std::vector<glm::vec3>  m_Vertices;
  std::vector<Uint16>     m_Indices;
  std::vector<Uint16>     m_IndicesLines;

public:
  CMesh();

  const bool Generate(const CModel::ModelId modelId);

  const std::string&  GetName() const;
  const std::vector<glm::vec3>& GetVertices() const;
  const std::vector<Uint16>&  GetIndices() const;
  const std::vector<Uint16>&  GetIndicesLines() const;

private:
  Uint16 AddVertex(const float x, const float y, const float z);
  Uint16 AddVertex(const glm::vec3& vertex);
  void AddTriangle(const Uint16 v1, const Uint16 v2, const Uint16 v3);
  void AddTriangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);
  void AddLine(const Uint16 v1, const Uint16 v2);
};

CModel::CMesh::CMesh() {}

const bool CModel::CMesh::Generate(const CModel::ModelId modelId) {
  switch (modelId) {
  default:
  case MT_NONE:
    return false;

  case MT_HTTP10:
    this->m_Name = "HTTP v1.0";
    AddVertex(0.0f, 0.0f, -4.0f);
    AddVertex(2.8f, 0.0f, 1.6f);
    AddVertex(-2.8f, 0.0f, 1.6f);
    AddVertex(0.0f, 2.4f, 1.2f);
    AddVertex(0.0f, 1.6f, 2.0f);

    AddTriangle(0, 1, 2);
    AddTriangle(0, 1, 3);
    AddTriangle(0, 2, 3);
    AddTriangle(1, 3, 4);
    AddTriangle(3, 4, 2);
    AddTriangle(1, 2, 4);
    return true;

  case MT_HTTP20:
    this->m_Name = "HTTP v2.0+";
    AddVertex(0.0f, 0.0f, -4.0f);
    AddVertex(2.4f, 0.0f, -0.8f);
    AddVertex(-2.4f, 0.0f, -0.8f);
    AddVertex(0.0f, 2.0f, 1.2f);
    AddVertex(0.0f, 0.4f, 2.0f);
    AddVertex(0.6f, 1.2f, 2.0f);
    AddVertex(-0.6f, 1.2f, 2.0f);

    AddTriangle(0, 1, 2);
    AddTriangle(0, 1, 3);
    AddTriangle(0, 2, 3);
    AddTriangle(1, 2, 4);
    AddTriangle(1, 4, 5);
    AddTriangle(1, 5, 3);
    AddTriangle(2, 4, 6);
    AddTriangle(2, 6, 3);
    AddTriangle(4, 5, 6);
    AddTriangle(5, 6, 3);
    return true;

  case MT_P2PGNU:
    this->m_Name = "P2P Gnutella";
    AddVertex(0.0f, 1.2f, -3.6f);
    AddVertex(0.8f, 0.0f, -4.0f);
    AddVertex(-0.8f, 0.0f, -4.0f);
    AddVertex(0.0f, 2.4f, 1.2f);
    AddVertex(2.4f, 0.0f, 2.0f);
    AddVertex(-2.4f, 0.0f, 2.0f);
    AddVertex(0.0f, 0.0f, 2.4f);
    AddVertex(0.8f, 1.2f, 2.4f);
    AddVertex(-0.8f, 1.2f, 2.4f);

    AddTriangle(0, 1, 2);
    AddTriangle(1, 6, 2);
    AddTriangle(0, 3, 4);
    AddTriangle(0, 3, 5);
    AddTriangle(0, 1, 4);
    AddTriangle(0, 2, 5);
    AddTriangle(1, 6, 4);
    AddTriangle(2, 6, 5);
    AddTriangle(3, 7, 8);
    AddTriangle(3, 7, 4);
    AddTriangle(3, 8, 5);
    AddTriangle(4, 7, 6);
    AddTriangle(5, 8, 6);
    AddTriangle(6, 7, 8);
    return true;

  case MT_P2PGNU2:
    this->m_Name = "P2P Gnutella 2";
    AddVertex(0.0f, 1.0f, -3.6f);
    AddVertex(0.8f, 0.4f, -4.0f);
    AddVertex(-0.8f, 0.4f, -4.0f);
    AddVertex(0.0f, 2.0f, 1.2f);
    AddVertex(2.0f, 0.0f, 2.4f);
    AddVertex(-2.0f, 0.0f, 2.4f);
    AddVertex(0.0f, 0.6f, 2.0f);
    AddVertex(0.8f, 1.2f, 2.0f);
    AddVertex(-0.8f, 1.2f, 2.0f);

    AddTriangle(0, 1, 2);
    AddTriangle(1, 6, 2);
    AddTriangle(0, 3, 4);
    AddTriangle(0, 3, 5);
    AddTriangle(0, 1, 4);
    AddTriangle(0, 2, 5);
    AddTriangle(1, 6, 4);
    AddTriangle(2, 6, 5);
    AddTriangle(3, 7, 8);
    AddTriangle(3, 7, 4);
    AddTriangle(3, 8, 5);
    AddTriangle(4, 7, 6);
    AddTriangle(5, 8, 6);
    AddTriangle(6, 7, 8);
    return true;

  case MT_P2PFT:
    this->m_Name = "P2P FastTrack";
    AddVertex(0.0f, 0.0f, -4.0f);
    AddVertex(0.8f, 0.0f, -2.0f);
    AddVertex(-0.8f, 0.0f, -2.0f);
    AddVertex(0.0f, 1.6f, 0.0f);
    AddVertex(0.0f, 0.4f, 1.2f);
    AddVertex(1.6f, 0.0f, 1.6f);
    AddVertex(-1.6f, 0.0f, 1.6f);
    AddVertex(0.8f, 0.8f, 0.8f);
    AddVertex(-0.8f, 0.8f, 0.8f);

    AddTriangle(0, 1, 3);
    AddTriangle(0, 2, 3);
    AddTriangle(0, 1, 2);
    AddTriangle(1, 2, 4);
    AddTriangle(4, 5, 1);
    AddTriangle(4, 6, 2);
    AddTriangle(1, 5, 7);
    AddTriangle(2, 6, 8);
    AddTriangle(4, 5, 7);
    AddTriangle(4, 6, 8);
    AddTriangle(3, 1, 7);
    AddTriangle(3, 2, 8);
    AddTriangle(3, 7, 8);
    AddTriangle(4, 7, 8);
    return true;

  case MT_P2PFT20:
    this->m_Name = "P2P FastTrack v2.0";
    AddVertex(0.0f, 0.2f, -4.0f);
    AddVertex(0.8f, 0.0f, -2.0f);
    AddVertex(-0.8f, 0.0f, -2.0f);
    AddVertex(0.0f, 1.2f, 1.2f);
    AddVertex(0.0f, 0.4f, 0.8f);
    AddVertex(2.0f, 0.0f, 2.0f);
    AddVertex(-2.0f, 0.0f, 2.0f);
    AddVertex(0.8f, 0.8f, 0.8f);
    AddVertex(-0.8f, 0.8f, 0.8f);

    AddTriangle(0, 1, 3);
    AddTriangle(0, 2, 3);
    AddTriangle(0, 1, 2);
    AddTriangle(1, 2, 4);
    AddTriangle(4, 5, 1);
    AddTriangle(4, 6, 2);
    AddTriangle(1, 5, 7);
    AddTriangle(2, 6, 8);
    AddTriangle(4, 5, 7);
    AddTriangle(4, 6, 8);
    AddTriangle(3, 1, 7);
    AddTriangle(3, 2, 8);
    AddTriangle(3, 7, 8);
    AddTriangle(4, 7, 8);
    return true;

  case MT_P2PEDK2K:
    this->m_Name = "P2P eDonkey 2000";
    AddVertex(0.0f, 0.8f, -4.0f);	// 0
    AddVertex(0.8f, 0.0f, -2.8f);	// 1
    AddVertex(-0.8f, 0.0f, -2.8f);// 2
    AddVertex(0.0f, 0.4f, -1.2f);	// 3
    AddVertex(0.0f, 1.6f, 0.8f);	// 4
    AddVertex(1.2f, 0.0f, 0.4f);	// 5
    AddVertex(-1.2f, 0.0f, 0.4f); // 6
    AddVertex(1.6f, 0.4f, 0.4f);	// 7
    AddVertex(-1.6f, 0.4f, 0.4f);	// 8
    AddVertex(0.8f, 0.4f, 0.4f);	// 9
    AddVertex(-0.8f, 0.4f, 0.4f);	// 10
    AddVertex(0.0f, 0.4f, 0.0f);	// 11

    AddTriangle(0, 1, 3);
    AddTriangle(1, 2, 3);
    AddTriangle(0, 2, 3);
    AddTriangle(0, 1, 4);
    AddTriangle(0, 2, 4);
    AddTriangle(1, 5, 7);
    AddTriangle(2, 6, 8);
    AddTriangle(1, 2, 11);
    AddTriangle(1, 5, 11);
    AddTriangle(2, 6, 11);
    AddTriangle(1, 4, 7);
    AddTriangle(2, 4, 8);
    AddTriangle(11, 5, 9);
    AddTriangle(11, 6, 10);
    AddTriangle(11, 4, 9);
    AddTriangle(11, 4, 10);
    AddTriangle(4, 7, 9);
    AddTriangle(4, 8, 10);
    AddTriangle(5, 7, 9);
    AddTriangle(6, 8, 10);
    break;

  case MT_P2PBT:
    this->m_Name = "P2P BitTorrent";
    AddVertex(0.0f, 0.4f, -3.2f);	// 0
    AddVertex(0.4f, 0.0f, -2.8f);	// 1
    AddVertex(-0.4f, 0.0f, -2.8f);// 2
    AddVertex(0.0f, 0.4f, -2.0f);	// 3
    AddVertex(0.0f, 1.2f, 0.4f);	// 4
    AddVertex(1.2f, 0.0f, 1.2f);	// 5
    AddVertex(-1.2f, 0.0f, 1.2f); // 6
    AddVertex(1.6f, 0.4f, 1.6f);	// 7
    AddVertex(-1.6f, 0.4f, 1.6f);	// 8
    AddVertex(0.8f, 0.4f, 1.2f);	// 9
    AddVertex(-0.8f, 0.4f, 1.2f);	// 10
    AddVertex(0.0f, 0.8f, 0.0f);	// 11

    AddTriangle(0, 1, 3);
    AddTriangle(1, 2, 3);
    AddTriangle(0, 2, 3);
    AddTriangle(0, 1, 4);
    AddTriangle(0, 2, 4);
    AddTriangle(1, 5, 7);
    AddTriangle(2, 6, 8);
    AddTriangle(1, 2, 11);
    AddTriangle(1, 5, 11);
    AddTriangle(2, 6, 11);
    AddTriangle(1, 4, 7);
    AddTriangle(2, 4, 8);
    AddTriangle(11, 5, 9);
    AddTriangle(11, 6, 10);
    AddTriangle(11, 4, 9);
    AddTriangle(11, 4, 10);
    AddTriangle(4, 7, 9);
    AddTriangle(4, 8, 10);
    AddTriangle(5, 7, 9);
    AddTriangle(6, 8, 10);
    return true;

  case MT_DL_PART:
    this->m_Name = "DownLoad Part";
    AddVertex(2.0f, -2.0f, 2.0f);	// 0
    AddVertex(-2.0f, -2.0f, 2.0f);	// 1
    AddVertex(-2.0f, -2.0f, -2.0f);// 2
    AddVertex(2.0f, -2.0f, -2.0f);	// 3
    AddVertex(2.0f, 2.0f, 2.0f);	// 4
    AddVertex(-2.0f, 2.0f, 2.0f);	// 5
    AddVertex(-2.0f, 2.0f, -2.0f);// 6
    AddVertex(2.0f, 2.0f, -2.0f);	// 7

    AddTriangle(0, 1, 2);
    AddTriangle(0, 2, 3);
    AddTriangle(4, 5, 6);
    AddTriangle(4, 6, 7);
    AddTriangle(0, 1, 5);
    AddTriangle(0, 5, 4);
    AddTriangle(3, 2, 6);
    AddTriangle(3, 6, 7);
    AddTriangle(2, 3, 7);
    AddTriangle(2, 7, 4);
    AddTriangle(1, 2, 6);
    AddTriangle(1, 6, 5);
    return true;

  case MT_DL_PART2:
    this->m_Name = "BIG DownLoad Part";
    AddVertex(3.0f, -3.0f, 3.0f);	// 0
    AddVertex(-3.0f, -3.0f, 3.0f);	// 1
    AddVertex(-3.0f, -3.0f, -3.0f);// 2
    AddVertex(3.0f, -3.0f, -3.0f);	// 3
    AddVertex(3.0f, 3.0f, 3.0f);	// 4
    AddVertex(-3.0f, 3.0f, 3.0f);	// 5
    AddVertex(-3.0f, 3.0f, -3.0f);// 6
    AddVertex(3.0f, 3.0f, -3.0f);	// 7

    AddTriangle(0, 1, 2);
    AddTriangle(0, 2, 3);
    AddTriangle(4, 5, 6);
    AddTriangle(4, 6, 7);
    AddTriangle(0, 1, 5);
    AddTriangle(0, 5, 4);
    AddTriangle(3, 2, 6);
    AddTriangle(3, 6, 7);
    AddTriangle(2, 3, 7);
    AddTriangle(2, 7, 4);
    AddTriangle(1, 2, 6);
    AddTriangle(1, 6, 5);
    return true;

  case MT_BOMB:
    this->m_Name = "CRC Error Bomb";
    AddVertex(0.0f, -2.5f, 0.0f);
    AddVertex(2.0f, 0.0f, 0.0f);
    AddVertex(0.0f, 0.0f, 1.33f);
    AddVertex(0.0f, 0.0f, -1.33f);
    AddVertex(0.0f, 2.5f, 0.0f);

    AddTriangle(0, 1, 2);
    AddTriangle(0, 2, 3);
    AddTriangle(0, 3, 1);
    AddTriangle(4, 1, 2);
    AddTriangle(4, 2, 3);
    AddTriangle(4, 3, 1);
    return true;

  case MT_HACK:
    this->m_Name = "H4X0R";
    AddVertex(0.0f, -0.5f, 3.0f);
    AddVertex(1.0f, -0.5f, 0.0f);
    AddVertex(-1.0f, -0.5f, 0.0);
    AddVertex(0.0f, 0.5f, 0.0f);

    AddTriangle(0, 1, 2);
    AddTriangle(0, 2, 3);
    AddTriangle(0, 3, 1);
    AddTriangle(1, 2, 3);
    return true;

  case MT_HACK2:
    this->m_Name = "M3G4 H4X0R d00d!!!";
    AddVertex(0.0f, -1.0f, 4.0f);
    AddVertex(2.0f, -1.0f, 0.0f);
    AddVertex(-2.0f, -1.0f, 0.0);
    AddVertex(0.0f, 1.0f, 0.0f);

    AddTriangle(0, 1, 2);
    AddTriangle(0, 2, 3);
    AddTriangle(0, 3, 1);
    AddTriangle(1, 2, 3);
    return true;
  };

  return false;
}

const std::string & CModel::CMesh::GetName() const {
  return m_Name;
}

const std::vector<glm::vec3>& CModel::CMesh::GetVertices() const {
  return m_Vertices;
}

const std::vector<Uint16>& CModel::CMesh::GetIndices() const {
  return m_Indices;
}

const std::vector<Uint16>& CModel::CMesh::GetIndicesLines() const {
  return m_IndicesLines;
}

Uint16 CModel::CMesh::AddVertex(const float x, const float y, const float z) {
  glm::vec3 vertex(x, y, z);
  return AddVertex(vertex);
}

Uint16 CModel::CMesh::AddVertex(const glm::vec3 & vertex) {
  std::vector<glm::vec3>::iterator it = std::find(m_Vertices.begin(), m_Vertices.end(), vertex);
  if (it != m_Vertices.end())
    return (Uint16)(it - m_Vertices.begin());

  m_Vertices.push_back(vertex);
  return (Uint16)m_Vertices.size() - 1;
}

void CModel::CMesh::AddTriangle(const Uint16 v1, const Uint16 v2, const Uint16 v3) {
  m_Indices.push_back(v1);
  m_Indices.push_back(v2);
  m_Indices.push_back(v3);

  AddLine(v1, v2);
  AddLine(v2, v3);
  AddLine(v3, v1);
}

void CModel::CMesh::AddTriangle(const glm::vec3 & v1, const glm::vec3 & v2, const glm::vec3 & v3) {
  Uint16 iv1 = AddVertex(v1);
  Uint16 iv2 = AddVertex(v2);
  Uint16 iv3 = AddVertex(v3);

  AddTriangle(iv1, iv2, iv3);
}

void CModel::CMesh::AddLine(const Uint16 v1, const Uint16 v2) {
  m_IndicesLines.push_back(v1);
  m_IndicesLines.push_back(v2);
}


//==========================================================================================================


CModel::CModel(const ModelId modelId) : m_ModelId(modelId) {
  CMesh mesh;

  mesh.Generate(modelId);

  m_Name = mesh.GetName();
  m_NumberOfTriangles = mesh.GetIndices().size();
  m_NumberOfLines = mesh.GetIndicesLines().size();

  glGenBuffers(1, &m_VertexBufferId);
  glGenBuffers(1, &m_IndexBufferId);
  glGenBuffers(1, &m_IndexBufferLinesId);

  glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, mesh.GetVertices().size() * sizeof(glm::vec3), &mesh.GetVertices()[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndices().size() * sizeof(Uint16), &mesh.GetIndices()[0], GL_STATIC_DRAW);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferLinesId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndicesLines().size() * sizeof(Uint16), &mesh.GetIndicesLines()[0], GL_STATIC_DRAW);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

CModel::~CModel() {
  glDeleteBuffers(1, &m_VertexBufferId);
  glDeleteBuffers(1, &m_IndexBufferId);
  glDeleteBuffers(1, &m_IndexBufferLinesId);
}

void CModel::Render(const glm::mat4& transform, const glm::vec4& lineColor, const glm::vec4& polyColor) {
  glLoadMatrixf(glm::value_ptr(transform));

  glBindBuffer(GL_ARRAY_BUFFER_ARB, m_VertexBufferId);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(glm::vec3), 0);

  glColor4fv(glm::value_ptr(polyColor));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
  glDrawElements(GL_TRIANGLES, m_NumberOfTriangles, GL_UNSIGNED_SHORT, 0);

  glColor4fv(glm::value_ptr(lineColor));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferLinesId);
  glDrawElements(GL_LINES, m_NumberOfLines, GL_UNSIGNED_SHORT, 0);

  glDisableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
}

const CModel::ModelId CModel::GetId() const {
  return m_ModelId;
}

const std::string CModel::GetName() const {
  return m_Name;
}

//==========================================================================================================

CModelRepository::CModelRepository() {}

CModelRepository::~CModelRepository() {
  Clear();
}

CModel * CModelRepository::Get(const CModel::ModelId modelId) {
  std::map<Uint32, CModel*>::iterator it = m_Models.find(modelId);
  if(it != m_Models.end())
    return it->second;

  CModel* pModel = new CModel(modelId);
  m_Models[modelId] = pModel;
  return pModel;
}

void CModelRepository::Clear() {
  for(std::map<Uint32, CModel*>::iterator it = m_Models.begin(); it != m_Models.end(); it++) {
    delete it->second;
  }
  m_Models.clear();
}
