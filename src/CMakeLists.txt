add_executable(worldWP
    main.cpp
)

add_library(perlin
    Perlin.cpp
)

target_link_libraries(worldWP PUBLIC perlin)
target_link_libraries(worldWP PUBLIC bgfx)
target_link_libraries(worldWP PUBLIC PkgConfig::GLFW3)
