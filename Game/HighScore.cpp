#include "stdafx.h"
#include "HighScore.h"
#include "Input.h"
#include "Log.h"
#include "Files.h"
#include "helper.h"
#include "GUI.h"
#include "GUIScreen.h"
#include "GUIController.h"
#include "GUIBuilder.h"

struct HSHEADER {
  char	  FILEID[3];
  Uint32	FILEVER;
  Uint32	SCORECOUNT;
};

#define HSFILEID	"HSF"
#define HSFILEVER	100

static const std::string textScore = "YOUR SCORE:    ";
static const std::string textScoreFmt = "YOUR SCORE: %d";
static const std::string textCommentNoPlace = "SORRY, YOU AREN'T ON HIGH SCORES...";
static const std::string textCommentSomePlace = "YOU ARE ON PLACE: ";
static const std::string textCommentThirdPlace = "GOOD. YOU ARE ON THIRD PLACE.";
static const std::string textCommentSecondPlace = "WONDERFUL! YOU ARE ON SECOND PLACE!";
static const std::string textCommentFirstPlace = "OMG! YOU ARE ON FIRST PLACE!";
static const std::string textNamePrompt = "ENTER YOUR NAME: ";


//============================================

CHighScore::CHighScore(CGUI* pGUI, const glm::vec2& screenSize) :
  m_pScreen(nullptr),
  m_pAnimation(nullptr),
  m_uTempScore(0),
  m_uCurPos(0),
  m_uCharCount(0),
  m_strTempName(""),
  m_strText1("YOUR SCORE: "),
  m_strText2("YOU ARE ON POSITION "),
  m_strText3(""),
  m_fTime(0.0f),
  m_bIsEnd(false) {
  glm::vec4 color(0.0f, 1.0f, 0.0f, 1.0f);

  std::string colText = "colText";
  std::string ctrlScoreText = "ctrlScoreText";
  std::string ctrlComment = "ctrlComment";

  m_pScreen = new CGUIScreen(pGUI, screenSize);
  CGUIBuilder gui(m_pScreen);

  gui.Color(colText, color);

  gui.Label(ctrlScoreText, textScore);
  gui.Label(ctrlComment, textCommentNoPlace);

  gui.Put(ctrlScoreText, glm::vec2(0.0f, -20.0f), CGUIScreen::IA_SCREEN_CENTER);
  gui.Put(ctrlComment, glm::vec2(0.0f), CGUIScreen::IA_SCREEN_CENTER);

  gui.SetVisible(ctrlScoreText, false);
  gui.SetVisible(ctrlComment, false);

  CGUIControllerBuilder guic(&gui);
  {
    guic.BeginSequence();

    guic.TextShow(ctrlScoreText, 1.0f);
    guic.TextCountAsc(ctrlScoreText, 1000, textScoreFmt, 2.0f);
    guic.Wait(1.0f);
    guic.TextShow(ctrlComment, 2.0f);
    guic.Wait(1.0f);

    guic.EndSequence();
  }
  
  m_pAnimation = guic.GetResultController();
}

CHighScore::~CHighScore() {
  helper::deleteobj(m_pAnimation);
  helper::deleteobj(m_pScreen);
}

const bool CHighScore::Load(const std::string& filename) {
  if (filename.empty())
    return false;

  CFile fp(filename);
  if (!fp.IsOpened()) {
    Log("Can't find HighScores file %s, NULLing table", filename.c_str());
    m_Scores.clear();
    return true;
  }

  HSHEADER head;
  fp.Read(&head, sizeof(HSHEADER));

  if (strncmp(head.FILEID, HSFILEID, 3) != 0 || head.FILEVER != HSFILEVER || head.SCORECOUNT > 10) {
    Log("Invalid file ID - %s isn't a HSF file", filename.c_str());
    return false;
  }

  m_Scores.resize(head.SCORECOUNT);
  for(std::vector<CScore>::iterator it = m_Scores.begin(); it != m_Scores.end(); it++) {
    fp.Read(&it->m_Points, sizeof(Uint32));
    helper::readstr(fp, it->m_Name);
  }

  Uint32	magicNum = 0;
  fp.Read(&magicNum, sizeof(Uint32));

  Uint32	magicNumCmp = 0;
  for(std::vector<CScore>::iterator it = m_Scores.begin(); it != m_Scores.end(); it++) {
    magicNumCmp += it->m_Points;
    for(Uint32 i = 0; i < it->m_Name.length(); i++)
      magicNumCmp += (Uint32)it->m_Name[i];
  }

  if (magicNum != (Uint32)(~magicNumCmp)) {
    Log("Invalid Check Sum - file corrupted, NULLing table");
    m_Scores.clear();
    return true;
  }

  return true;
}

