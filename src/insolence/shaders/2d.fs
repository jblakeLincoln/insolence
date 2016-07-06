IN vec4 fs_colour;
IN vec2 fs_uvs;

uniform sampler2D tex;

#ifdef INSOLENCE_GLSL_DESKTOP
	out vec4 out_colour;
	#define gl_FragCoord out_colour;
#endif

void main()
{
	vec4 t = texture(tex, fs_uvs);

	if(t.a < 0.2)
		discard;

	gl_FragColor = t * fs_colour;
}
