#include "animation.h"

Animation::Animation()
{
	}

Animation::~Animation()
{

}

Animation::Animation(Texture *tex, int frames, int cols, int time)
{
	if(tex != NULL)
	{
		tex_width = tex->GetWidth();
		tex_height = tex->GetHeight();
	}

	this->cols = cols;
	this->frames = frames;
	frame_time = time;

	timer = TimeSpan(0);
	offset = glm::vec2(32, 58);
	size = glm::vec2(176, 180);
}

void Animation::Update(const TimeSpan& t)
{
	timer += t;

	if(timer.ElapsedMilliseconds() >= frame_time * frames)
	{
		timer.SetTime(Milliseconds((int)timer.ElapsedMilliseconds() %
					(int)(frame_time * frames)));
	}

	current_frame = timer.ElapsedMilliseconds() / (int)(frame_time);

	current_col = current_frame % cols;
	current_row = (int)(current_frame / cols);

	rect.x = offset.x + (size.x * (float)current_col);
	rect.y = offset.y + (size.y * (float)current_row);
	rect.z = size.x;
	rect.w = size.y;
}

glm::vec4 Animation::GetNormalisedRect()
{
	return glm::vec4(rect.x / tex_width, rect.y / tex_height,
			rect.z / tex_width, rect.w / tex_height);
	//return glm::vec4(0);//rect / glm::vec2(tex_width, tex_height);
}
