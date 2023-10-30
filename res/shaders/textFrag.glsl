#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 tex = texture(text, TexCoords);
    vec4 sampled = vec4(1.0, 1.0, 1.0, tex.r);
    color = vec4(textColor, 1.0) * sampled;
    if (tex.xyz == vec3(0.0))
    {
        discard;
    }
}  