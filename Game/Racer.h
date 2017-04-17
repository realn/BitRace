#pragma once

#include <glm/glm.hpp>

#include "Model.h"

class CRacer {
private:
  CModel*				m_cModel;
  float				m_fRotation;
  float				  m_fBitRate;
  glm::vec3     m_Speed;
  glm::vec3				  m_vVec;
  glm::vec4 mColor;

public:
  static float		s_fConstHeight;
  static float		s_fMaxRotation;
  static float		s_fMaxBitRate;

  CRacer();
  ~CRacer();

  void Free();
  bool Init(unsigned uModelType);

  void ModRotation(float fRotation);
  void SetRotation(float fRotation);
  void SetColor(const glm::vec4& color);
  glm::vec3 GetVec() const;
  float GetBitRate() const;
  void ModBitRate(float fBitRate);
  CModel* GetModel();

  void Engine(float fDT);
  void Render(const glm::mat4& transform);
};