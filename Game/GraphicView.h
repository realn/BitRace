#ifndef __BITRACE_GRAPHICView_H__
#define __BITRACE_GRAPHICView_H__

#include <SDL_types.h>
#include <glm/fwd.hpp>

class CGUI;

class IGraphicView {
protected:
  IGraphicView();
public:
  virtual ~IGraphicView();

  virtual void Render(const glm::mat4& transform) const = 0;
  virtual void RenderUI(CGUI& gui) const = 0;
};

#endif // !__BITRACE_GRAPHICView_H__
