$input v_position

#include <bgfx_shader.sh>

uniform float b0 = .1;
uniform float b1 = .2;
uniform float b2 = .3;
uniform float b3 = .4;
uniform float b4 = .5;

void main()
{
	float mod_res = mod(v_position.z, 4);
	if (mod_res < b0)
		gl_FragColor = vec4(0.66, 0.66, 0.66, 1.0);
	else if (mod_res < b1)
		gl_FragColor = vec4(0.33, 0.33, 0.33, 1.0);
	else if (mod_res < b2)
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	else if (mod_res < b3)
		gl_FragColor = vec4(0.33, 0.33, 0.33, 1.0);
	else if (mod_res < b4)
		gl_FragColor = vec4(0.66, 0.66, 0.66, 1.0);
	else
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
