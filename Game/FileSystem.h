#ifndef __BITRACE_FILESYSTEM_H__
#define __BITRACE_FILESYSTEM_H__

#include <SDL_types.h>
#include <CBIO/Defines.h>
#include <CBXml/Serialize.h>
#include <CBXml/Document.h>

class IFileSystem {
protected:
  IFileSystem();

public:
  virtual ~IFileSystem();

  virtual const bool Read(const cb::string& filepath, cb::bytevector& data) = 0;
  virtual const bool Write(const cb::string& filepath, const cb::bytevector& data) = 0;

  virtual const bool ReadText(const cb::string& filepath, cb::string& text) = 0;
  virtual const bool WriteText(const cb::string& filepath, const cb::string& text) = 0;

  template<typename _Type>
  const bool ReadXml(const cb::string& filepath, const cb::string& rootName, _Type& obj) {
    cb::CXmlDocument xmlDoc;
    if(!ReadXmlDoc(filepath, xmlDoc)) {
      return false;
    }

    if(xmlDoc.RootNode.GetName() != rootName) {
      return false;
    }

    return cb::ReadXmlObject(xmlDoc.RootNode, obj);
  }
  template<typename _Type>
  const bool WriteXml(const cb::string& filepath, const cb::string& rootName, const _Type& obj) {
    cb::CXmlDocument xmlDoc;
    xmlDoc.RootNode.SetName(rootName);
    if(!cb::WriteXmlObject(xmlDoc.RootNode, obj)) {
      return false;
    }

    return WriteXmlDoc(filepath, xmlDoc);
  }

protected:
  virtual const bool ReadXmlDoc(const cb::string& filepath, cb::CXmlDocument& xmlDoc);
  virtual const bool WriteXmlDoc(const cb::string& filepath, const cb::CXmlDocument& xmlDoc);
};

#endif // !__BITRACE_FILESYSTEM_H__

