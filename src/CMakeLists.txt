add_executable(worldWP
    main.cpp
    Util.cpp
    Plane.cpp
	Frame.cpp
	DiamondFrame.cpp
)

#add_library(perlin
#    Perlin.cpp
#)

#names of sourcefile/binary.
set(shaders lines; simple)
#Shadertypes.
set(type_long FRAGMENT; VERTEX)
#prefix of sourcefile.
set(type_short f; v)

#loop over shader-properties.
foreach(shader IN LISTS shaders)
	foreach(type IN ZIP_LISTS type_long type_short)
		set(filename_pre ${type_1}s_${shader})

		shaderc(FILE shaders/${filename_pre}.sc
		        OUTPUT ../build/shaders/${filename_pre}.bin
		        LABEL ${filename_pre}
		        ${type_0}
		        LINUX
		        VARYINGDEF shaders/varying_${shader}.def.sc
		        INCLUDES ../libs/bgfx.cmake/bgfx/src
		        PROFILE 120
		)
	endforeach()
endforeach()

add_custom_target(Shader ALL DEPENDS
	../build/shaders/fs_lines.bin
	../build/shaders/vs_lines.bin
	../build/shaders/fs_simple.bin
	../build/shaders/vs_simple.bin
)

target_link_libraries(worldWP PUBLIC fastNoise)
target_link_libraries(worldWP PUBLIC bgfx)
target_link_libraries(worldWP PUBLIC bx)
target_link_libraries(worldWP PUBLIC PkgConfig::GLFW3)
