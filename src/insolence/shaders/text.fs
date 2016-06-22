#version 140

in vec4 fs_colour;
in vec2 fs_uvs;

out vec4 out_colour;
uniform sampler2D tex;

void main()
{
	vec4 t = texture(tex, fs_uvs);

	if(t.a < 0.5)
		discard;

	out_colour = vec4(1, 1, 1, t.r) * fs_colour;
}
