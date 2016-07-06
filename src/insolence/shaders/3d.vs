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

IN vec3 in_vert;
IN vec3 in_norm;
IN vec2 in_uv;

IN vec4 in_modelx;
IN vec4 in_modely;
IN vec4 in_modelz;
IN vec4 in_modelw;
IN vec4 in_colour;

OUT vec3 frag_vert;
OUT vec3 frag_pos;
OUT vec3 frag_norm;
OUT vec2 frag_uv;
OUT vec4 frag_colour;

void main() {
	mat4 model = mat4(in_modelx, in_modely, in_modelz, in_modelw);

	frag_vert = in_vert;
	frag_norm = in_norm;
	frag_uv = in_uv;
	frag_colour = in_colour;

	frag_pos = vec3(model * vec4(in_vert, 1.0));
#ifdef INSOLENCE_GLSL_DESKTOP
	frag_norm = mat3(transpose(inverse(model))) * in_norm;
#endif
#ifdef INSOLENCE_GLSL_ES
	frag_norm = mat3(model) * in_norm;
#endif

	gl_Position = mat_proj * mat_view * model * vec4(in_vert, 1.0);
}


