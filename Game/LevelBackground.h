#ifndef __BITRACE_LEVELBACKGROUND_H__
#define __BITRACE_LEVELBACKGROUND_H__

#include "LaserGrid.h"

class CLevelBackground {
private:
  CLaserGrid mGridTop;
  CLaserGrid mGridBottom;
  glm::vec3 mStaticPos;
  glm::vec3 mDynamicPos;
  glm::vec3 mDynamicVec;

public:
  CLevelBackground();

  void Update(const float timeDelta);
  void UpdateRender();
  void Render(const glm::mat4& transform);
};

#endif // !__BITRACE_LEVELBACKGROUND_H__
