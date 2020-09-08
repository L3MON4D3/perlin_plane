#ifndef UTIL_H_
#define UTIL_H_

#include "FastNoise.h"
#include "bgfx/bgfx.h"
#include "bx/math.h"

namespace worldWp {
namespace util {

struct NoiseMods {
	float x_stretch,
	      y_stretch,
	      res_stretch;
};

struct PosNormalColorVertex {
    float pos[3];
	float normal[3];
    uint32_t rgba;

    static void init();
    static bgfx::VertexLayout layout;
};
    
bgfx::ShaderHandle load_shader(const char *name);
void glfw_errorCallback(int error, const char *description);
void add_normal(PosNormalColorVertex *vert_vec, const float* vec_a, const float* vec_b);
bx::Vec3 triangle_normal(bx::Vec3 t, bx::Vec3 a, bx::Vec3 b);
float get_noise_mdfd(float x, float y, FastNoise fn, NoiseMods nm);

};
};


#endif
