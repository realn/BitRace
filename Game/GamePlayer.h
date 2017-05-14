#pragma once

#include <SDL_types.h>
#include <glm/fwd.hpp>
#include <CBStr/Defines.h>
#include <map>

class CModel;
class CModelRepository;
class IFileSystem;

class CGameWeapon {
public:
  float ProjectileDamage;
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

class CGamePlayer {
private:
  cb::string mName;
  cb::string mModelFile;
  float mMaxHealth;
  float mHealth;
  float mRotation;
  glm::vec3 mSpeed;
  glm::vec3 mDirection;
  glm::vec4 mColor;
  CGameWeapon mWeapon;
  CModel* mModel;

public:
  static const float sMaxRotation;
  static const float sRotationSpeed;

  CGamePlayer(const CGamePlayerType& playerType);
  ~CGamePlayer();

  const bool LoadResources(CModelRepository& modelRepo);
  void Free();

  void Update(const float timeDelta);
  void Render(const glm::mat4& transform) const;

  void ModRotation(const float rotation);
  void ModHealth(const float value);

  const glm::vec3& GetDirection() const;
  const float GetHealth() const;
  const float GetMaxHealth() const;

};