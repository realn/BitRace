#pragma once

#include "../Common/Vector.h"
#include "Model.h"

class CRacer {
private:
  CModel*				m_cModel;
  float				m_fRotation;
  float				  m_fBitRate;
  float				  m_fXSpeed;
  float				  m_fZSpeed;
  vec3				  m_vVec;
  unsigned				m_dwColor;

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
  void SetColor(unsigned dwColor);
  vec3 GetVec();
  float GetBitRate();
  void ModBitRate(float fBitRate);
  CModel* GetModel();

  void Engine(float fDT);
  void Render();
};