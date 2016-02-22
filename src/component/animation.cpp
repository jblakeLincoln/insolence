#include "animation.h"

Animation::Animation()
{
	tex_width = tex_height = 1;
	rect = glm::vec4(0, 0, 1, 1);
}

Animation::~Animation()
{
}

Animation::Animation(Texture *tex, int in_frames, int in_cols,
		const glm::vec4& in_rect, int in_time)
{
	if(tex != NULL)
	{
		tex_width = tex->GetWidth();
		tex_height = tex->GetHeight();
	}

	this->cols = in_cols;
	this->frames = in_frames;
	this->offset = glm::vec2(in_rect.x, in_rect.y);
	this->size = glm::vec2(in_rect.z, in_rect.w);
	rect = in_rect;

	current_frame = 0;
	frame_time = in_time;
	timer = TimeSpan(0);
}

void Animation::Update(const TimeSpan& t)
{
	timer += t;

	if(timer.ElapsedMilliseconds() >= frame_time * frames)
	{
		timer.SetTime(Milliseconds((int)timer.ElapsedMilliseconds() %
					(int)(frame_time * frames)));
	}

	current_frame = (double)timer.ElapsedMilliseconds() / frame_time;

	float current_col = current_frame % cols;
	float current_row = (current_frame / cols);

	rect.x = offset.x + (size.x * (float)current_col);
	rect.y = offset.y + (size.y * (float)current_row);
	rect.z = size.x;
	rect.w = size.y;
}

glm::vec4 Animation::GetNormalisedRect()
{
	return glm::vec4(rect.x / tex_width, rect.y / tex_height,
			rect.z / tex_width, rect.w / tex_height);
}
