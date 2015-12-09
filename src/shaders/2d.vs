#version 150

uniform mat4 mat_proj;
uniform mat4 mat_view;

in vec3 in_verts;
in vec2 in_uvs;
in vec4 in_colour;

in vec4 in_modelx;
in vec4 in_modely;
in vec4 in_modelz;
in vec4 in_modelw;

out vec4 fs_colour;
out vec2 fs_uvs;

void main()
{
	mat4 model_matrix = mat4(in_modelx, in_modely, in_modelz, in_modelw);
	fs_colour = in_colour;
	fs_uvs = in_uvs;
	gl_Position = mat_proj * mat_view * model_matrix * vec4((in_verts), 1);
}
