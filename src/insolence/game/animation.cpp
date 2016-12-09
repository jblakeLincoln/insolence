#include "animation.h"

#include "../game/texture.h"

Animation::Animation()
:
	rect(glm::vec4(0, 0, 1, 1))
{
}

void Animation::Setup(Texture *t)
{
	if(t == NULL)
		return;

	size = glm::vec2(t->GetWidth(), t->GetHeight());
}

Animation::~Animation()
{}

Animation::Animation(int in_frames, int in_cols,
		const glm::vec4& in_rect, int in_time)
	: frames(in_frames),
	cols(in_cols),
	offset(glm::vec2(in_rect.x, in_rect.y)),
	size(glm::vec2(in_rect.z, in_rect.w)),
	rect(in_rect),
	frame_time(in_time),
	timer(0)
{
}

void Animation::SetFrame(int frame)
{
	if(cols == 0)
		cols = 1;

	float current_col = frame % cols;
	float current_row = frame / cols;

	rect.x = offset.x + (size.x * (float)current_col);
	rect.y = offset.y + (size.y * (float)current_row);
	rect.z = size.x;
	rect.w = size.y;
}

void Animation::Progress(const TimeSpan& frametime)
{
	if(frames == 0 || frame_time == 0)
	{
		SetFrame(0);
		return;
	}

	timer += frametime;

	if(timer.ElapsedMilliseconds() >= frame_time * frames)
	{
		timer.SetTime(Milliseconds((int)timer.ElapsedMilliseconds() %
					(int)(frame_time * frames)));
	}

	SetFrame((double)timer.ElapsedMilliseconds() / frame_time);
}

glm::vec4 Animation::GetAnimationRectangle(const Texture *t) const
{
	return glm::vec4(rect.x / t->width, rect.y / t->height,
			rect.z / t->width, rect.w / t->height);
}
