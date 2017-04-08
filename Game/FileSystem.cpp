#include "stdafx.h"
#include "FileSystem.h"

IFileSystem::IFileSystem() {}

IFileSystem::~IFileSystem() {}

const bool IFileSystem::ReadXmlDoc(const cb::string & filepath, cb::CXmlDocument & xmlDoc) {
  cb::string text;
  if(!ReadText(filepath, text)) {
    return false;
  }

  return xmlDoc.Parse(text);
}

const bool IFileSystem::WriteXmlDoc(const cb::string & filepath, const cb::CXmlDocument & xmlDoc) {
  cb::string text = xmlDoc.ToString();
  if(text.empty()) {
    return false;
  }

  return WriteText(filepath, text);
}
