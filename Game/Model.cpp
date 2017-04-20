#include "stdafx.h"
#include "Model.h"
#include "GLDefines.h"

CModelData::CModelData() {}

const bool CModelData::Generate(const ModelType type) {
  switch(type) {
  default:
  case ModelType::MT_NONE:
    return false;
  case ModelType::MT_HTTP10:
    //this->m_strModelName = "HTTP v1.0";
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
    break;
  case ModelType::MT_HTTP20:
    //this->m_strModelName = "HTTP v2.0+";
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
    break;
  case ModelType::MT_P2PGNU:
    //this->m_strModelName = "P2P Gnutella";
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
    break;
  case ModelType::MT_P2PGNU2:
    //this->m_strModelName = "P2P Gnutella 2";
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
    break;
  case ModelType::MT_P2PFT:
    //this->m_strModelName = "P2P FastTrack";
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
    break;
  case ModelType::MT_P2PFT20:
    //this->m_strModelName = "P2P FastTrack v2.0";
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
    break;
  case ModelType::MT_P2PEDK2K:
    //this->m_strModelName = "P2P eDonkey 2000";
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
  case ModelType::MT_P2PBT:
    //this->m_strModelName = "P2P BitTorrent";
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
    break;
  case ModelType::MT_DL_PART:
    //this->m_strModelName = "DownLoad Part";
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
    break;
  case ModelType::MT_DL_PART2:
    //this->m_strModelName = "BIG DownLoad Part";
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
    break;
  case ModelType::MT_BOMB:
    //this->m_strModelName = "CRC Error Bomb";
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
    break;
  case ModelType::MT_HACK:
    //this->m_strModelName = "H4X0R";
    AddVertex(0.0f, -0.5f, 3.0f);
    AddVertex(1.0f, -0.5f, 0.0f);
    AddVertex(-1.0f, -0.5f, 0.0);
    AddVertex(0.0f, 0.5f, 0.0f);

    AddTriangle(0, 1, 2);
    AddTriangle(0, 2, 3);
    AddTriangle(0, 3, 1);
    AddTriangle(1, 2, 3);
    break;
  case ModelType::MT_HACK2:
    //this->m_strModelName = "M3G4 H4X0R d00d!!!";
    AddVertex(0.0f, -1.0f, 4.0f);
    AddVertex(2.0f, -1.0f, 0.0f);
    AddVertex(-2.0f, -1.0f, 0.0);
    AddVertex(0.0f, 1.0f, 0.0f);

    AddTriangle(0, 1, 2);
    AddTriangle(0, 2, 3);
    AddTriangle(0, 3, 1);
    AddTriangle(1, 2, 3);
    break;
  };

  CreateIndexLines();

  Type = type;
  return true;
}

void CModelData::AddVertex(const float x, const float y, const float z) {
  Vertices.push_back(glm::vec3(x, y, z));
}

void CModelData::AddTriangle(const Uint16 v1, const Uint16 v2, const Uint16 v3) {
  TriangleIndices.push_back(v1);
  TriangleIndices.push_back(v2);
  TriangleIndices.push_back(v3);
}

void CModelData::CreateIndexLines() {
  for(size_t i = 0; i < TriangleIndices.size(); i += 3) {
    LineIndices.push_back(TriangleIndices[i + 0]);
    LineIndices.push_back(TriangleIndices[i + 1]);

    LineIndices.push_back(TriangleIndices[i + 1]);
    LineIndices.push_back(TriangleIndices[i + 2]);

    LineIndices.push_back(TriangleIndices[i + 2]);
    LineIndices.push_back(TriangleIndices[i + 0]);
  }
}

CModel::CModel()
  : mVertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW)
  , mIndexBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
  , mType(ModelType::MT_NONE)
  , mLineIndicesNumber(0)
  , mTriangleIndicesNumber(0) {

}

CModel::~CModel() {
  Free();
}

const ModelType CModel::GetType() const {
  return mType;
}

