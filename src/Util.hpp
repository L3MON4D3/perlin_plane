#ifndef UTIL_H_
#define UTIL_H_

#include "FastNoise.h"
#include "bgfx/bgfx.h"
#include "bx/math.h"
#include <functional>

namespace worldWp {
namespace util {

struct PlaneSpecs {
	int x_dim,
	    z_dim,
	    res;
};

struct NoiseMods {
	NoiseMods(
	  float x_stretch,
	  float y_stretch,
	  const PlaneSpecs& ms,
	  const std::function<float(int x, int z)>& res_fill_func,
	  std::function<float(float noise_val)> post_mod
	);
	NoiseMods(const NoiseMods& rhs);

	int res_stretch_sz;
	float x_stretch,
	      z_stretch,
	      *res_stretch;
	std::function<float(float noise_val)> post_mod;
	NoiseMods& operator=(const NoiseMods& rhs);
	~NoiseMods();
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
float get_noise_mdfd(int res_indx, float x, float z, FastNoise fn, const NoiseMods& nm);

};
};

#endif
