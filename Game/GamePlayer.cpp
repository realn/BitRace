#include "stdafx.h"
#include "GamePlayer.h"
#include "GLDefines.h"
#include "Model.h"
#include "XmlTypes.h"
#include "FileSystem.h"
#include "GraphicDefines.h"
#include "GameObjectDefines.h"

const float CGamePlayer::sMaxRotation = 75.0f;
const float CGamePlayer::sRotationSpeed = 120.0f;

static const cb::string XML_GAMEPLAYERTYPEMAP_ROOT = L"PlayerTypes";

CGameWeapon::CGameWeapon()
  : ProjectileDamage(1.0f)
  , ProjectileSpeed(10.0f)
  , ProjectileNumber(1)
  , ProjectileColor(gColorRed) {}

CGamePlayerType::CGamePlayerType()
  : Name(L"Sample")
  , Speed(60.0f, 50.0f)
  , Color(gColorGreen)
  , ModelFile(L"mdl_sample.xml") {}

const bool CGamePlayerType::Save(const TypeMapT& typeMap,
                                 IFileSystem& fs,
                                 const cb::string& filepath) {
  return fs.WriteXml(filepath, XML_GAMEPLAYERTYPEMAP_ROOT, typeMap);
}

const bool CGamePlayerType::Load(TypeMapT& typeMap,
                                 IFileSystem& fs,
                                 const cb::string& filepath) {
  return fs.ReadXml(filepath, XML_GAMEPLAYERTYPEMAP_ROOT, typeMap);
}

CGamePlayer::CGamePlayer(const CGamePlayerType& playerType) 
  : CGameActor(playerType.Name,
               playerType.ModelFile,
               GameObjectType::Player, 
               glm::vec2(), glm::vec2(), 
               playerType.Speed, 
               playerType.Color,
               playerType.MaxHealth,
               1.4f)
  , mRotation(0.0f)
  , mWeapon(playerType.Weapon)
{}

CGamePlayer::~CGamePlayer() {}

void CGamePlayer::Update(const float timeDelta) {
  if (glm::abs(mRotation) > 0.0f)
    mRotation -= sRotationSpeed * (mRotation / sMaxRotation) * timeDelta;
  if (glm::abs(mRotation) > sMaxRotation)
    mRotation = sMaxRotation * glm::sign(mRotation);

  mDir = mSpeed * glm::vec2(-(mRotation / sMaxRotation), 1.0f);
}

void CGamePlayer::Render(const glm::mat4& transform) const {
  glm::mat4 mat =
    transform *
    glm::rotate(glm::radians(mRotation), gAxis3DZ);

  glLoadMatrixf(glm::value_ptr(mat));
  mpModel->Render(mColor, gColorBlack);
}

void CGamePlayer::ModRotation(const float value) {
  mRotation = glm::clamp(mRotation + value, -sMaxRotation, sMaxRotation);
}

const CGameWeapon & CGamePlayer::GetWeapon() const {
  return mWeapon;
}


static const cb::string XML_GAMEWEAPON_PROJECTILEDAMAGE = L"ProjectileDamage";
static const cb::string XML_GAMEWEAPON_PROJECTILESPEED = L"ProjectileSpeed";
static const cb::string XML_GAMEWEAPON_PROJECTILENUMBER = L"ProjectileNumber";
static const cb::string XML_GAMEWEAPON_PROJECTILECOLOR = L"ProjectileColor";

CB_DEFINEXMLRW(CGameWeapon) {
  return
    RWAttribute(XML_GAMEWEAPON_PROJECTILEDAMAGE, mObject.ProjectileDamage) &&
    RWAttribute(XML_GAMEWEAPON_PROJECTILESPEED, mObject.ProjectileSpeed) &&
    RWAttribute(XML_GAMEWEAPON_PROJECTILENUMBER, mObject.ProjectileNumber) &&
    RWAttribute(XML_GAMEWEAPON_PROJECTILECOLOR, mObject.ProjectileColor);
}

static const cb::string XML_GAMEPLAYERTYPE_NAME = L"Name";
static const cb::string XML_GAMEPLAYERTYPE_MAXHEALTH = L"MaxHealth";
static const cb::string XML_GAMEPLAYERTYPE_SPEED = L"Speed";
static const cb::string XML_GAMEPLAYERTYPE_COLOR = L"Color";
static const cb::string XML_GAMEPLAYERTYPE_MODELFILE = L"ModelFile";
static const cb::string XML_GAMEPLAYERTYPE_WEAPON = L"Weapon";

CB_DEFINEXMLRW(CGamePlayerType) {
  return
    RWAttribute(XML_GAMEPLAYERTYPE_NAME, mObject.Name) &&
    RWAttribute(XML_GAMEPLAYERTYPE_MAXHEALTH, mObject.MaxHealth) &&
    RWAttribute(XML_GAMEPLAYERTYPE_SPEED, mObject.Speed) &&
    RWAttribute(XML_GAMEPLAYERTYPE_COLOR, mObject.Color) &&
    RWAttribute(XML_GAMEPLAYERTYPE_MODELFILE, mObject.ModelFile) &&
    RWNode(XML_GAMEPLAYERTYPE_WEAPON, mObject.Weapon);
}

static const cb::string XML_GAMEPLAYERTYPEMAP_GAMEPLAYER = L"GamePlayer";
static const cb::string XML_GAMEPLAYERTYPEMAP_KEY = L"Id";

CB_DEFINEXMLRW(CGamePlayerType::TypeMapT) {
  return
    RWNodeMap(mObject, 
              XML_GAMEPLAYERTYPEMAP_GAMEPLAYER, 
              XML_GAMEPLAYERTYPEMAP_KEY);
}
