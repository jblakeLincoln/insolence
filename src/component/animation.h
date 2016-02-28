#ifndef COMPONENT_ANIMATION_H
#define COMPONENT_ANIMATION_H

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "../component/component.h"
#include "../game/timespan.h"

struct Animation : public Component {
	int current_frame;
	int cols;
	int frames;
	float frame_time;
	glm::vec2 offset;
	glm::vec2 size;
	glm::vec4 rect;
	TimeSpan timer;

public:
	Animation();
	~Animation();

	/**
	 * \param frames	Number of frames in the animation.
	 * \param cols		Number of columns in the animation.
	 * \param in_rect	X and Y to start on texture, Z and W for size.
	 * \param time		Time length of each frame in milliseconds.
	 */
	Animation(int frames, int cols,
		const glm::vec4& in_rect, int time);
};

#endif
