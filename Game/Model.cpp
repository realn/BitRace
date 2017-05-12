#include "stdafx.h"
#include "Model.h"
#include "GLDefines.h"
#include "XmlTypes.h"
#include "FileSystem.h"

#include <CBXml/Serialize.h>

CModelData::CModelData::CVertex::CVertex(const glm::vec3& pos)
  : Pos(pos) {}

CModelData::CModelData::CSubset::CSubset(const RenderMode mode)
  : Mode(mode) {}

CModelData::CModelData() {}

CModelData::~CModelData() {}

static const cb::string XML_MODELDATA = L"Mesh";

const bool CModelData::Save(IFileSystem & fs, const cb::string & filePath) const {
  return fs.WriteXml(filePath, XML_MODELDATA, *this);
}

const bool CModelData::Load(IFileSystem & fs, const cb::string & filePath) {
  return fs.ReadXml(filePath, XML_MODELDATA, *this);
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

  CModelData::CSubset& triSubset = data.Subsets[L"Faces"];
  CModelData::CSubset& lineSubset = data.Subsets[L"Lines"];

  mLineIndicesNumber = lineSubset.Indices.size();
  mTriangleIndicesNumber = triSubset.Indices.size();

  mIndexBuffer.Load(
    sizeof(Uint16) * (mLineIndicesNumber + mTriangleIndicesNumber), nullptr);

  mIndexBuffer.SubLoad(0,
                       sizeof(Uint16) * mTriangleIndicesNumber,
                       cb::vectorptr(triSubset.Indices));
  mIndexBuffer.SubLoad(sizeof(Uint16) * mTriangleIndicesNumber,
                       sizeof(Uint16) * mLineIndicesNumber,
                       cb::vectorptr(lineSubset.Indices));

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

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, NULL);

  mIndexBuffer.Bind();
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

CModelRepository::CModelRepository(IFileSystem* pFileSystem)
  : mFileSystem(pFileSystem)
{}

CModelRepository::~CModelRepository() {
  Clear();
}

CModel * CModelRepository::GetModel(const cb::string& filename) {
  ModelMapT::iterator it = mModelMap.find(filename);
  if(it != mModelMap.end()) {
    return it->second;
  }

  CModelData data;
  if(!data.Load(*mFileSystem, filename)) {
    return nullptr;
  }

  CModel* pModel = new CModel();
  pModel->Load(data);
  mModelMap[filename] = pModel;
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


const cb::string toStr(const CModelData::RenderMode mode) {
  switch(mode) {
  case CModelData::RenderMode::Triangles: return L"Triangles";
  case CModelData::RenderMode::Lines:     return L"Lines";
  default:
    return L"None";
  }
}

const bool fromStr(const cb::string& text, CModelData::RenderMode& outMode) {
  if(text == L"Triangles") {
    outMode = CModelData::RenderMode::Triangles;
    return true;
  }
  if(text == L"Lines") {
    outMode = CModelData::RenderMode::Lines;
    return true;
  }
  return false;
}


static const cb::string XML_MODELVERTEX_POSITION = L"Pos";

CB_DEFINEXMLRW(CModelData::CVertex) {
  return
    RWAttribute(XML_MODELVERTEX_POSITION, mObject.Pos);
}


static const cb::string XML_MODELSUBSET_MODE = L"Mode";
static const cb::string XML_MODELSUBSET_INDICES = L"Indices";
static const cb::string XML_MODELSUBSET_IND_SEP = L",";

CB_DEFINEXMLREAD(CModelData::CSubset) {
  if(!GetAttribute(XML_MODELSUBSET_MODE, mObject.Mode)) {
    return false;
  }

  cb::string indlist;
  if(!GetAttribute(XML_MODELSUBSET_INDICES, indlist)) {
    return false;
  }

  cb::strvector list = cb::split(indlist, XML_MODELSUBSET_IND_SEP, true);
  return fromStr(list, mObject.Indices);
}

CB_DEFINEXMLWRITE(CModelData::CSubset) {
  if(!SetAttribute(XML_MODELSUBSET_MODE, mObject.Mode)) {
    return false;
  }

  cb::strvector list = toStr(mObject.Indices);
  cb::string indList = cb::join(list, XML_MODELSUBSET_IND_SEP);
  return SetAttribute(XML_MODELSUBSET_INDICES, indList);
}


static const cb::string XML_MODELDATA_VERTEX = L"Vertex";
static const cb::string XML_MODELDATA_SUBSET = L"Subset";
static const cb::string XML_MODELDATA_SUBSET_KEY = L"Id";

CB_DEFINEXMLRW(CModelData) {
  return
    RWNodeList(mObject.Vertices, XML_MODELDATA_VERTEX) &&
    RWNodeMap(mObject.Subsets, XML_MODELDATA_SUBSET, XML_MODELDATA_SUBSET_KEY);
}

