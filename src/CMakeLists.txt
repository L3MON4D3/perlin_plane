add_executable(worldWP
    main.cpp
)

target_link_libraries(worldWP PUBLIC PkgConfig::CAIROMM)
