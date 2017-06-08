#version 330

uniform mat4 MVP;

in vec3 vCol;
in vec3 vPos;
in vec2 tc;

out vec2 texCoord;
out vec3 color;

void main()
{
    gl_Position = MVP * vec4(vPos, 1.0);
    color = vCol;
    texCoord = tc;
}
