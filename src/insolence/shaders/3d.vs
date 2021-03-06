in vec3 in_vert;
in vec3 in_norm;
in vec2 in_uv;

in vec4 in_modelx;
in vec4 in_modely;
in vec4 in_modelz;
in vec4 in_modelw;
in vec4 in_colour;

out vec3 frag_vert;
out vec3 frag_pos;
out vec3 frag_norm;
out vec2 frag_uv;
out vec4 frag_colour;

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
#ifdef INSOLENCE_GLSL_WEB
	frag_norm = mat3(model) * in_norm;
#endif

	gl_Position = mat_proj * mat_view * model * vec4(in_vert, 1.0);
}


