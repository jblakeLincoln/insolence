#version 150

in vec3 frag_vert;
in vec3 frag_norm;
in vec2 frag_uv;
uniform sampler2D tex;
out vec4 out_colour;

void main() {
	out_colour = texture(tex, frag_uv);
}
