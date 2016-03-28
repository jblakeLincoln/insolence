#version 140

in vec4 fs_colour;
in vec2 fs_uvs;

out vec4 out_colour;
uniform sampler2D tex;

void main()
{
	out_colour = texture(tex, fs_uvs) * fs_colour;
}
