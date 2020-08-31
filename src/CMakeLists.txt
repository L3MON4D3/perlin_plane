add_executable(worldWP
    main.cpp
    Util.cpp
    ModelBuilder.cpp
)

add_library(perlin
    Perlin.cpp
)

target_link_libraries(worldWP PUBLIC fastNoise)
target_link_libraries(worldWP PUBLIC bgfx)
target_link_libraries(worldWP PUBLIC bx)
target_link_libraries(worldWP PUBLIC PkgConfig::GLFW3)
