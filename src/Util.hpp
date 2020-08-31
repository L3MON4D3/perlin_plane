#include "ModelBuilder.hpp"
#include "bgfx/bgfx.h"

#ifndef UTIL_H_
#define UTIL_H_

namespace worldWp {
namespace util {
    
bgfx::ShaderHandle load_shader(const char *name);
void glfw_errorCallback(int error, const char *description);
void add_normal(worldWp::PosNormalColorVertex *vert_vec, const float* vec_a, const float* vec_b);

};
};


#endif