const bool CHighScore::Save(const std::string& filename) {
  if (filename.empty())
    return false;

  CFile fp(filename, "wb");
  if (!fp.IsOpened())
    return false;

  HSHEADER head;
  strncpy_s(head.FILEID, 4, HSFILEID, 3);
  head.FILEVER = HSFILEVER;
  head.SCORECOUNT = m_Scores.size();

  fp.Write(&head, sizeof(HSHEADER));
  for(std::vector<CScore>::iterator it = m_Scores.begin(); it != m_Scores.end(); it++) {
    fp.Write(&it->m_Points, sizeof(Uint32));
    helper::writestr(fp, it->m_Name);
  }

  Uint32 magicNum = 0;
  for(std::vector<CScore>::iterator it = m_Scores.begin(); it != m_Scores.end(); it++) {
    magicNum += it->m_Points;
    for(Uint32 i = 0; i < it->m_Name.length(); i++)
      magicNum += (Uint32)it->m_Name[i];
  }
  magicNum = (Uint32)~magicNum;
  fp.Write(&magicNum, sizeof(Uint32));

  return true;
}

void CHighScore::StartScoreAnim(const Uint32 score) {
  m_pValueController->SetValue(score);


  m_ScorePoints = score;
  m_ScoreName = "";
  m_ScorePosition = -1;
  this->m_pAnimation->Rerun();
  if(CheckScore()) {

  }
}

void CHighScore::Update(CInput* pInput, float timeDelta) {
  m_pAnimation->Update(timeDelta);

  //switch (m_uHSS) {
  //case HSS_STATE1:
  //  m_fTime += 1.0f * timeDelta;
  //  if (m_fTime > 0.1f) {
  //    if (m_uCharCount < m_strText1.length())
  //      m_uCharCount++;
  //    else {
  //      m_uHSS = HSS_STATE2;
  //      m_uCharCount = 0;
  //    }
  //    m_fTime = 0.0f;
  //  }
  //  break;

  //case HSS_STATE2:
  //  m_fTime += float(m_uTempScore) * 0.3f * timeDelta;
  //  if (m_fTime >= float(m_uTempScore)) {
  //    m_fTime = 0.0f;
  //    m_strText1 += helper::tostr(m_uTempScore);
  //    m_uHSS = HSS_STATE3;
  //  }
  //  break;

  //case HSS_STATE3:
  //  m_fTime += 1.0f * timeDelta;
  //  if (m_fTime > 2.0f) {
  //    if (CheckScore()) {
  //      switch (m_uHSPos) {
  //      case 0:
  //        m_strText2 = "OMG! YOU ARE ON FIRST PLACE!"; break;
  //      case 1:
  //        m_strText2 = "WONDERFUL! YOU ARE ON SECOND PLACE!"; break;
  //      case 2:
  //        m_strText2 = "GOOD. YOU ARE ON THIRD PLACE."; break;
  //      default:
  //        m_strText2 = "YOU ARE ON PLACE: " + helper::tostr(m_uHSPos + 1); break;
  //      };
  //    }
  //    else {
  //      m_strText2 = "SORRY, YOU AREN'T ON HIGH SCORES...";
  //    }
  //    m_fTime = 0.0f;
  //    m_uHSS = HSS_STATE4;
  //  }
  //  break;

  //case HSS_STATE4:
  //  m_fTime += 1.0f * timeDelta;
  //  if (m_fTime > 0.1f) {
  //    if (m_uCharCount < m_strText2.length())
  //      m_uCharCount++;
  //    else {
  //      m_uHSS = HSS_STATE5;
  //      m_uCharCount = 0;
  //    }
  //    m_fTime = 0.0f;
  //  }
  //  break;

  //case HSS_STATE5:
  //  m_fTime += 1.0f * timeDelta;
  //  if (m_fTime > 2.0f) {
  //    m_fTime = 0.0f;
  //    m_uHSS = HSS_STATE6;
  //  }
  //  break;

  //case HSS_STATE6:
  //  m_fTime += 1.0f * timeDelta;
  //  if (m_fTime > 0.1f) {
  //    if (m_uCharCount < m_strText3.length())
  //      m_uCharCount++;
  //    else {
  //      m_uHSS = HSS_STATE7;
  //      m_uCharCount = 0;
  //      pInput->StartTextInput();
  //    }
  //    m_fTime = 0.0f;
  //  }
  //  break;

  //case HSS_STATE7:
  //  m_strTempName = pInput->GetText();
  //  if(pInput->IsKeyboardKeyPressed(SDL_SCANCODE_BACKSPACE)) {
  //    if(m_strTempName.length() > 0)
  //      m_strTempName = m_strTempName.substr(0, m_strTempName.length() - 1);
  //  }
  //  if(pInput->IsKeyboardKeyPressed(SDL_SCANCODE_RETURN)) {
  //    strncpy_s(m_aScore[m_uHSPos].m_acName, 7, m_strTempName.c_str(), 6);
  //    m_uHSS = HSS_STATE8;
  //  }
  //  break;

  //case HSS_STATE8:
  //  pInput->StopTextInput();
  //  m_bIsEnd = true;
  //  m_uHSS = HSS_STATE1;
  //  break;
  //};
}

