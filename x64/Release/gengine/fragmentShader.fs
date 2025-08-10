#version 330 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform float alpha;
uniform float red;
uniform float green;
uniform float blue;

void main()
{
    FragColor = vec4(texture(texture1, TexCoord));
	FragColor.a *= alpha;
	FragColor.r *= red;
	FragColor.g *= green;
	FragColor.b *= blue;
}