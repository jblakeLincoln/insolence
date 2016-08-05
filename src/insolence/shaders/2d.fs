in vec4 fs_colour;
in vec2 fs_uvs;

uniform sampler2D tex;

void main()
{
	vec4 t = texture(tex, fs_uvs);

	if(t.a < 0.2)
		discard;

	gl_FragColor = t * fs_colour;
}
