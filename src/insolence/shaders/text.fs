IN vec4 fs_colour;
IN vec2 fs_uvs;

uniform sampler2D tex;

#ifdef INSOLENCE_GLSL_DESKTOP
	out vec4 out_colour;
	#define gl_FragColor out_colour
#endif

void main()
{
	vec4 t = texture2D(tex, fs_uvs);

	//if(t.a < 0.5)
	//	discard;

	gl_FragColor = vec4(1, 1, 1, t.r) * fs_colour;
}
