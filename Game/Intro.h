#pragma once

#include <SDL_types.h>
#include <CBIO/Defines.h>

#include "LogicProcess.h"
#include "GraphicView.h"

class CGUI;

enum class IntroState {
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

class CIntroProcess
  : public ILogicProcess {
private:
  IntroState	mIntroState;
  unsigned	mCharCount;
  float	mTime;
  bool	mIntroEnd;

public:
  CIntroProcess();
  ~CIntroProcess();

  void Update(const float timeDelta) override;

  const IntroState GetState() const;
  const float GetTime() const;
  const Uint32 GetCharNumber() const;

  bool IsIntroEnded();
};

class CIntroView
  : public IGraphicView {
private:
  CIntroProcess& mIntro;
  unsigned	mLogosTex;

public:
  CIntroView(CIntroProcess& intro);
  ~CIntroView();

  const bool Init(const cb::string& logosFilePath);
  void Free();

  void Render() const override;
  void RenderUI(CGUI& gui) const override;

private:
  const bool LoadTexture(const cb::string& filepath);
  void RenderLogo(Uint32 index) const;
};