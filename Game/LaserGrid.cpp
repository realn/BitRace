#include "stdafx.h"
#include "LaserGrid.h"
#include "GLDefines.h"
#include "MeshFunctions.h"

#include <glm/gtc/type_ptr.hpp>

CLaserGrid::CLaserGrid(const glm::vec2& size, const glm::uvec2& steps, const float height)
  : mVertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW)
  , mIndexBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW) {

  vec3vector	vertices;
  ind16vector	indices;

  glm::uvec2 lines = steps + glm::uvec2(1);
  glm::vec2 lineSize = size / glm::vec2(steps);
  glm::vec2 start = -size / 2.0f;

  vertices.resize(lines.x * lines.y);
  for(size_t i = 0; i < vertices.size(); i++) {
    glm::vec2 pos(i % lines.x, i / lines.y);
    glm::vec2 v = start + pos * lineSize;

    vertices[i] = glm::vec3(v.x, height, v.y);
  }

  mVertexBuffer.Load(vertices);

  for(Uint32 Y = 0; Y < lines.y; Y++) {
    for(Uint32 X = 0; X < lines.x; X++) {
      Uint16 ind = Y * lines.x + X;
      if(X < lines.x - 1) {
        indices.push_back(ind);
        indices.push_back(ind + 1);
      }
      if(Y < lines.y - 1) {
        indices.push_back(ind);
        indices.push_back(ind + lines.x);
      }
    }
  }

  mIndicesNumber = indices.size();

  indices.push_back(0);
  indices.push_back(lines.x - 1);
  indices.push_back(lines.x * lines.y - 1);
  indices.push_back(lines.x * lines.y - 1);
  indices.push_back(lines.x * (lines.y - 1));
  indices.push_back(0);

  mIndexBuffer.Load(indices);
}

CLaserGrid::~CLaserGrid() {}

void CLaserGrid::Render(const glm::mat4& transform, const glm::vec3& color) const {
  glm::vec4 lineColor = glm::vec4(color, 1.0f);
  glm::vec4 quadColor = glm::vec4(color, 0.2f);

  glLoadMatrixf(glm::value_ptr(transform));
  glEnableClientState(GL_VERTEX_ARRAY);

  mVertexBuffer.Bind();
  mIndexBuffer.Bind();

  glColor4fv(glm::value_ptr(lineColor));
  glVertexPointer(3, GL_FLOAT, 0, nullptr);
  glDrawElements(GL_LINES, mIndicesNumber, GL_UNSIGNED_SHORT, nullptr);


  glEnable(GL_BLEND);
  glColor4fv(glm::value_ptr(quadColor));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 
                 reinterpret_cast<void*>(mIndicesNumber * sizeof(Uint16)));
  glDisable(GL_BLEND);

  mIndexBuffer.UnBind();
  mVertexBuffer.UnBind();

  glDisableClientState(GL_VERTEX_ARRAY);
}