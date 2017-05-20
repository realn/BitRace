#ifndef __BITRACE_GAMEACTOR_H__
#define __BITRACE_GAMEACTOR_H__

#include <SDL_types.h>
#include <glm/fwd.hpp>
#include <CBIO/Ptr.h>

#include "GameObject.h"

class CModel;
class CModelRepository;

class CGameActor
  : public CGameObject {
protected:
  cb::string mName;
  cb::string mModelFile;
  cb::ptr<CModel> mpModel;
  float mHealth;
  float mMaxHealth;
  float mCollisionRadius;

public:
  CGameActor(const cb::string& name,
             const cb::string& modelFile,
             const GameObjectType type,
             const glm::vec2& pos,
             const glm::vec2& dir,
             const glm::vec2& speed,
             const glm::vec4& color,
             const float maxHealth,
             const float collisionRadius);
  virtual ~CGameActor();

  virtual const bool LoadResources(CModelRepository& modelRepository);

  void ModHealth(const float value);
  void SetHealth(const float value);

  const cb::string& GetName() const;
  const float GetHealth() const;
  const float GetMaxHealth() const;
  const float GetCollisionRadius() const;
};

#endif // !__BITRACE_GAMEACTOR_H__

