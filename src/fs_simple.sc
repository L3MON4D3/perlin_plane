$input v_color0, v_normal

#include <bgfx_shader.sh>

void main()
{
    vec3 lightDir = vec3(0.0, 0.0, -1.0);
    float ndotl = dot(v_normal, lightDir);
    float spec = 0.0;
    gl_FragColor = v_color0 + ndotl + spec;
}
