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

  virtual void RenderView() const = 0;
  virtual void RenderUI() const = 0;
};

#endif // !__BITRACE_GRAPHICView_H__
