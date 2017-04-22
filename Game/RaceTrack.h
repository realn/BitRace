#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>

#include "LaserGrid.h"
#include "Racer.h"
#include "GameEntity.h"

class CGameDifficulty {
public:
  typedef std::map<cb::string, Uint32> EntitySpawnMapT;

  cb::string Name;
  float EntitySpawnPause;
  EntitySpawnMapT EntitySpawnRates;
  cb::string NextId;
  Uint32 NextNeededPoints;

  CGameDifficulty();

  const cb::string GetEntity(const Uint32 pos) const;
  const Uint32 GetEntityWeightSum() const;
};
typedef std::map<cb::string, CGameDifficulty> GameDifficultyMapT;

class CGUI;

class CRaceTrack {
private:
  const GameEntityTypeMapT& mEntityTypes;
  CLaserGrid	mGridTop;
  CLaserGrid	mGridBottom;
  GameDifficultyMapT mDifficultyMap;
  CRacer* m_pRacer;

  ProjectileVectorT mProjectiles;
  GameEntityVectorT mEntities;
  ProjectileVertexVectorT mProjectileVertices;

  cb::string mDiffId;
  glm::vec2	m_vMove;
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
  glm::vec3	m_vFSQColor;
  unsigned	m_uPoints;
  unsigned	m_uNeedPoints;
  unsigned	m_uFireCount;
  unsigned	m_uTrackState;
  unsigned	m_unsignedroState;
  unsigned	m_uGameOverCharCount;
  bool	m_bGameOver;
  bool	m_bGameRuning;
  std::string m_strGameOver;

  const glm::vec2 CreateEntityPosition();

  void AddEntity(const cb::string& typeId);

  void Engine_Intro(float fDT);
  void Engine_Track(float fDT);
  void Engine_GameOver(float fDT);

  void Render_Intro(const glm::mat4& transform) const;
  void Render_Track(const glm::mat4& transform) const;
  void Render_GameOver(const glm::mat4& transform) const;

  void CheckDifLevel();
  void SetUpgScreen(float fTimeOut);
  void SetFSQ(float fTimeOut, glm::vec3 vColor);
  void GenRandomObject();
  void Clear();
  const ModelType GetLevelModelType() const;
public:
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
  CRaceTrack(const GameEntityTypeMapT& entityTypes);
  ~CRaceTrack();

  void Free();
  bool Init();
  void SetRacer(CRacer* pRacer);
  void ResetGame();

  void Render(const glm::mat4& transform) const;
  void RenderUI(CGUI& gui) const;
  void Update(const float timeDelta);

  void FireWeapon();
  unsigned GetPoints();
  void SetPoints(unsigned uPoints);
  void SkipIntro();
  bool IsGameOver() const;
  bool IsGameRuning();

private:
  void UpdateEntities(const float timeDelta);
  void UpdateProjectiles(const float timeDelta);
  void UpdateRenderProjectiles();
  void RenderProjectiles(const glm::mat4& transform) const;
};
