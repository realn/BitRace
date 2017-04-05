#pragma once

#include "GUI.h"
#include <CBIO/Defines.h>

class CIntro {
private:
  enum INTRO_STATE {
    IS_STATE1 = 0,
    IS_STATE2,
    IS_STATE3,
    IS_STATE4,
    IS_STATE5,
    IS_STATE6,
    IS_STATE7,
    IS_STATE8,
    IS_STATE9,
    IS_STATE10,
    IS_STATE11,
    IS_STATE12,
    IS_STATE13
  };
  unsigned	m_IntroState;
  unsigned	m_uLogosTex;
  unsigned	m_uCharCount;
  float	m_fTime;
  bool	m_bIntroEnd;
  std::string m_strText1;
  std::string m_strText2;
  std::string m_strText3;

  const bool LoadTexture(const cb::string& filepath);
  void RenderLogo(unsigned index);

public:
  CIntro();
  ~CIntro();

  const bool Init(const cb::string& logosFilepath);
  void Free();

  void Engine(float fDT);
  void Render();
  void RenderGUI(CGUI* GUI);

  bool IsIntroEnded();
};