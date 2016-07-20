
#ifdef INSOLENCE_GLSL_DESKTOP
layout(std140) uniform Camera {
	mat4 mat_proj;
	mat4 mat_view;
};
#endif
#ifdef INSOLENCE_GLSL_ES
uniform mat4 mat_proj;
uniform mat4 mat_view;
#endif

IN vec2 in_verts;
IN vec2 in_pos;
IN vec4 in_colour;
IN float in_scale;

OUT vec4 fs_colour;

void main()
{
	fs_colour = in_colour;
	gl_Position = mat_proj * mat_view * vec4(in_verts.xy * in_scale + in_pos.xy, 0.0, 1.0);
}
