#ifndef __BITRACE_CONFIG_H__
#define __BITRACE_CONFIG_H__

#include <SDL_types.h>
#include <glm/glm.hpp>

class IFileSystem;

class CConfig {
public:
  class CScreen {
  public:
    Uint32 Width;
    Uint32 Height;
    Uint32 ColorBits;
    Uint32 RefreshRate;
    Uint32 DevId;
    bool Fullscreen;
    bool VSync;

    CScreen();

    const glm::uvec2 GetSize() const;
    const float GetAspectRatio() const;
  } Screen;

  class CRender {
  public:
    bool	SmoothLines;
    bool	SmoothShade;
    bool	Blur;
    Uint32 BlurTexSize;
    float BlurTexAlpha;

    CRender();
  } Render;

  class CDiag {
  public: 
    bool	FPSCounter;

    CDiag();
  } Diag;

  CConfig();

  const bool Read(IFileSystem& fs, const cb::string& filepath);
  const bool Write(IFileSystem& fs, const cb::string& filepath);
};

#endif // !__BITRACE_CONFIG_H__

