$input v_color0, v_normal

#include <bgfx_shader.sh>

void main()
{
    vec3 lightDir = vec3(0.0, 0.0, -1.0);
    float ndotl = dot(v_normal, lightDir);
    float spec = 0.0;
    gl_FragColor = vec4(pow(pow(v_color0.xyz, vec3_splat(2.2) ) * ndotl + spec, vec3_splat(1.0/2.2) ), 1.0);
}
