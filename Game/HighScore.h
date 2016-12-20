#pragma once

#include <SDL_types.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

class CInput;
class CGUI;
class CGUIScreen;
class CGUIControllerList;
class CGUIController;
template<typename _Type> class CGUITextCountAnimation;
class CGUITextAnimation;

class CHighScore {
public:
  class CScore {
  public:
    Uint32 m_Points;
    std::string m_Name;

    CScore() : m_Points(0) {}
  };
private:
  std::vector<CScore> m_Scores;

  CGUIScreen* m_pScreen;
  CGUIControllerList* m_pControllerList;

  CGUIController* m_pAnimStart;
  CGUIController* m_pAnimEnd;
  CGUITextAnimation* m_pNameAnim;
  CGUITextCountAnimation<Uint32>* m_pScoreAnim;

  Uint32    m_ScorePoints;
  Sint32    m_ScorePosition;
  std::string m_ScoreName;
  std::string m_Comment;

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
  bool	m_bIsEnd;

public:
  CHighScore(CGUI* pGUI, const glm::vec2& screenSize);
  ~CHighScore();

  const bool	Load(const std::string& filename);
  const bool	Save(const std::string& filename);

  void  StartScoreAnim(const Uint32 score);

  void	Update(CInput* pInput, float timeDelta);
  void	RenderGUI(CGUI* GUI);

  const std::string GetName(const Uint32 position) const;
  const Uint32	GetScore(const Uint32 position) const;

  bool	IsEnded();
  void	ResetAllScores();

private:
  bool	CheckScore();
};