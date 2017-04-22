#ifndef __BITRACE_MESHFUNCTIONS_H__
#define __BITRACE_MESHFUNCTIONS_H__

#include <glm/glm.hpp>
#include <vector>

typedef std::vector<glm::vec4> vec4vector;
typedef std::vector<glm::vec3> vec3vector;
typedef std::vector<glm::vec2> vec2vector;
typedef std::vector<Uint16> ind16vector;

extern void RenderVectorList(const Uint32 mode, const Uint32 vFloatNum, 
                             const void* vptr, const Uint32 size);
extern void RenderVectorList(const Uint32 mode, const Uint32 vFloatNum, 
                             const void* vptr, const ind16vector& indices);

extern void RenderVectorList(const Uint32 mode, const vec3vector& vecList);
extern void RenderVectorList(const Uint32 mode, const vec3vector& vecList,
                             const ind16vector& indices);

extern void RenderTexVectorList(const Uint32 mode, 
                                const Uint32 vFloatNum, const void* vptr,
                                const Uint32 texFloatNum, const void* texptr,
                                const Uint32 size);
extern void RenderTexVectorList(const Uint32 mode,
                                const Uint32 vFloatNum, const void* vptr,
                                const Uint32 texFloatNum, const void* texptr,
                                const ind16vector& indices);


extern void RenderTexVectorList(const Uint32 mode, const vec3vector& vecList,
                                const vec2vector& texList);
extern void RenderTexVectorList(const Uint32 mode, const vec3vector& vecList,
                                const vec2vector& texList, ind16vector& indices);

extern void RenderQuad(const glm::vec2& pos, const glm::vec2& size);
extern void RenderTexQuad(const glm::vec2& pos, const glm::vec2& size,
                          const glm::vec2& tpos = glm::vec2(0.0f), 
                          const glm::vec2& tsize = glm::vec2(1.0f));

#endif // !__BITRACE_MESHFUNCTIONS_H__

