#ifndef __BITRACE_INPUTBINDINGSXML_H__
#define __BITRACE_INPUTBINDINGSXML_H__

#include <SDL_types.h>
#include <glm/fwd.hpp>

#include <CBXml/Serialize.h>

#include "InputBindings.h"

CB_DEFINEXMLREAD(CInputBindings::CBinding);
CB_DEFINEXMLWRITE(CInputBindings::CBinding);
CB_DEFINEXMLREAD(CInputBindings);
CB_DEFINEXMLWRITE(CInputBindings);

#endif // !__BITRACE_INPUTBINDINGSXML_H__

