//#pragma once
//
//#include <SDL_types.h>
//#include <glm/glm.hpp>
//
//#include "GraphicView.h"
//#include "LogicProcess.h"
//
//#include "Space.h"
//#include "Racer.h"
//
//class CGUI;
//
//class CRaceTrack
//  : public ILogicProcess
//  , public IGraphicView {
//public:
//
//  class CShot {
//  private:
//    glm::vec2	m_vVec;
//    glm::vec2	m_vPos;
//    glm::vec3	m_vColor;
//    float	m_fDamage;
//    bool	m_bCanDelete;
//  public:
//    CShot(glm::vec2 vVec, glm::vec2 vStartPos, glm::vec3 vColor, float fDamage);
//    void Engine(float fDT);
//    void SetRender(glm::vec3* vpLine, glm::vec3* vpColor);
//    glm::vec2 GetPos() const;
//    glm::vec2 GetVec() const;
//    float GetDamage() const;
//    void SetCanDelete(bool bSet);
//    bool GetCanDelete();
//  };
//
//  class CEntity {
//  private:
//    CModel*	m_Model;
//    glm::vec2	m_vPos;
//    glm::vec2	m_vVec;
//    glm::vec3	m_vColor;
//    float	m_fValue;
//    float	m_fHealth;
//    float	m_fTemp;
//    unsigned	m_uType;
//    ModelType	mModelType;
//    bool	m_bCanDelete;
//  public:
//    enum ENTITY_TYPE {
//      ET_NONE = 0,
//      ET_BONUS = 1,
//      ET_ENEMY = 2
//    };
//    CEntity(glm::vec2 vPos, glm::vec2 vVec, glm::vec3 vColor, const ModelType modelType);
//    bool	Engine(float fDT, float fRacerPosX, CShot** aShotList, const unsigned uShotCount);
//    void	Render(const glm::mat4& transform) const;
//    float	GetValue();
//    float	GetHealth();
//    unsigned	GetType();
//    const ModelType	GetModelType() const;
//    glm::vec2	GetPos();
//    void SetCanDelete(bool bSet);
//    bool GetCanDelete();
//  };
//private:
//  CSpace	m_SpaceSky;
//  CSpace	m_SpaceGround;
//  CRacer* m_pRacer;
//
//  std::vector<CShot*> m_aShotList;
//  std::vector<glm::vec3>	m_avShotRenderList;
//  std::vector<glm::vec3>	m_avShotRenderColorList;
//  std::vector<CEntity*>	m_aEntityList;
//
//  glm::vec2	m_vMove;
//  float	m_fMoveX;
//  float	m_fTime;
//  float	m_fDamage;
//  float	m_fUpgTimeOut;
//  float	m_fUpgTime;
//  float	m_fFSQTimeOut;
//  float	m_fFSQTime;
//  float	m_fIntroTime;
//  float	m_fGameOverTime;
//  float	m_fGameOverTime2;
//  glm::vec3	m_vFSQColor;
//  unsigned	m_uPoints;
//  unsigned	m_uNeedPoints;
//  unsigned	m_uDifLevel;
//  unsigned	m_uFireCount;
//  unsigned	m_uTrackState;
//  unsigned	m_unsignedroState;
//  unsigned	m_uGameOverCharCount;
//  bool	m_bGameOver;
//  bool	m_bGameRuning;
//  std::string m_strGameOver;
//
//  const glm::vec2 CreateEntityPosition();
//
//  void AddEntity(const glm::vec2& vec, const glm::vec3& color, const ModelType type);
//  void AddEntity_DL();
//  void AddEntity_DL2();
//  void AddEntity_BOMB();
//  void AddEntity_HACK();
//  void AddEntity_HACK2();
//
//  void Engine_Entity(float fDT);
//  void Engine_Shot(float fDT);
//  void Engine_Intro(float fDT);
//  void Engine_Track(float fDT);
//  void Engine_GameOver(float fDT);
//
//  void Render_Intro(const glm::mat4& transform) const;
//  void Render_Track(const glm::mat4& transform) const;
//  void Render_GameOver(const glm::mat4& transform) const;
//
//  void CheckDifLevel();
//  void SetUpgScreen(float fTimeOut);
//  void SetFSQ(float fTimeOut, glm::vec3 vColor);
//  void GenRandomObject();
//  void DeleteShot(size_t i);
//  void Clear();
//  const std::string GetDifLevelString() const;
//  const ModelType GetLevelModelType() const;
//public:
//  enum DIF_LEVEL {
//    DL_VERY_EASY = 0,
//    DL_EASY = 1,
//    DL_MEDIUM = 2,
//    DL_HARD = 3,
//    DL_VERY_HARD = 4,
//    DL_HOLY_SHIT = 5
//  };
//  enum TRACK_STATE {
//    TS_NONE = 0,
//    TS_INTRO = 1,
//    TS_GAME = 3,
//    TS_GAMEOVER = 2
//  };
//  enum INTRO_STATE {
//    IS_STATE1 = 0,
//    IS_STATE2 = 1,
//    IS_STATE3 = 2,
//    IS_STATE4 = 3,
//    IS_ENDSTATE = 4,
//    IS_SKIP = 5
//  };
//  CRaceTrack();
//  ~CRaceTrack();
//
//  void Free();
//  bool Init();
//  void SetRacer(CRacer* pRacer);
//  void ResetGame();
//
//  void Render(const glm::mat4& transform) const override;
//  void RenderUI(CGUI& gui) const override;
//  void Update(const float timeDelta);
//
//  void FireWeapon();
//  unsigned GetDifLevel();
//  void SetDifLevel(unsigned uDifLevel);
//  unsigned GetPoints();
//  void SetPoints(unsigned uPoints);
//  void SkipIntro();
//  bool IsGameOver() const;
//  bool IsGameRuning();
//
//private:
//  void RenderProjectiles(const glm::mat4& transform) const;
//};
