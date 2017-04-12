#ifndef __BITRACE_MESHFUNCTIONS_H__
#define __BITRACE_MESHFUNCTIONS_H__

#include <glm/glm.hpp>
#include <vector>

typedef std::vector<glm::vec3> vec3vector;

extern void RenderVectorList(const Uint32 mode, const vec3vector& vecList);

#endif // !__BITRACE_MESHFUNCTIONS_H__

