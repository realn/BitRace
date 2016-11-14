#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <string>

class CGUI;
class CGUIScreen;

class CGUITextControl;
class CGUIRectControl;

class CGUITextAnimation;
class CGUIFadeAnimation;
class CGUITimer;

class CIntro {
private:
  enum INTRO_STATE;
  CGUI*   m_pGUI;
  CGUIScreen* m_pScreen;
  CGUITextControl* m_pPresentTextControl;
  CGUITextControl* m_pTechText1Control;
  CGUITextControl* m_pTechText2Control;
  CGUIRectControl* m_pLogoControl;

  CGUITextAnimation*  m_pPresentAnim;
  CGUITextAnimation*  m_pTechText1Anim;
  CGUITextAnimation*  m_pTechText2Anim;
  CGUIFadeAnimation*  m_pLogoAnim;

  CGUITimer*  m_pWaitTimer;

  glm::vec2 m_Size;
  Uint32	m_IntroState;
  Uint32	m_LogoTexId;
  Uint32	m_uCharCount;
  bool	  m_IntroEnd;
  std::string m_TextPresent;
  std::string m_TextTech1;
  std::string m_TextTech2;

  bool LoadTexture(std::string file);

public:
  CIntro(CGUI* pGUI, const glm::vec2& size);
  ~CIntro();

  bool Init(const std::string& logoFilename, const glm::vec2& size);
  void Free();

  void Update(float timeDelta);
  void Render();
  void RenderGUI();

  bool IsIntroEnded();
};