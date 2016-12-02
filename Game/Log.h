#pragma once

#include <string>

extern bool Log_Init(std::string filename, std::string InitMsg);
extern bool Log(std::string str, ...);
extern bool Log_Error(std::string str, ...);
extern bool Log_Free();