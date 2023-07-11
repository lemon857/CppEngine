#version 460
layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 texture_coord;

out vec2 texCoord;

uniform mat4 modelMat;
uniform mat4 projectionMat;
uniform float layer;

void main() 
{
   texCoord = texture_coord;
   gl_Position = projectionMat * modelMat * vec4(vertex_position, layer, 1.0);   
}