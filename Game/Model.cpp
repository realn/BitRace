#include "stdafx.h"
#include "Model.h"
#include "GLDefines.h"

CModel	g_Model[MODELTYPE_COUNT];

CModel::CModel() : m_uModelType(0) {

}

CModel::~CModel() {
  Free();
}

void CModel::Free() {
  m_afVertex.clear();
  m_auIndexTriangles.clear();
  m_auIndexLines.clear();
  m_strModelName = "";
  m_uModelType = MT_NONE;
  if (glIsBufferARB(m_uVBOVertex))
    glDeleteBuffersARB(1, &m_uVBOVertex);
}

bool CModel::Generate(unsigned uModelType) {
  switch (uModelType) {
  default:
  case MT_NONE:
    return false;
  case MT_HTTP10:
    this->m_strModelName = "HTTP v1.0";
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
  case MT_HTTP20:
    this->m_strModelName = "HTTP v2.0+";
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
  case MT_P2PGNU:
    this->m_strModelName = "P2P Gnutella";
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
  case MT_P2PGNU2:
    this->m_strModelName = "P2P Gnutella 2";
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
  case MT_P2PFT:
    this->m_strModelName = "P2P FastTrack";
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
  case MT_P2PFT20:
    this->m_strModelName = "P2P FastTrack v2.0";
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
  case MT_P2PEDK2K:
    this->m_strModelName = "P2P eDonkey 2000";
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
    this->m_strModelName = "P2P BitTorrent";
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
  case MT_DL_PART:
    this->m_strModelName = "DownLoad Part";
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
  case MT_DL_PART2:
    this->m_strModelName = "BIG DownLoad Part";
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
  case MT_BOMB:
    this->m_strModelName = "CRC Error Bomb";
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
  case MT_HACK:
    this->m_strModelName = "H4X0R";
    AddVertex(0.0f, -0.5f, 3.0f);
    AddVertex(1.0f, -0.5f, 0.0f);
    AddVertex(-1.0f, -0.5f, 0.0);
    AddVertex(0.0f, 0.5f, 0.0f);

    AddTriangle(0, 1, 2);
    AddTriangle(0, 2, 3);
    AddTriangle(0, 3, 1);
    AddTriangle(1, 2, 3);
    break;
  case MT_HACK2:
    this->m_strModelName = "M3G4 H4X0R d00d!!!";
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

  this->m_uModelType = uModelType;
  CreateIndexLines();

  glGenBuffersARB(1, &m_uVBOVertex);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_uVBOVertex);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_afVertex.size() * sizeof(float), &m_afVertex[0], GL_STATIC_DRAW_ARB);

  return true;
}

void CModel::AddVertex(float x, float y, float z) {
  m_afVertex.push_back(x);
  m_afVertex.push_back(y);
  m_afVertex.push_back(z);
}

void CModel::AddTriangle(unsigned v1, unsigned v2, unsigned v3) {
  m_auIndexTriangles.push_back(v1);
  m_auIndexTriangles.push_back(v2);
  m_auIndexTriangles.push_back(v3);
}

void CModel::CreateIndexLines() {
  size_t i;
  for (i = 0; i < m_auIndexTriangles.size(); i += 3) {
    m_auIndexLines.push_back(m_auIndexTriangles[i]);
    m_auIndexLines.push_back(m_auIndexTriangles[i + 1]);
    m_auIndexLines.push_back(m_auIndexTriangles[i + 1]);
    m_auIndexLines.push_back(m_auIndexTriangles[i + 2]);
    m_auIndexLines.push_back(m_auIndexTriangles[i + 2]);
    m_auIndexLines.push_back(m_auIndexTriangles[i]);
  }
}

void CModel::Render() {
  if (m_afVertex.size() == 0 || m_auIndexTriangles.size() == 0 || m_auIndexLines.size() == 0)
    return;
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_uVBOVertex);
  glVertexPointer(3, GL_FLOAT, 0, NULL);

  glPushAttrib(GL_CURRENT_BIT);
  glColor3f(0.0f, 0.0f, 0.0f);
  glDrawElements(GL_TRIANGLES, (GLsizei)m_auIndexTriangles.size(), GL_UNSIGNED_INT, &m_auIndexTriangles[0]);
  glPopAttrib();
  glDrawElements(GL_LINES, (GLsizei)m_auIndexLines.size(), GL_UNSIGNED_INT, &m_auIndexLines[0]);

  glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
  glDisableClientState(GL_VERTEX_ARRAY);
}

unsigned CModel::GetModelType() {
  return m_uModelType;
}

//=================================================

bool CModel::InitModels() {
  for (unsigned i = 0; i < MODELTYPE_COUNT; i++)
    g_Model[i].Generate(i);

  glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

  return true;
}

void CModel::FreeModels() {
  for (unsigned i = 0; i < MODELTYPE_COUNT; i++)
    g_Model[i].Free();
}

CModel* CModel::GetModel(unsigned uModelType) {
  if (uModelType > MODELTYPE_COUNT) return NULL;
  return &g_Model[uModelType];
}