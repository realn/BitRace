#include "stdafx.h"
#include "GameDifficulty.h"
#include "FileSystem.h"

#include <CBXml/Serialize.h>

CGameDifficulty::CGameDifficulty()
  : EntitySpawnPause(0.0f)
  , NextNeededPoints(0) {}

const cb::string CGameDifficulty::GetEntity(const Uint32 pos) const {
  Uint32 weigth = 0;
  for(EntitySpawnMapT::const_iterator it = EntitySpawnRates.begin();
      it != EntitySpawnRates.end(); it++) {
    if(weigth <= pos && weigth + it->second > pos) {
      return it->first;
    }
    weigth += it->second;
  }
  return cb::string();
}

const Uint32 CGameDifficulty::GetEntityWeightSum() const {
  Uint32 weight = 0;
  for(EntitySpawnMapT::const_iterator it = EntitySpawnRates.begin();
      it != EntitySpawnRates.end(); it++) {
    weight += it->second;
  }
  return weight;
}

class CGameDiffData {
public:
  cb::string First;
  GameDifficultyMapT Map;
};
static const cb::string XML_GAMEDIFFICULTYSETTING = L"DifficultySetting";


CGameDifficultySetting::CGameDifficultySetting() {}

CGameDifficultySetting::~CGameDifficultySetting() {}

const bool CGameDifficultySetting::SetCurrent(const cb::string & id) {
  GameDifficultyMapT::iterator it = mMap.find(id);
  if(it != mMap.end()) {
    mCurrent = id;
    return true;
  }
  return false;
}

const bool CGameDifficultySetting::SetFirst(const cb::string & id) {
  GameDifficultyMapT::iterator it = mMap.find(id);
  if(it != mMap.end()) {
    mFirst = id;
    return true;
  }
  return false;
}

void CGameDifficultySetting::SetMap(const GameDifficultyMapT & map) {
  mMap = map;
}

const cb::string & CGameDifficultySetting::GetCurrentId() const {
  return mCurrent;
}

const CGameDifficulty & CGameDifficultySetting::GetCurrent() const {
  return mMap.at(mCurrent);
}

const cb::string & CGameDifficultySetting::GetFirstId() const {
  return mFirst;
}

const bool CGameDifficultySetting::CanPass(const Uint32 points) const {
  const CGameDifficulty& diff = GetCurrent();
  if(diff.NextNeededPoints == 0 || diff.NextId.empty()) {
    return false;
  }
  return points >= diff.NextNeededPoints;
}

const cb::string CGameDifficultySetting::GetRandomEntity(const Uint32 rand) const {
  const CGameDifficulty& diff = GetCurrent();
  Uint32 maxW = diff.GetEntityWeightSum();
  Uint32 pos = rand % maxW;

  return diff.GetEntity(pos);
}

const bool CGameDifficultySetting::Load(IFileSystem & fs, const cb::string & filePath) {
  CGameDiffData data;
  if(!fs.ReadXml(filePath, XML_GAMEDIFFICULTYSETTING, data)) {
    return false;
  }
  mMap = data.Map;
  if(!SetFirst(data.First)) {
    return false;
  }
  Reset();
  return true;
}

const bool CGameDifficultySetting::Save(IFileSystem & fs, const cb::string & filePath) const {
  CGameDiffData data;
  data.Map = mMap;
  data.First = mFirst;
  return fs.WriteXml(filePath, XML_GAMEDIFFICULTYSETTING, data);
}

void CGameDifficultySetting::Reset() {
  mCurrent = mFirst;
}

void CGameDifficultySetting::Next() {
  const CGameDifficulty& diff = GetCurrent();
  if(diff.NextId.empty())
    return;

  mCurrent = diff.NextId;
}

static const cb::string XML_GAMEDIFFICULTY_ID = L"Id";
static const cb::string XML_GAMEDIFFICULTY_NAME = L"Name";
static const cb::string XML_GAMEDIFFICULTY_SPAWNPAUSE = L"EntitySpawnPause";
static const cb::string XML_GAMEDIFFICULTY_SPAWNRATE = L"Spawn";
static const cb::string XML_GAMEDIFFICULTY_SPAWNRATES_ENTID = L"EntityId";
static const cb::string XML_GAMEDIFFICULTY_SPAWNRATES_RATE = L"Rate";
static const cb::string XML_GAMEDIFFICULTY_NEXTID = L"NextId";
static const cb::string XML_GAMEDIFFICULTY_NEXTPOINTS = L"NextNeededPoints";

CB_DEFINEXMLRW(CGameDifficulty) {
  return
    RWAttribute(XML_GAMEDIFFICULTY_NAME, mObject.Name) &&
    RWAttribute(XML_GAMEDIFFICULTY_SPAWNPAUSE, mObject.EntitySpawnPause) &&
    RWNodeMap(mObject.EntitySpawnRates,
              XML_GAMEDIFFICULTY_SPAWNRATE,
              XML_GAMEDIFFICULTY_SPAWNRATES_ENTID,
              XML_GAMEDIFFICULTY_SPAWNRATES_RATE) &&
    RWAttribute(XML_GAMEDIFFICULTY_NEXTID, mObject.NextId) &&
    RWAttribute(XML_GAMEDIFFICULTY_NEXTPOINTS, mObject.NextNeededPoints);
}

static const cb::string XML_GAMEDIFFICULTYSETTING_FIRST = L"FirstId";
static const cb::string XML_GAMEDIFFICULTYSETTING_DIFFICULTY = L"Difficulty";

CB_DEFINEXMLRW(CGameDiffData) {
  return
    RWAttribute(XML_GAMEDIFFICULTYSETTING_FIRST, mObject.First) &&
    RWNodeMap(mObject.Map,
              XML_GAMEDIFFICULTYSETTING_DIFFICULTY,
              XML_GAMEDIFFICULTY_ID);
}
