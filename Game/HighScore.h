#pragma once

#include <SDL_types.h>
#include <CBStr/Defines.h>

#include "GraphicView.h"
#include "LogicProcess.h"

class CGUI;
class CInputDeviceMap;
class CGUIMenuManager;

class CHighScore
  : public ILogicProcess
  , public IGraphicView {
public:
  class CScore {
  public:
    unsigned m_uScore;
    char m_acName[7];

    CScore();
  };
private:
  const CInputDeviceMap& mInput;
  CScore	m_aScore[10];
  unsigned	m_uTempScore;
  unsigned	m_uCurPos;
  unsigned	m_uHSS;
  unsigned	m_uCharCount;
  unsigned		m_uHSPos;
  std::string	m_strTempName;
  std::string m_strText1;
  std::string m_strText2;
  std::string m_strText3;
  float	m_fTime;
  bool	m_bCanWrite;
  bool	m_bIsEnd;
  bool	m_bKeyDown[256];

  void	ParseKey(unsigned key);
  bool	CheckScore();

  enum	HIGHSCORE_STATE {
    HSS_STATE1 = 0,
    HSS_STATE2,
    HSS_STATE3,
    HSS_STATE4,
    HSS_STATE5,
    HSS_STATE6,
    HSS_STATE7,
    HSS_STATE8
  };

public:
  CHighScore(const CInputDeviceMap& input);
  virtual ~CHighScore();

  bool	LoadScores(const cb::string& filepath);
  bool	SaveScores(const cb::string& filepath);

  void	SetTempScore(unsigned uScore);

  void	Update(const float timeDelta) override;
  void  Render() const override;
  void	RenderUI(CGUI& gui) const override;

  std::string GetName(unsigned uIndex) const;
  unsigned	GetScore(unsigned uIndex) const;

  void FillHSMenu(CGUIMenuManager& menuMng) const;

  bool	IsEnded();
  void	ResetAllScores();
};
