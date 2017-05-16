#include "stdafx.h"
#include "GameSkyBox.h"

static const glm::vec2 gGridSize(800.0f);
static const glm::uvec2 gGridSteps(40);

CGameSkyBox::CGameSkyBox()
  : mGridTop(gGridSize, gGridSteps, 0.0f)
  , mGridBottom(gGridSize, gGridSteps, 0.0) 
  , mGridColors(0.0f, 1.0f, 0.0f, 1.0f)
{}

void CGameSkyBox::SetDynamicVec(const glm::vec3 & vec) {
  mDynamicVec = vec;
}

void CGameSkyBox::SetSepHeight(const float val) {
  mSepHeight = val;
}

void CGameSkyBox::Update(const float timeDelta) {
  mDynamicPos += mDynamicVec * timeDelta;
  glm::vec2 gridTile = gGridSize / glm::vec2(gGridSteps);
  glm::vec3 gridTileLimit(gridTile.x, 1.0f, gridTile.y);

  mDynamicPos = glm::mod(mDynamicPos, gridTileLimit);
}

void CGameSkyBox::UpdateRender() {}

void CGameSkyBox::Render(const glm::mat4 & transform) const {
  glm::mat4 trans = transform * glm::translate(mStaticPos + mDynamicPos);

  glm::vec3 sep(0.0f, mSepHeight / 2.0f, 0.0f);

  mGridBottom.Render(trans * glm::translate(-sep), mGridColors);
  mGridTop.Render(trans * glm::translate(sep), mGridColors);
}
