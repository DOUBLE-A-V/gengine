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
uniform float rotation;

void main()
{
    FragColor = vec4(vec3(texture(texture1, TexCoord)), 1.0);
	FragColor.a *= alpha;
	FragColor.r *= red;
	FragColor.g *= green;
	FragColor.b *= blue;
}