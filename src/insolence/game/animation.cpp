#include "animation.h"

#include "../game/texture.h"

Animation::Animation()
:
	frames(0),
	cols(0),
	offset(glm::vec2(0)),
	rect(glm::vec4(0, 0, 1, 1)),
	frame_time(0)
{
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

void Animation::Progress(const TimeSpan& frametime)
{
	if(frames == 0)
		return;

	timer += frametime;

	if(timer.ElapsedMilliseconds() >= frame_time * frames)
	{
		timer.SetTime(Milliseconds((int)timer.ElapsedMilliseconds() %
					(int)(frame_time * frames)));
	}

	current_frame = (double)timer.ElapsedMilliseconds() / frame_time;

	float current_col = current_frame % cols;
	float current_row = current_frame / cols;

	rect.x = offset.x + (size.x * (float)current_col);
	rect.y = offset.y + (size.y * (float)current_row);
	rect.z = size.x;
	rect.w = size.y;
}

glm::vec4 Animation::GetAnimationRectangle(const Texture *t) const
{
	return glm::vec4(rect.x / t->width, rect.y / t->height,
			rect.z / t->width, rect.w / t->height);
}
