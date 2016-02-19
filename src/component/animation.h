#ifndef ANIMATION_H
#define ANIMATION_H

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "../component/texture.h"
#include "../game/timespan.h"
#include <cmath>

struct Animation {
private:
	float tex_width;
	float tex_height;
	int current_row;
	int current_col;
	int current_frame;
	glm::vec2 size;
	glm::vec4 rect;

	TimeSpan timer;
public:
	Animation();
	~Animation();
	Animation(Texture*, int rows, int cols, int time);

	void Update(const TimeSpan& t);
	glm::vec4 GetNormalisedRect();
	glm::vec2 offset;
	int cols;
	int frames;
	float frame_time;
};

#endif
