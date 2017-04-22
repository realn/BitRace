#pragma once

#include <SDL_types.h>
#include <glm/fwd.hpp>
#include <vector>

#include "GraphicBuffer.h"

class CLaserGrid {
private:
  CGraphicBuffer mVertexBuffer;
  CGraphicBuffer mIndexBuffer;
  Uint32 mIndicesNumber;

public:
  CLaserGrid(const glm::vec2& size, const glm::uvec2& steps, const float height);
  ~CLaserGrid();

  void Render(const glm::mat4& transform, const glm::vec3& color) const;
};