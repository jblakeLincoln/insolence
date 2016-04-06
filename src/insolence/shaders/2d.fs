#version 140

in vec4 fs_colour;
in vec2 fs_uvs;

out vec4 out_colour;
uniform sampler2D tex;

void main()
{
	vec4 t = texture(tex, fs_uvs);

	if(t.a < 0.2)
		discard;

	out_colour = t * fs_colour;
}
