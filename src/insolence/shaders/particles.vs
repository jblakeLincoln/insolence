in vec2 in_verts;
in vec2 in_uvs;
in vec2 in_pos;
in vec4 in_colour;
in float in_scale;

out vec4 fs_colour;

void main()
{
	fs_colour = in_colour;
	gl_Position = mat_proj * mat_view * vec4(in_verts.xy * in_scale + in_pos.xy, 0.0, 1.0);
}
