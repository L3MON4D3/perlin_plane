#include "Util.hpp"

#include "bgfx/bgfx.h"
#include "bx/math.h"
#include <cstdio>
#include <fstream>

namespace worldWp {
namespace util {

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
void add_normal(worldWp::PosNormalColorVertex *vert_target,
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

};
};
