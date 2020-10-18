$input v_position

#include <bgfx_shader.sh>

uniform float b0 = 0.3;
uniform float b1 = 0.6;

void main()
{
	float mod_res = mod(v_position.z, 4);
	if (mod_res <= b0) {
		float clr = (b0-mod_res)/b0;
		//gl_FragColor = vec4(clr, clr, clr, 1.0);
		gl_FragColor = vec4(clr, clr, clr, 1.0);
	} else if(mod_res <= b1) {
		float clr = (mod_res-(b1-b0))/b0;
		//gl_FragColor = vec4(clr, clr, clr, 1.0);
		gl_FragColor = vec4(clr, clr, clr, 1.0);
	} else {
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
}
