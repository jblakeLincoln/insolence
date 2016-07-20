IN vec4 fs_colour;

#ifdef INSOLENCE_GLSL_DESKTOP
	out vec4 out_colour;
	#define gl_FragColor out_colour
#endif

void main()
{
	gl_FragColor = vec4(fs_colour);
}
