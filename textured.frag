#version 330

in vec3 color;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
    fragColor = texture(tex, texCoord) * vec4(color, 1.0);
    //fragColor = vec4(texCoord, 0.0, 1.0);
}
