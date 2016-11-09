#pragma once

#include "Space.h"
#include "Racer.h"
#include "GUI.h"

class CRaceTrack {
public:
  class CShot {
  private:
    vec2	m_vVec;
    vec2	m_vPos;
    vec3	m_vColor;
    float	m_fDamage;
    bool	m_bCanDelete;
  public:
    CShot(vec2 vVec, vec2 vStartPos, vec3 vColor, float fDamage);
    void Engine(float fDT);
    void SetRender(vec3* vpLine, vec3* vpColor);
    vec2 GetPos();
    vec2 GetVec();
    float GetDamage();
    void SetCanDelete(bool bSet);
    bool GetCanDelete();
  };
  class CEntity {
  private:
    CModel*	m_Model;
    vec2	m_vPos;
    vec2	m_vVec;
    vec3	m_vColor;
    float	m_fValue;
    float	m_fHealth;
    float	m_fTemp;
    UINT	m_uType;
    UINT	m_uModelType;
    bool	m_bCanDelete;
  public:
    enum ENTITY_TYPE {
      ET_NONE = 0,
      ET_BONUS = 1,
      ET_ENEMY = 2
    };
    CEntity(vec2 vPos, vec2 vVec, vec3 vColor, UINT uModelType);
    bool	Engine(float fDT, float fRacerPosX, CShot** aShotList, const UINT uShotCount);
    void	Render();
    float	GetValue();
    float	GetHealth();
    UINT	GetType();
    UINT	GetModelType();
    vec2	GetPos();
    void SetCanDelete(bool bSet);
    bool GetCanDelete();
  };
private:
  CSpace	m_SpaceSky;
  CSpace	m_SpaceGround;
  CRacer* m_pRacer;

  std::vector<CShot*> m_aShotList;
  std::vector<vec3>	m_avShotRenderList;
  std::vector<vec3>	m_avShotRenderColorList;
  std::vector<CEntity*>	m_aEntityList;

  vec2	m_vMove;
  float	m_fMoveX;
  float	m_fTime;
  float	m_fDamage;
  float	m_fUpgTimeOut;
  float	m_fUpgTime;
  float	m_fFSQTimeOut;
  float	m_fFSQTime;
  float	m_fIntroTime;
  float	m_fGameOverTime;
  float	m_fGameOverTime2;
  vec3	m_vFSQColor;
  UINT	m_uPoints;
  UINT	m_uNeedPoints;
  UINT	m_uDifLevel;
  UINT	m_uFireCount;
  UINT	m_uTrackState;
  UINT	m_uIntroState;
  UINT	m_uGameOverCharCount;
  bool	m_bGameOver;
  bool	m_bGameRuning;
  std::string m_strGameOver;

  void AddEntity_DL();
  void AddEntity_DL2();
  void AddEntity_BOMB();
  void AddEntity_HACK();
  void AddEntity_HACK2();

  void Engine_Entity(float fDT);
  void Engine_Shot(float fDT);
  void Engine_Intro(float fDT);
  void Engine_Track(float fDT);
  void Engine_GameOver(float fDT);

  void Render_Intro();
  void Render_Track();
  void Render_GameOver();

  void CheckDifLevel();
  void SetUpgScreen(float fTimeOut);
  void SetFSQ(float fTimeOut, vec3 vColor);
  void GenRandomObject();
  void DeleteShot(size_t i);
  void Clear();
  std::string GetDifLevelString();
  UINT GetLevelModelType();
public:
  enum DIF_LEVEL {
    DL_VERY_EASY = 0,
    DL_EASY = 1,
    DL_MEDIUM = 2,
    DL_HARD = 3,
    DL_VERY_HARD = 4,
    DL_HOLY_SHIT = 5
  };
  enum TRACK_STATE {
    TS_NONE = 0,
    TS_INTRO = 1,
    TS_GAME = 3,
    TS_GAMEOVER = 2
  };
  enum INTRO_STATE {
    IS_STATE1 = 0,
    IS_STATE2 = 1,
    IS_STATE3 = 2,
    IS_STATE4 = 3,
    IS_ENDSTATE = 4,
    IS_SKIP = 5
  };
  CRaceTrack();
  ~CRaceTrack();

  void Free();
  bool Init();
  void SetRacer(CRacer* pRacer);
  void ResetGame();

  void Render();
  void RenderGUI(CGUI* GUI);
  void Engine(float fDT);

  void FireWeapon();
  UINT GetDifLevel();
  void SetDifLevel(UINT uDifLevel);
  UINT GetPoints();
  void SetPoints(UINT uPoints);
  void SkipIntro();
  bool IsGameOver();
  bool IsGameRuning();
};