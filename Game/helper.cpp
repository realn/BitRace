#include "stdafx.h"
#include "helper.h"
#include <SDL_types.h>

#include <cstdarg>

const std::string helper::format(const std::string text, ...) {
  if(text.empty())
    return std::string();

  va_list pArg = nullptr;
  Sint32 len = 0;
  std::string result;

  va_start(pArg, text);
  len = _vscprintf(text.c_str(), pArg);

  result.resize(len);
  vsprintf_s(&result[0], len + 1, text.c_str(), pArg);
  va_end(pArg);

  return result;
}
