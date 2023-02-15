#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoords;
out vec3 position;
out vec2 texxy;
uniform mat4 transform;
out vec3 norm;
uniform mat4 model;
uniform mat4 view;
void main(){
vec4 pos = view * transform * model * vec4(pos.x, pos.y, pos.z, 1.0f);
//gl_Position = vec4(pos.xy, pos.z, 5 * pos.z);
gl_Position = vec4(pos.xyz, pos.z);
position = (transform * model * vec4(pos.x, pos.y, pos.z, 1.0f)).xyz;
norm = normal;
texxy = texcoords;
}