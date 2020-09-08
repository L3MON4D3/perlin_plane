#include "Util.hpp"
#include "ModelBuilder.hpp"

#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "bgfx/platform.h"
#include "bx/math.h"

#include <cstdlib>
#include <iostream>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>


bgfx::VertexLayout worldWp::util::PosNormalColorVertex::layout;

worldWp::util::PosNormalColorVertex cubeVertices[] = {
    {-1.0f, -1.0f,  0.0f, 0, 0, 0, 0xff000000 },
    {-1.0f,  1.0f,  0.0f, 0, 0, 0, 0xff0000ff },
    { 1.0f,  1.0f,  0.0f, 0, 0, 0, 0xff00ff00 },
    { 1.0f, -1.0f,  0.0f, 0, 0, 0, 0xff00ffff },
};

const uint16_t cubeTriList[] = {
    0, 2, 1,
    0, 3, 2
};

/**
 * Create new GLFW-Window with dims width x height. GLFW needs to be initialized.
 * @param init Pass empty bgfx::Init.
 */
GLFWwindow* create_window(int width, int height) {
    GLFWwindow *window =
        glfwCreateWindow(width, height, "worldWp", nullptr, nullptr);

    bgfx::Init init;

    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void*) glfwGetX11Window(window);
    
    glfwGetWindowSize(window, &width, &height);
    init.resolution.height = height;
    init.resolution.width = width;
    init.resolution.reset = BGFX_RESET_VSYNC;

    bgfx::init(init);

    return window;
}

std::ostream& operator<<(std::ostream& out, const bx::Vec3& v) {
	return out << "{" << v.x << ", " << v.y << ", " << v.z << "}";
}

int main(int argc, char** argv) {
    //using so lines dont get too long.
    using namespace bgfx;

    FastNoise fn;
    fn.SetNoiseType(FastNoise::Perlin);
    fn.SetSeed(2454244);
    worldWp::ModelBuilder builder( {10, 10, 9}, fn, {2, 2, 80} );
    //Call renderFrame before init (in create_window) to render on this thread.
    glfwInit();
    glfwSetErrorCallback(worldWp::util::glfw_errorCallback);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    //init should not go out of scope until program finishes.
    int width = 1000, height = 1000;
    renderFrame();
    GLFWwindow *window = create_window(width, height);

    worldWp::util::PosNormalColorVertex::init();

    const ViewId clearView = 0;
    setViewClear(clearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x111111ff, 1.0f, 0);
    setViewRect(clearView, 0, 0, BackbufferRatio::Equal);

    VertexBufferHandle vbh = builder.getVBufferHandle();
    //VertexBufferHandle vbh = createVertexBuffer(
    //    makeRef(cubeVertices, sizeof(cubeVertices)),
    //    worldWp::PosNormalColorVertex::layout);

    IndexBufferHandle ibh = builder.getIBufferHandle();
    //IndexBufferHandle ibh = createIndexBuffer(
    //	makeRef(cubeTriList, sizeof(cubeTriList)));

    ShaderHandle vsh = worldWp::util::load_shader("build/src/vs_simple.bin");
    ShaderHandle fsh = worldWp::util::load_shader("build/src/fs_simple.bin");
    ProgramHandle program = createProgram(vsh, fsh, true);

    touch(clearView);

    float pos {-15.0f};


	int frame_ctr{-1};
    while (!glfwWindowShouldClose(window)) {
		++frame_ctr;
		if (frame_ctr == 1000)
			frame_ctr = 0;
		
        glfwPollEvents();
        int oldWidth = width, oldHeight = height;
        glfwGetWindowSize(window, &width, &height);
        if (width != oldWidth || height != oldHeight) {
            reset(width, height, BGFX_RESET_VSYNC);
            setViewRect(clearView, 0, 0, BackbufferRatio::Equal);
        }

		float* offset_noise;
		if (frame_ctr == 0) {
			fn.SetSeed(std::rand());
			float* new_noise = builder.get_raw_noise(fn);

			offset_noise = new float[10*10];
			builder.for_each_vertex(
				[new_noise, offset_noise](worldWp::util::PosNormalColorVertex& v, int i) {
					//offset_nose is difference between new and old noise.
					offset_noise[i] = (new_noise[i] - v.pos[2])*.001;
			});
		}

		builder.for_each_vertex(
			[offset_noise](worldWp::util::PosNormalColorVertex& v, int i) {
				v.pos[2]+=offset_noise[i];
		});
		builder.add_normals();
		vbh = builder.getVBufferHandle();

        bx::Vec3 at  {0, 0,    0};
        bx::Vec3 eye {0, 0, -100};

        float view[16];
        bx::mtxLookAt(view, eye, at);

        float proj[16];
        bx::mtxProj(proj,
            90.0,
            ((float)width)/height,
            0.1f,
            800.0f,
            bgfx::getCaps()->homogeneousDepth);

        bgfx::setViewTransform(clearView, view, proj);
        bgfx::setViewRect(clearView, 0, 0, width, height);

        touch(clearView);

        float mtx[16];
        bx::mtxRotateY(mtx, 0.0f);
		pos += 0.1;
        mtx[12] = -20;
        mtx[13] = -20;
        mtx[14] = -10;

        bgfx::setTransform(mtx);

        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);
        bgfx::setState(BGFX_STATE_DEFAULT);

        bgfx::submit(clearView, program);

        frame();
    }

    destroy(vbh);
    destroy(ibh);
    shutdown();
    glfwTerminate();
    return 0;
}
