#include "Perlin.hpp"
#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "bgfx/platform.h"
#include <iostream>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

static void glfw_errorCallback(int error, const char *description)
{
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

/**
 * Create new GLFW-Window with dims. GLFW needs to be initialized.
 * @param init Pass empty bgfx::Init.
 */
GLFWwindow* create_window(int width, int height, bgfx::Init &init) {
    GLFWwindow *window =
        glfwCreateWindow(width, height, "helloworld", nullptr, nullptr);

    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void*) glfwGetX11Window(window);
    
    glfwGetWindowSize(window, &width, &height);
    init.resolution.height = height;
    init.resolution.width = width;
    init.resolution.reset = BGFX_RESET_VSYNC;

    return window;
}

int main(int argc, char** argv){
    RandomGenerator::Perlin pln = RandomGenerator::Perlin(20);
    //Call renderFrame before bgfx::init (in create_window) to render on this thread.
    bgfx::renderFrame();
    glfwSetErrorCallback(glfw_errorCallback);
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    //init should not go out of scope until program finishes.
    bgfx::Init init;
    int width = 100, height = 100;
    GLFWwindow *window = create_window(width, height, init);
    bgfx::init(init);
    if (!window) {
        std::cout << "lol";
        return 1;
    }
    

    const bgfx::ViewId clearView = 0;
    bgfx::setViewClear(clearView, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(clearView, 0, 0, bgfx::BackbufferRatio::Equal);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        int oldWidth = width, oldHeight = height;
        glfwGetWindowSize(window, &width, &height);
        if (width != oldWidth || height != oldHeight) {
            bgfx::reset(width, height, BGFX_RESET_VSYNC);
            bgfx::setViewRect(clearView, 0, 0, bgfx::BackbufferRatio::Equal);
        }
        
        bgfx::touch(clearView);
        bgfx::frame();
    }

    bgfx::shutdown();
    glfwTerminate();
    return 0;
}
