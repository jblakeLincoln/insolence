#version 150

uniform mat4 mat_proj;
uniform mat4 mat_view;

in vec3 in_vert;
in vec3 in_norm;
in vec2 in_uv;

in vec4 in_modelx;
in vec4 in_modely;
in vec4 in_modelz;
in vec4 in_modelw;

out vec3 frag_vert;
out vec3 frag_norm;
out vec2 frag_uv;

void main() {
	mat4 model = mat4(in_modelx, in_modely, in_modelz, in_modelw);
	gl_Position = mat_proj * mat_view * model * vec4(in_vert, 1.0);

	frag_vert = in_vert;
	frag_norm = in_norm;
	frag_uv = in_uv;
	frag_norm = mat3(transpose(inverse(mat3(model))))*in_norm;
}


