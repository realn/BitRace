#ifndef __BITRACE_BASICFILESYSTEM_H__
#define __BITRACE_BASICFILESYSTEM_H__

#include "FileSystem.h"

class CBasicFileSystem
  : public IFileSystem {
public:
  CBasicFileSystem();
  virtual ~CBasicFileSystem();

  // Inherited via IFileSystem
  virtual const bool Read(const cb::string & filepath, cb::bytevector & data) override;
  virtual const bool Write(const cb::string & filepath, const cb::bytevector & data) override;
  virtual const bool ReadText(const cb::string & filepath, cb::string & text) override;
  virtual const bool WriteText(const cb::string & filepath, const cb::string & text) override;
};

#endif // !__BITRACE_BASICFILESYSTEM_H__

