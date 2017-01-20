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

struct PointLight {
	vec3 pos;

	float constant;
	float linear;
	float quadratic;
	
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
};

#define NUM_POINT_LIGHTS 32

uniform DirLight dir_light;
uniform PointLight point_lights[NUM_POINT_LIGHTS];
uniform vec3 view_pos;

vec3 do_dir_light(DirLight light, vec3 norm, vec3 view_dir)
{
	vec3 frag_tex = texture(tex_diffuse, frag_uv).xyz;
	vec3 light_dir = normalize(-light.direction);
	float diff = max(dot(norm, light_dir), 0.0);

	/* Specular setup. */
	vec3 reflect_dir = reflect(-light_dir, norm); 
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);

	vec3 diffuse = dir_light.diffuse * diff * frag_tex;
	vec3 ambient = dir_light.ambient * frag_tex;
	vec3 specular = dir_light.specular * spec;

	return diffuse + ambient + specular;
}

/*
vec3 do_point_light(PointLight light, vec3 norm, vec3 view_dir)
{
	vec3 frag_tex = texture(tex_diffuse, frag_uv).xyz;
	vec3 light_dir = normalize(light.pos - frag_pos);

	float diff = max(dot(norm, light_dir), 0.0);

	vec3 reflect_dir = reflect(-light_dir, norm);

	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);

	float distance = length(light.pos - frag_pos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
			light.quadratic * (distance * distance));

	vec3 diffuse = light.diffuse * diff * frag_tex;
	vec3 ambient = light.ambient * frag_tex;
	vec3 specular = light.specular * spec * frag_tex;

	diffuse *= attenuation;
	ambient *= attenuation;
	specular *= attenuation;

	return (diffuse + ambient + specular);
}
*/

vec3 do_point_light(PointLight light, vec3 norm, vec3 view_dir)
{
	if(light.constant == 0.0)
		return vec3(0);

	vec3 frag_tex = texture(tex_diffuse, frag_uv).xyz;

	vec3 light_dir = normalize(light.pos - frag_pos);
	vec3 halfway_dir = normalize(light_dir + view_dir);

	float diff = max(dot(norm, light_dir), 0.0);
	float spec = pow(max(dot(norm, halfway_dir), 0.0), 1.0);

	float distance = length(light.pos - frag_pos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance);

	vec3 diffuse = light.diffuse * diff * frag_tex;
	vec3 ambient = light.ambient * frag_tex;
	vec3 specular = light.specular * spec * frag_tex;

	diffuse *= attenuation;
	ambient *= attenuation;
	specular *= attenuation;

	return (diffuse + ambient + specular);
}

void main() {


	/* Diffuse setup. */

	vec3 norm;
	vec3 view_dir = normalize(view_pos - frag_pos);

	if(normal_map == false)
		norm = normalize(frag_norm);
	else
	{
		norm = texture(tex_normal, frag_uv).xyz;
		norm = normalize(norm * 2.0 - 1.0);
	}

	//result += do_point_light(point_lights[1], norm, view_dir);
	vec3 result = do_dir_light(dir_light, norm, view_dir);

	for(int i = 0; i < NUM_POINT_LIGHTS; ++i)
		result += do_point_light(point_lights[i], norm, view_dir);

	gl_FragColor = vec4(result, 1.0) * frag_colour;
}
