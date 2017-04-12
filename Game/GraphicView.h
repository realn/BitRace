#ifndef __BITRACE_GRAPHICView_H__
#define __BITRACE_GRAPHICView_H__

#include <SDL_types.h>

class CGUI;

class IGraphicView {
protected:
  IGraphicView();
public:
  virtual ~IGraphicView();

  virtual void Render() const = 0;
  virtual void RenderUI(CGUI& gui) const = 0;
};

#endif // !__BITRACE_GRAPHICView_H__