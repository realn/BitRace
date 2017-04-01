#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <string>

class CGUI;
class CGUIScreen;

class CGUITextControl;
class CGUIRectControl;

class CGUIController;
class CGUIControllerList;

class CIntro {
private:
  CGUI*   m_pGUI;
  CGUIScreen* m_pScreen;

  CGUIController* m_pAnimation;

  glm::vec2 m_Size;
  Uint32	m_LogoTexId;
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