void CHighScore::RenderGUI(CGUI *GUI) {
  this->m_pScreen->Render();
  //glm::vec4 color(0.0f, 1.0f, 0.0f, 1.0f);

  //std::stringstream ss;
  //switch (m_uHSS) {
  //case HSS_STATE1:
  //  GUI->Print(glm::vec2(200.0f, 200.0f), color, m_strText1.substr(0, m_uCharCount) + "_");
  //  break;

  //case HSS_STATE2:
  //  ss << unsigned(floor(m_fTime));
  //  GUI->Print(glm::vec2(200.0f, 200.0f), color, m_strText1 + ss.str() + "_");
  //  break;

  //case HSS_STATE3:
  //  GUI->Print(glm::vec2(200.0f, 200.0f), color, m_strText1);
  //  break;

  //case HSS_STATE4:
  //  GUI->Print(glm::vec2(200.0f, 200.0f), color, m_strText1);
  //  GUI->Print(glm::vec2(200.0f, 220.0f), color, m_strText2.substr(0, m_uCharCount) + "_");
  //  break;

  //case HSS_STATE5:
  //  GUI->Print(glm::vec2(200.0f, 200.0f), color, m_strText1);
  //  GUI->Print(glm::vec2(200.0f, 220.0f), color, m_strText2);
  //  break;

  //case HSS_STATE6:
  //  GUI->Print(glm::vec2(200.0f, 200.0f), color, m_strText1);
  //  GUI->Print(glm::vec2(200.0f, 220.0f), color, m_strText2);
  //  GUI->Print(glm::vec2(200.0f, 240.0f), color, m_strText3.substr(0, m_uCharCount) + "_");
  //  break;

  //case HSS_STATE7:
  //  GUI->Print(glm::vec2(200.0f, 200.0f), color, m_strText1);
  //  GUI->Print(glm::vec2(200.0f, 220.0f), color, m_strText2);
  //  GUI->Print(glm::vec2(200.0f, 240.0f), color, m_strText3 + m_strTempName + "_");
  //  break;
  //}
}

bool CHighScore::IsEnded() {
  return m_pAnimation->IsDone();
}

bool CHighScore::CheckScore() {
  if(m_ScorePoints == 0) {
    return false;
  }

  m_ScorePosition = -1;
  for(Uint32 i = 0; i < m_Scores.size(); i++) {
    if(m_ScorePoints > m_Scores[i].m_Points) {
      m_ScorePosition = (Sint32)i;
      break;
    }
  }

  if(m_ScorePosition < 0)
    return false;


  CScore score;
  score.m_Points = m_ScorePoints;
  m_Scores.insert(m_Scores.begin() + m_ScorePosition, score);

  if(m_Scores.size() > 10)
    m_Scores.pop_back();

  return true;
}

const std::string CHighScore::GetName(const Uint32 position) const {
  if(position >= m_Scores.size())
    return std::string();
  return m_Scores[position].m_Name;
}

const Uint32 CHighScore::GetScore(const Uint32 position) const {
  if(position >= m_Scores.size())
    return 0;
  return m_Scores[position].m_Points;
}

void CHighScore::ResetAllScores() {
  m_Scores.clear();
}