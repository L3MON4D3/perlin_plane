#include "Util.hpp"

#include "bgfx/bgfx.h"
#include "bx/math.h"
#include <cstdio>
#include <fstream>

namespace worldWp {
namespace util {

void PosNormalColorVertex::init() {
    layout
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
        .end();
}

NoiseMods::NoiseMods(
  float x_stretch,
  float z_stretch,
  const PlaneSpecs& ms,
  const std::function<float(int x, int z)>& res_fill_func,
  std::function<float(float noise)> post_mod
)
	: res_stretch_sz{ms.x_dim*ms.z_dim},
	  x_stretch{x_stretch},
	  z_stretch{z_stretch},
	  res_stretch{ new float[res_stretch_sz] },
	  post_mod{post_mod} {
	
	int indx{0};
	for(int i{0}; i != ms.x_dim; ++i)
		for(int j{0}; j != ms.z_dim; ++j, ++indx)
			res_stretch[indx] = res_fill_func(i, j);
}

NoiseMods::NoiseMods(const NoiseMods& rhs)
	: res_stretch_sz{ rhs.res_stretch_sz },
	  res_stretch{ new float[res_stretch_sz] },
	  x_stretch{ rhs.x_stretch },
	  z_stretch{ rhs.z_stretch },
	  post_mod{ rhs.post_mod } {
	for(int i{0}; i != res_stretch_sz; ++i)
		res_stretch[i] = rhs.res_stretch[i];
}

NoiseMods& NoiseMods::operator=(const NoiseMods &rhs){
	if (res_stretch_sz == rhs.res_stretch_sz)
		for(int i{0}; i != res_stretch_sz; ++i)
			res_stretch[i] = rhs.res_stretch[i];
	else {
		res_stretch_sz = rhs.res_stretch_sz;
		delete[] res_stretch;
		res_stretch = new float[res_stretch_sz];
	}
	
	res_stretch_sz = rhs.res_stretch_sz;
	res_stretch = rhs.res_stretch;
	x_stretch = rhs.x_stretch;
	z_stretch = rhs.z_stretch;
	return *this;
}

NoiseMods::~NoiseMods() {
	delete[] res_stretch;
}

bgfx::ShaderHandle load_shader(const char *name) {
    char *data = new char[2048];
    std::ifstream file;
    size_t fileSize;
    file.open(name);
    if(file.is_open()) {
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        file.read(data, fileSize);
        file.close();
    }
    const bgfx::Memory* mem = bgfx::copy(data,fileSize+1);
    mem->data[mem->size-1] = '\0';
    bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, name);
    return handle;
}

void glfw_errorCallback(int error, const char *description) {
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

bx::Vec3 triangle_normal(bx::Vec3 t, bx::Vec3 a, bx::Vec3 b) {
	return bx::normalize(bx::cross(
		bx::sub(a, t), bx::sub(b, t) ));
}

//adds normal-component to Vertex vert_target.
void add_normal(PosNormalColorVertex *vert_target,
    float const *vert_a, float const *vert_b) {
	
    float *fl_target = (float*) vert_target;
    const bx::Vec3 vc_target {fl_target[0], fl_target[1], fl_target[2]};
    const bx::Vec3 vc_a {vert_a[0], vert_a[1], vert_a[2]};
    const bx::Vec3 vc_b {vert_b[0], vert_b[1], vert_b[2]};

    bx::Vec3 vc_norm { triangle_normal(vc_target, vc_a, vc_b) };

    //account for offset.
    fl_target[3] = vc_norm.x;
    fl_target[4] = vc_norm.y;
    fl_target[5] = vc_norm.z;
}

float get_noise_mdfd(int res_indx, float x, float z, FastNoise fn, const NoiseMods& nm) {
	return nm.post_mod(nm.res_stretch[res_indx]*fn.GetNoise(nm.x_stretch*x, nm.z_stretch*z));
}

};
};
