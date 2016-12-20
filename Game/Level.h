#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

class CGUI;
class CGUIScreen;

class CGUITextControl;
class CGUIProgressBarControl;
class CGUIRectControl;
class CGUIControllerList;
class CGUIController;

class CSpace;

class CModel;
class CModelRepository;

class CEntityType;
class CEntity;

class CLevel {
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
  enum DifficultyId;

  class CDifficulty;
  class CProjectile;

  class CLineParticle {
  private:
    glm::vec3 m_StartPos;
    glm::vec4 m_StartColor;
    glm::vec3 m_EndPos;
    glm::vec4 m_EndColor;

  public:
    CLineParticle();
    CLineParticle(const CLineParticle& other);
    ~CLineParticle();

    void Set(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);
  };

private:
  CModelRepository* m_pModelRepo;

  CGUIScreen* m_pGUIScreen;
  CGUITextControl*  m_pGUIPoints;
  CGUITextControl*  m_pGUIPointsNeeded;
  CGUITextControl*  m_pGUILevelText;
  CGUIProgressBarControl* m_pGUIHealthBar;
  CGUIRectControl*  m_pGUIHealOverlay;
  CGUIRectControl*  m_pGUIDamageOverlay;
  CGUIControllerList* m_pGUIControllerList;
  CGUIController*   m_pGUIHealController;
  CGUIController*   m_pGUIDamageController;

  CSpace*  m_pSpaceTop;
  CSpace*  m_pSpaceBottom;
  CEntity* m_pPlayer;

  std::map<Uint32, CEntityType*>  m_EntityTypes;
  std::map<Uint32, CDifficulty*>  m_DifficultyLevels;

  std::vector<CProjectile*> m_Projectiles;
  std::vector<CLineParticle>  m_LineParticles;

  std::vector<CEntity*>	m_Entities;

  Uint32  m_LineParticleBufferId;

  glm::vec2	m_vMove;
  float m_WeaponDamage;
  float m_SpawnTime;
  float	m_fIntroTime;
  float	m_fGameOverTime;
  float	m_fGameOverTime2;
  glm::vec3	m_vFSQColor;
  Uint32  m_WeaponNumberOfProjectiles;
  unsigned	m_uPoints;
  unsigned	m_uNeedPoints;
  unsigned	m_uDifLevel;
  unsigned	m_uTrackState;
  unsigned	m_IntroState;
  unsigned	m_uGameOverCharCount;
  bool	m_bGameOver;
  bool	m_bGameRuning;
  std::string m_strGameOver;

public:
  CLevel();
  ~CLevel();

  const bool Init(CGUI* pGUI, const glm::vec2& screenSize);
  void Free();
  void ResetGame();

  void Render(const glm::mat4& transform);
  void RenderGUI(CGUI* GUI);

  void Update(const float timeDelta);

  const CEntity*  GetPlayer() const;
  
  void PlayerModRotation(const float value);
  void PlayerFireWeapon();

  unsigned GetDifLevel();
  void SetDifLevel(unsigned uDifLevel);
  unsigned GetPoints();
  void SetPoints(unsigned uPoints);
  void SkipIntro();
  bool IsGameOver();
  bool IsGameRuning();

private:
  void UpdateGame(const float timeDelta);
  void UpdateEntities(const float timeDelta);
  void UpdateProjectiles(const float timeDelta);
  void UpdateGUI(const float timeDelta);

  void RenderGame(const glm::mat4& transform);
  void RenderSkybox(const glm::mat4& transform);
  void RenderProjectiles(const glm::mat4& transform);

  void CheckCollisions();
  void CheckEntityCollisions(CEntity* pEntity);
  void CheckProjectileCollisions(CEntity* pEntity);
  const bool  ExecuteCollision(CEntity* pEntityA, CEntity* pEntityB);

  const glm::vec2 CreateEntityPosition();

  CEntity* CreateEntity(const Uint32 entityId, const bool randomStartPos = true);

  void Engine_Intro(float fDT);
  void Engine_GameOver(float fDT);

  void Render_Intro();
  void Render_GameOver();

  void CheckDifLevel();
  void GenRandomObject();
  void Clear();
  std::string GetDifLevelString();
  unsigned GetLevelModelType();

  const glm::vec2 GetLevelPos(CEntity* pEntity) const;
};
