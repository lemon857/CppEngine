#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 texture_coord;

out vec3 color;
out vec2 texCoord;

uniform mat4 modelMat;
uniform mat4 projectionMat;

void main() 
{
   color = vertex_color;
   texCoord = texture_coord;
   gl_Position = projectionMat * modelMat * vec4(vertex_position, 1.0);   
}