#include "Log.h"
#include "Files.h"
#include <stdarg.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

CFile Log_fp;

bool Log_Init(std::string filename, std::string InitMsg) {
  if (Log_fp.IsOpened())
    return false;

  if (!Log_fp.Open(filename, "w"))
    return false;

  Log_fp.WriteLine(InitMsg);

  return true;
}

bool Log(std::string str, ...) {
  if (!Log_fp.IsOpened())
    return false;

  char* text;
  int len;
  va_list ap;

  va_start(ap, str);

  len = _vscprintf(str.c_str(), ap) + 1;
  text = new char[len];

  vsprintf_s(text, len, str.c_str(), ap);
  va_end(ap);

  Log_fp.WriteLine(std::string(text));

  delete[] text;

  return true;
}

bool Log_Error(std::string str, ...) {
  char* text;
  int len;
  va_list ap;

  va_start(ap, str);

  len = _vscprintf(str.c_str(), ap) + 1;
  text = new char[len];

  vsprintf_s(text, len, str.c_str(), ap);
  va_end(ap);

  if (Log_fp.IsOpened()) {
    Log_fp.WriteLine(std::string(text));
  }

  MessageBox(NULL, text, "ERROR", MB_OK);

  delete[] text;

  return true;
}

bool Log_Free() {
  return Log_fp.Close();
}
