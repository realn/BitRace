#include "GameView.h"
#include "IniFiles.h"
#include "Log.h"

#include <SDL.h>

CGameView::CGameView(CIniFile& ini) :
  m_pWindow(nullptr),
  m_pGLContext(nullptr) 
{
  if(SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
    Log_Error("Failed initializing SDL.");
    throw std::exception("Failed initializing SDL VIDEO.");
  }

  m_ScrParam.uWidth = Uint32(ini.Read("GRAPHIC", "uWidth", 640));
  m_ScrParam.uHeight = Uint32(ini.Read("GRAPHIC", "uHeight", 480));
  m_ScrParam.uColorBits = Uint32(ini.Read("GRAPHIC", "uColorBits", 32));
  m_ScrParam.uRefreshRate = Uint32(ini.Read("GRAPHIC", "uRefreshRate", 60));
  m_ScrParam.bFullscreen = ini.Read("GRAPHIC", "bFullscreen", true);
  m_ScrParam.bSmoothShade = ini.Read("GRAPHIC", "bSmoothShade", true);
  m_ScrParam.bSmoothLines = ini.Read("GRAPHIC", "bSmoothLines", true);
  m_ScrParam.bFPSCount = ini.Read("GRAPHIC", "bFPSCount", false);
  m_ScrParam.bVSync = ini.Read("GRAPHIC", "bVSync", true);
  m_ScrParam.bBlur = ini.Read("GRAPHIC", "bBlur", false);
}

CGameView::~CGameView() {
  Destroy();

  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

const bool CGameView::Create(const std::string& title) {
  int winFlags = SDL_WINDOW_OPENGL;
  if(m_ScrParam.bFullscreen) {
    winFlags |= SDL_WINDOW_FULLSCREEN;
    winFlags |= SDL_WINDOW_BORDERLESS;
  }
  this->m_pWindow = SDL_CreateWindow(title.c_str(),
                                     SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                     int(m_ScrParam.uWidth), int(m_ScrParam.uHeight),
                                     winFlags);
  if(this->m_pWindow == nullptr) {
    Log_Error("Failed to create window.");
    return false;
  }

  SDL_ShowWindow(this->m_pWindow);

  if(m_ScrParam.bFullscreen)
    ChangeDispMode();

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, int(m_ScrParam.uColorBits));
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

  this->m_pGLContext = SDL_GL_CreateContext(this->m_pWindow);
  if(this->m_pGLContext == nullptr) {
    Log_Error("Failed to create OpenGL Context.");
    return false;
  }

  SDL_GL_MakeCurrent(this->m_pWindow, this->m_pGLContext);
  return true;
}

void CGameView::Destroy() {
  if(this->m_pGLContext) {
    SDL_GL_DeleteContext(this->m_pGLContext);
    this->m_pGLContext = nullptr;
  }

  if(m_ScrParam.bFullscreen) {
    SDL_SetWindowDisplayMode(this->m_pWindow, &this->m_ModeOryginal);
  }

  if(this->m_pWindow) {
    SDL_DestroyWindow(this->m_pWindow);
    this->m_pWindow = nullptr;
  }
}

const bool CGameView::ScanDispModes() {
  SDL_DisplayMode	curDispMode;
  memset(&curDispMode, 0, sizeof(SDL_DisplayMode));

  if(SDL_GetCurrentDisplayMode(0, &curDispMode) != 0)
    return false;

  int dispNum = SDL_GetNumDisplayModes(0);
  for(int i = 0; i < dispNum; i++) {
    SDL_DisplayMode mode;
    memset(&mode, 0, sizeof(SDL_DisplayMode));
    if(SDL_GetDisplayMode(0, i, &mode) != 0) {
      continue;
    }

    if(mode.format != curDispMode.format || mode.refresh_rate != curDispMode.refresh_rate) {
      continue;
    }

    if(mode.w == curDispMode.w && mode.h == curDispMode.h) {
      m_ScrParam.uDevID = i;
    }

    this->m_ModeList.push_back(mode);
  }

  return true;
}

const bool CGameView::ChangeDispMode() {
  SDL_DisplayMode mode;

  if(SDL_GetCurrentDisplayMode(0, &mode) != 0)
    return false;

  mode.w = m_ScrParam.uWidth;
  mode.h = m_ScrParam.uHeight;

  if(SDL_SetWindowDisplayMode(this->m_pWindow, &mode) != 0) {
    Log_Error("Can't change display settings to %ux%u", m_ScrParam.uWidth, m_ScrParam.uHeight);
    return false;
  }
  return true; 
}

const bool CGameView::GetDispMode(const Uint32 id, SDL_DisplayMode & outDispMode) const {
  if(id >= m_ModeList.size())
    return false;

  outDispMode = m_ModeList[id];
  return true;
}

void CGameView::Minimize() {
  SDL_MinimizeWindow(this->m_pWindow);

  if(m_ScrParam.bFullscreen)
    SDL_SetWindowDisplayMode(this->m_pWindow, &this->m_ModeOryginal);
}

void CGameView::Restore() {
  if(m_ScrParam.bFullscreen)
    this->ChangeDispMode();
}

CGameView::SCREENPARAMS & CGameView::GetParams() {
  return m_ScrParam;
}

const CGameView::SCREENPARAMS & CGameView::GetParams() const {
  return m_ScrParam;
}

const glm::vec2 CGameView::GetSize() const {
  return glm::vec2(m_ScrParam.uWidth, m_ScrParam.uHeight);
}

const float CGameView::GetAspectRatio() const {
  glm::vec2 size = GetSize();
  return size.x / size.y;
}

void CGameView::Swap() {
  if(m_pWindow)
    SDL_GL_SwapWindow(m_pWindow);
}
