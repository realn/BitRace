#pragma once

#include <SDL_types.h>
#include <glm/fwd.hpp>
#include <CBStr/Defines.h>
#include <map>

#include "GameObject.h"
#include "GameActor.h"

class CModel;
class CModelRepository;
class IFileSystem;

class CGameWeapon {
public:
  float ProjectileDamage;
  float ProjectileSpeed;
  Uint32 ProjectileNumber;
  glm::vec4 ProjectileColor;

  CGameWeapon();
};

class CGamePlayerType {
public:
  typedef std::map<cb::string, CGamePlayerType> TypeMapT;

  cb::string Name;
  float MaxHealth;
  glm::vec2 Speed;
  glm::vec4 Color;
  cb::string ModelFile;
  CGameWeapon Weapon;

  CGamePlayerType();

  static const bool Save(const TypeMapT& typeMap, 
                         IFileSystem& fs, 
                         const cb::string& filepath);
  static const bool Load(TypeMapT& typeMap,
                         IFileSystem& fs,
                         const cb::string& filepath);
};

class CGamePlayer 
  : public CGameActor
{
private:
  float mRotation;
  CGameWeapon mWeapon;

public:
  static const float sMaxRotation;
  static const float sRotationSpeed;

  CGamePlayer(const CGamePlayerType& playerType);
  ~CGamePlayer();

  void Update(const float timeDelta);
  void Render(const glm::mat4& transform) const;

  void ModRotation(const float rotation);

  const CGameWeapon& GetWeapon() const;
};