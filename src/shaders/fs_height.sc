$input v_position

#include <bgfx_shader.sh>

void main()
{
	gl_FragColor = vec4(1, v_position.z, 0, 0);
}
