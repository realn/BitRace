#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>

class CModel;
class CModelRepository;

class CRacer {
private:
  CModel*		m_pModel;
  Uint32    m_ModelId;
  float			m_Rotation;
  float			m_BitRate;
  glm::vec3 m_Speed;
  glm::vec3	m_Vec;
  glm::vec4	m_Color;

public:
  static float	s_ConstHeight;
  static float	s_MaxRotation;
  static float	s_MaxBitRate;

  CRacer(const Uint32 modelId, CModelRepository* pModelRepo);
  ~CRacer();

  void Update(const float timeDelta);
  void Render();

  void ModRotation(float rotation);
  void SetRotation(float rotation);
  void SetColor(const glm::vec4& color);
  const glm::vec3 GetVec() const;
  const float GetBitRate() const;
  void ModBitRate(const float bitRate);
  CModel* GetModel() const;
};