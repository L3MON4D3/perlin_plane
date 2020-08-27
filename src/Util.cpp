#include "Util.hpp"

#include "bgfx/bgfx.h"
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

};
};
