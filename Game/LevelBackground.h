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
  glm::vec4 mGridColors;
  float mSepHeight;

public:
  CLevelBackground();

  void SetDynamicVec(const glm::vec3& vec);
  void SetSepHeight(const float val);

  void Update(const float timeDelta);
  void UpdateRender();
  void Render(const glm::mat4& transform) const;
};

#endif // !__BITRACE_LEVELBACKGROUND_H__
