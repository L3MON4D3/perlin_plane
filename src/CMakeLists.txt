add_executable(worldWP
    main.cpp
)

add_library(Perlin
    Perlin.cpp
)

target_link_libraries(worldWP PUBLIC Perlin)
target_link_libraries(worldWP PUBLIC PkgConfig::CAIROMM)
