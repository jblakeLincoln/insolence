#include "animation.h"

Animation::Animation()
{
	rect = glm::vec4(0, 0, 1, 1);
}

Animation::~Animation()
{
}

Animation::Animation(int in_frames, int in_cols,
		const glm::vec4& in_rect, int in_time)
	: frames(in_frames),
	cols(in_cols),
	offset(glm::vec2(in_rect.x, in_rect.y)),
	size(glm::vec2(in_rect.z, in_rect.w)),
	frame_time(in_time),
	timer(0)
{}
