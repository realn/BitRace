#include "stdafx.h"
#include "BasicFileSystem.h"

CBasicFileSystem::CBasicFileSystem() {}

CBasicFileSystem::~CBasicFileSystem() {}

const bool CBasicFileSystem::Read(const cb::string & filepath, cb::bytevector & data) {
  return cb::readfile(filepath, data);
}

const bool CBasicFileSystem::Write(const cb::string & filepath, const cb::bytevector & data) {
  return cb::writefile(filepath, data);
}

const bool CBasicFileSystem::ReadText(const cb::string & filepath, cb::string & text) {
  text = cb::readtextfileutf8(filepath);
  return !text.empty();
}

const bool CBasicFileSystem::WriteText(const cb::string & filepath, const cb::string & text) {
  return cb::writetextfileutf8(filepath, text);
}
