
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

IN vec3 in_verts;
IN vec2 in_uvs;
IN vec4 in_colour;

IN vec4 in_modelx;
IN vec4 in_modely;
IN vec4 in_modelz;
IN vec4 in_modelw;

IN vec4 in_rect;

OUT vec4 fs_colour;
OUT vec2 fs_uvs;

void main()
{
	mat4 model_matrix = mat4(in_modelx, in_modely, in_modelz, in_modelw);
	fs_colour = in_colour;
	fs_uvs = in_uvs * in_rect.zw + in_rect.xy;

	gl_Position = mat_proj * mat_view * model_matrix * vec4((in_verts), 1);
}
