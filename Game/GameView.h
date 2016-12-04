#pragma once

#include <SDL_types.h>
#include <SDL_video.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class CIniFile;

class CGameView {
public:
  struct	SCREENPARAMS {
    unsigned	uWidth;
    unsigned	uHeight;
    unsigned	uColorBits;
    unsigned	uRefreshRate;

    unsigned	uDevID;

    bool	bSmoothLines;
    bool	bSmoothShade;
    bool	bFullscreen;
    bool	bFPSCount;
    bool	bVSync;
    bool	bBlur;
  };

private:
  SCREENPARAMS m_ScrParam;

  SDL_Window*   m_pWindow;
  SDL_GLContext m_pGLContext;

  SDL_DisplayMode m_ModeOryginal;
  std::vector<SDL_DisplayMode> m_ModeList;

public:
  CGameView(CIniFile& ini);
  ~CGameView();

  const bool Create(const std::string& title);
  void Destroy();

  const bool ScanDispModes();
  const bool ChangeDispMode();

  const bool GetDispMode(const Uint32 id, SDL_DisplayMode& outDispMode) const;

  void Minimize();
  void Restore();

  SCREENPARAMS& GetParams();
  const SCREENPARAMS& GetParams() const;
  const glm::vec2 GetSize() const;
  const float GetAspectRatio() const;

  void Swap();
};