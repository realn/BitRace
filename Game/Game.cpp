#include "stdafx.h"
#include "Game.h"
#include "BasicFileSystem.h"

CGame::CGame()
  : mpFileSystem(NULL)
  , m_pWindow(NULL)
  , m_pGLContext(NULL)
  , m_GUI()
  , m_MenuMng(&m_GUI)
  , m_bShutdown(false)
  , m_bGamePause(false)
  , m_bTakeScreen(false)
  , m_iLastTick(0)
  , m_iFreq(0)
  , m_fDT(0.0f)
  , m_uGameState(GS_INTRO)
  , m_uBlurTexture(0)
{
  mLogFile.open(L"game.log", std::ios::out | std::ios::trunc);
  mLogger.AddStream(&mLogFile);

  cb::CLogger::SetInstance(&mLogger);

  SDL_Init(0);

  mpFileSystem = new CBasicFileSystem();
}

CGame::~CGame() {
  Free();
  SDL_Quit();

  cb::CLogger::SetInstance(nullptr);

  delete mpFileSystem;
}

int CGame::MainLoop() {
  SDL_Event event;

  while(!m_bShutdown) {

    if(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT)
        this->m_bShutdown = true;
    }

    this->Update();
    this->Render();
  }

  Free();
  return 0;
}

bool CGame::IsKeyboardKeyDown(const SDL_Scancode code) const {
  return this->m_KeyState[code] > 0;
}

bool CGame::IsKeyboardKeyPressed(const SDL_Scancode code) const {
  return this->m_KeyState[code] && this->m_KeyStatePrev[code] == 0;
}

bool CGame::IsMouseButtonDown(const Uint32 button) const {
  return (this->m_MouseButtonState & SDL_BUTTON(button)) > 0;
}

bool CGame::IsMouseButtonPressed(const Uint32 button) const {
  return
    (this->m_MouseButtonState & SDL_BUTTON(button)) &&
    !(this->m_MouseButtonStatePrev & SDL_BUTTON(button));
}

const glm::ivec2 & CGame::GetMousePos() const {
  return this->m_MousePos;
}

const glm::ivec2 CGame::GetMousePosDelta() const {
  return this->m_MousePos - this->m_MousePosPrev;
}

void CGame::SaveConfig() {
  cb::info(cb::format(L"Saving config file to path {0}.", mConfigFilePath));
  if(!mConfig.Write(*mpFileSystem, mConfigFilePath)) {
    cb::error(L"Failed to save config file.");
  }
}

void CGame::LoadConfig() {
  cb::info(cb::format(L"Loading config file from path {0}.", mConfigFilePath));
  if(!mConfig.Read(*mpFileSystem, mConfigFilePath)) {
    cb::error(L"Failed to load config file, reseting fo default.");
    mConfig = CConfig();
  }
}