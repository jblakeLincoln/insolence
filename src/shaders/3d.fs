#version 150

in vec3 frag_vert;
in vec3 frag_pos;
in vec3 frag_norm;
in vec2 frag_uv;
in vec4 frag_colour;

uniform sampler2D tex;
out vec4 out_colour;

struct DirLight {
	vec3 direction;

	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
};

uniform DirLight dir_light;
uniform vec3 view_pos;

void main() {

	vec3 frag_tex = texture(tex, frag_uv).xyz;

	/* Diffuse setup. */
	vec3 light_dir = normalize(-dir_light.direction);

	vec3 norm = normalize(frag_norm);

	float diff = max(dot(norm, light_dir), 0.0);

	/* Specular setup. */
	vec3 view_dir = normalize(view_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, frag_norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 1.0);

	vec3 diffuse = dir_light.diffuse * diff * frag_tex;
	vec3 ambient = dir_light.ambient * frag_tex;
	vec3 specular = dir_light.specular * spec;

	out_colour = vec4(ambient + diffuse + specular, 1.0) * frag_colour;
}
