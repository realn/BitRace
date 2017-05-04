#include "stdafx.h"
#include "LevelBackground.h"

static const glm::vec2 gGridSize(800.0f);
static const glm::uvec2 gGridSteps(40);

CLevelBackground::CLevelBackground()
  : mGridTop(gGridSize, gGridSteps, 0.0f)
  , mGridBottom(gGridSize, gGridSteps, 0.0)
{}

void CLevelBackground::Update(const float timeDelta) {}

void CLevelBackground::UpdateRender() {}

void CLevelBackground::Render(const glm::mat4 & transform) {}
