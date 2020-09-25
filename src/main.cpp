#include "Util.hpp"
#include "PlaneBuilder.hpp"
#include "Frame.hpp"

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

/**
 * Create new GLFW-Window with dims width x height. GLFW needs to be initialized.
 * @param init Pass empty bgfx::Init.
 */
GLFWwindow* create_window(int width, int height) {
    //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    GLFWwindow *window =
        glfwCreateWindow(width, height, "worldWp", nullptr, nullptr);
	//glfwMakeContextCurrent(window);

    bgfx::Init init;

    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void*) glfwGetX11Window(window);
    //init.platformData.context = glfwGetCurrentContext();
    
    glfwGetWindowSize(window, &width, &height);
    init.resolution.height = height;
    init.resolution.width = width;
    init.resolution.reset = BGFX_RESET_MSAA_X2;

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
    fn.SetSeed(std::rand());
	worldWp::ModelSpecs specs {10, 10, 9};
    worldWp::PlaneBuilder plane(specs, fn, {2, 2, 40});

	worldWp::Frame frame {specs};
    //Call renderFrame before init (in create_window) to render on this thread.
    glfwInit();
    glfwSetErrorCallback(worldWp::util::glfw_errorCallback);

    //init should not go out of scope until program finishes.
    int width = 1000, height = 1000;
    renderFrame();
    GLFWwindow *window = create_window(width, height);

    worldWp::util::PosNormalColorVertex::init();

    const ViewId clearView = 0;
    setViewClear(clearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x282828ff, 1.0f, 0);

    VertexBufferHandle vbh = plane.getVBufferHandle();
    //VertexBufferHandle vbh = createVertexBuffer(
    //    makeRef(cubeVertices, sizeof(cubeVertices)),
    //    worldWp::PosNormalColorVertex::layout);

    IndexBufferHandle ibh = plane.getIBufferHandle();
    //IndexBufferHandle ibh = createIndexBuffer(
    //	makeRef(cubeTriList, sizeof(cubeTriList)));

	VertexBufferHandle frame_vbh{frame.getVBufferHandle()};
	IndexBufferHandle frame_ibh{frame.getIBufferHandle()};

    ShaderHandle vsh = worldWp::util::load_shader("build/src/vs_simple.bin");
    ShaderHandle fsh = worldWp::util::load_shader("build/src/fs_simple.bin");
    ProgramHandle program = createProgram(vsh, fsh, true);

    touch(clearView);

    float pos {-15.0f};

	int frame_ctr{-1}, ctr{0};

	int tran_length{800};
	float* offset_noise;
    while (!glfwWindowShouldClose(window)) {
		++frame_ctr;
		if (frame_ctr == tran_length)
			frame_ctr = 0;
		
        glfwPollEvents();
        int oldWidth = width, oldHeight = height;
        glfwGetWindowSize(window, &width, &height);
        if (width != oldWidth || height != oldHeight) {
            reset(width, height, BGFX_RESET_VSYNC);
            setViewRect(clearView, 0, 0, BackbufferRatio::Equal);
        }

		if (frame_ctr == 0) {
			fn.SetSeed(std::rand());
			float* new_noise = plane.get_raw_noise(fn);

			offset_noise = new float[specs.x_dim*specs.z_dim];
			plane.for_each_vertex(
				[new_noise, offset_noise, tran_length]
				(worldWp::util::PosNormalColorVertex& v, int i) {
					//offset_nose is difference between new and old noise.
					offset_noise[i] = (new_noise[i] - v.pos[1])*(1.0/tran_length);
			});
		}

		plane.for_each_vertex(
			[offset_noise](worldWp::util::PosNormalColorVertex& v, int i) {
				v.pos[1]+=offset_noise[i];
		});
		plane.add_normals();
		//IMPORTANT!!!
		bgfx::destroy(vbh);
		vbh = plane.getVBufferHandle();

        bx::Vec3 at  {0, 0, 0};
        bx::Vec3 eye {0, 70*2, 100*2};

        float view[16];
        bx::mtxLookAt(view, eye, at);

        float proj[16];
        bx::mtxProj(proj,
            60.0f,
            ((float)width)/height,
            0.1f,
            800.0f,
            bgfx::getCaps()->homogeneousDepth);

        bgfx::setViewTransform(clearView, view, proj);
        bgfx::setViewRect(clearView, 0, 0, width, height);

        touch(clearView);

        float mtx[16];
        bx::mtxRotateY(mtx, bx::kPiQuarter*(pos+=.001));

		//submit plane+base.
        bgfx::setTransform(mtx);
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);
        bgfx::submit(clearView, program);

		//submit Frame.
		bgfx::setTransform(mtx);
		bgfx::setVertexBuffer(0, frame_vbh);
        bgfx::setIndexBuffer(frame_ibh);
		bgfx::submit(clearView, program);

		bgfx::frame();
    }

	delete offset_noise;
    destroy(vbh);
    destroy(ibh);
    shutdown();
    glfwTerminate();
    return 0;
}
