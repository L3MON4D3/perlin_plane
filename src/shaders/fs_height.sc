$input v_position

#include <bgfx_shader.sh>

void main()
{
	float mod_res = mod(v_position.z, 4);
	if (mod_res > -.5 && mod_res < .5)
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	else
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
