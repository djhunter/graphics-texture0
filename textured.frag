#version 330

in vec3 color;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D tex0;

void main()
{
    fragColor = texture(tex0, texCoord) * vec4(color, 1.0);
}
