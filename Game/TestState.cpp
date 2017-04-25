#include "stdafx.h"
#include "TestState.h"
#include "MeshFunctions.h"
#include "Model.h"

CTestState::CTestState()
  : mRotation(0.0f) {}

CTestState::~CTestState() {}

void CTestState::Update(const float timeDelta) {
  mRotation += timeDelta * 30.0f;
}

void CTestState::UpdateRender(const float timeDelta) {}

void CTestState::Render() const {
  vec3vector verts = {
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(-1.0f, -1.0f, 0.0f),
    glm::vec3(1.0f, -1.0f, 0.0f)
  };

  glm::mat4 proj = glm::perspective(glm::radians(50.0f), 
                                    16.0f / 9.0f, 
                                    1.f, 
                                    1000.0f);

  glm::mat4 trans =
    proj *
    glm::translate(glm::vec3(0.0f, 0.0f, -10.0f)) *
    glm::rotate(glm::radians(mRotation), glm::vec3(0.0f, 1.0f, 0.0f));

  CModel* pModel = CModelRepository::Instance.GetModel(ModelType::MT_DL_PART);

  glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
  glLoadMatrixf(glm::value_ptr(trans));
  glColor4fv(glm::value_ptr(color));
  pModel->Render(color, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));


  //{
  //  glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
  //  glm::mat4 mdl =
  //    trans * glm::translate(glm::vec3(0.0f, 0.0f, 1.0f));

  //  glLoadMatrixf(glm::value_ptr(mdl));
  //  glColor4fv(glm::value_ptr(color));
  //  RenderVectorList(GL_TRIANGLES, verts);
  //}
  //{
  //  glm::vec4 color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
  //  glm::mat4 mdl =
  //    trans * glm::translate(glm::vec3(0.0f, 0.0f, -1.0f));

  //  glLoadMatrixf(glm::value_ptr(mdl));
  //  glColor4fv(glm::value_ptr(color));
  //  RenderVectorList(GL_TRIANGLES, verts);
  //}
}

void CTestState::RenderUI() const {}

const bool CTestState::IsDone() const {
  return false;
}

IEngineState * CTestState::GetNext(CEngine & engine) {
  return nullptr;
}
