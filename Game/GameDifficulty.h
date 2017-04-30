#ifndef __BITRACE_GAMEDIFFICULTY_H__
#define __BITRACE_GAMEDIFFICULTY_H__

#include <SDL_types.h>
#include <CBStr/StringEx.h>

class IFileSystem;

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

class CGameDifficultySetting {
private:
  GameDifficultyMapT mMap;
  cb::string mFirst;
  cb::string mCurrent;

public:
  CGameDifficultySetting();
  ~CGameDifficultySetting();

  const bool SetCurrent(const cb::string& id);
  const bool SetFirst(const cb::string& id);
  void SetMap(const GameDifficultyMapT& map);

  const cb::string& GetCurrentId() const;
  const CGameDifficulty& GetCurrent() const;
  const cb::string& GetFirstId() const;

  const bool CanPass(const Uint32 points) const;
  const cb::string GetRandomEntity(const Uint32 rand) const;

  const bool Load(IFileSystem& fs, const cb::string& filePath);
  const bool Save(IFileSystem& fs, const cb::string& filePath) const;

  void Reset();
  void Next();
};

#endif // !__BITRACE_GAMEDIFFICULTY_H__

