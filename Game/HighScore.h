#pragma once

#include "GUI.h"
#include "../Common/Files.h"
#include <sstream>

#include <SDL.h>

class CGame;

class CHighScore {
public:
  class CScore {
  public:
    unsigned m_uScore;
    char m_acName[7];

    CScore();
  };
private:
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
  CHighScore();

  bool	LoadScores(std::string strFile);
  bool	SaveScores(std::string strFile);

  void	SetTempScore(unsigned uScore);

  void	Update(CGame* pGame, float fDT);
  void	RenderGUI(CGUI* GUI);

  std::string GetName(unsigned uIndex);
  unsigned	GetScore(unsigned uIndex);

  bool	IsEnded();
  void	ResetAllScores();
};