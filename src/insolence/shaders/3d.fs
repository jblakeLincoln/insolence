in vec3 frag_vert;
in vec3 frag_pos;
in vec3 frag_norm;
in vec2 frag_uv;
in vec4 frag_colour;

uniform sampler2D tex_diffuse;
uniform sampler2D tex_normal;
uniform bool normal_map;

struct DirLight {
	vec3 direction;

	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
};

uniform DirLight dir_light;
uniform vec3 view_pos;

void main() {

	vec3 frag_tex = texture(tex_diffuse, frag_uv).xyz;

	/* Diffuse setup. */
	vec3 light_dir = normalize(-dir_light.direction);

	vec3 norm;

	if(normal_map == false)
		norm = normalize(frag_norm);
	else
	{
		norm = texture(tex_normal, frag_uv).xyz;
		norm = normalize(norm * 2.0 - 1.0);
	}

	float diff = max(dot(norm, light_dir), 0.0);

	/* Specular setup. */
	vec3 view_dir = normalize(view_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, frag_norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 1.0);

	vec3 diffuse = dir_light.diffuse * diff * frag_tex;
	vec3 ambient = dir_light.ambient * frag_tex;
	vec3 specular = dir_light.specular * spec;

	gl_FragColor = vec4(ambient + diffuse + specular, 1.0) * frag_colour;
}