const bool CModel::Load(CModelData & data) {
  mVertexBuffer.Load(data.Vertices);

  mLineIndicesNumber = data.LineIndices.size();
  mTriangleIndicesNumber = data.TriangleIndices.size();

  mIndexBuffer.Load(
    sizeof(Uint16) * (mLineIndicesNumber + mTriangleIndicesNumber), nullptr);

  mIndexBuffer.SubLoad(0,
                       sizeof(Uint16) * mTriangleIndicesNumber,
                       cb::vectorptr(data.TriangleIndices));
  mIndexBuffer.SubLoad(sizeof(Uint16) * mTriangleIndicesNumber,
                       sizeof(Uint16) * mLineIndicesNumber,
                       cb::vectorptr(data.LineIndices));

  mType = data.Type;
  return true;
}

void CModel::Free() {
  mTriangleIndicesNumber = 0;
  mLineIndicesNumber = 0;
  mType = ModelType::MT_NONE;
}

void CModel::Render(const glm::vec4& lineColor, const glm::vec4& triColor) {
  mVertexBuffer.Bind();
  mIndexBuffer.Bind();

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, NULL);


  glColor4fv(glm::value_ptr(triColor));
  glDrawElements(GL_TRIANGLES,
                 mTriangleIndicesNumber,
                 GL_UNSIGNED_SHORT,
                 nullptr);

  glColor4fv(glm::value_ptr(lineColor));
  glDrawElements(GL_LINES,
                 mLineIndicesNumber,
                 GL_UNSIGNED_SHORT,
                 reinterpret_cast<void*>(mTriangleIndicesNumber * sizeof(Uint16)));

  glDisableClientState(GL_VERTEX_ARRAY);

  mIndexBuffer.UnBind();
  mVertexBuffer.UnBind();
}

//=================================================

CModelRepository CModelRepository::Instance;

CModelRepository::CModelRepository() {}

CModelRepository::~CModelRepository() {
  Clear();
}

CModel * CModelRepository::GetModel(ModelType type) {
  ModelMapT::iterator it = mModelMap.find(type);
  if(it != mModelMap.end()) {
    return it->second;
  }

  CModelData data;
  if(!data.Generate(type)) {
    return nullptr;
  }

  CModel* pModel = new CModel();
  pModel->Load(data);
  mModelMap[type] = pModel;
  return pModel;
}

void CModelRepository::Clear() {
  for(ModelMapT::iterator it = mModelMap.begin(); it != mModelMap.end(); it++) {
    delete it->second;
  }
  mModelMap.clear();
}

typedef std::map<ModelType, cb::string> _ModelTypeStrMapT;
static _ModelTypeStrMapT gModelTypeMap = {
  {ModelType::MT_NONE, L"None"},
  {ModelType::MT_BOMB, L"Bomb"},
  {ModelType::MT_DL_PART, L"DlPart"},
  {ModelType::MT_DL_PART2, L"DlPart2"},
  {ModelType::MT_HACK, L"Hack"},
  {ModelType::MT_HACK2, L"Hack2"},
  {ModelType::MT_HTTP10, L"Http10"},
  {ModelType::MT_HTTP20, L"Http20"},
  {ModelType::MT_P2PBT, L"P2PBT"},
  {ModelType::MT_P2PEDK2K, L"P2PEDK2K"},
  {ModelType::MT_P2PFT, L"P2PFT"},
  {ModelType::MT_P2PFT20, L"P2PFT20"},
  {ModelType::MT_P2PGNU, L"P2PGNU"},
  {ModelType::MT_P2PGNU2, L"P2PGNU2"},
};

const cb::string toStr(const ModelType type) {
  _ModelTypeStrMapT::iterator it = gModelTypeMap.find(type);
  if(it != gModelTypeMap.end())
    return it->second;

  return L"None";
}

const bool fromStr(const cb::string & text, ModelType & outType) {
  for(_ModelTypeStrMapT::iterator it = gModelTypeMap.begin(); it != gModelTypeMap.end(); it++) {
    if(it->second == text) {
      outType = it->first;
      return true;
    }
  }
  return false;
}
