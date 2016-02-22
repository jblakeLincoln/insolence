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
	 * \param tex		Pointer to animation texture.
	 * \param frames	Number of frames in the animation.
	 * \param cols		Number of columns in the animation.
	 * \param in_rect	X and Y to start on texture, Z and W for size.
	 * \param time		Time length of each frame in milliseconds.
	 */
	Animation(Texture *tex, int frames, int cols,
		const glm::vec4& in_rect, int time);

	/**
	 * Update the animation timer and continue the animation if necessary.
	 *
	 * \param t	Timespan to iterate by.
	 */
	void Update(const TimeSpan& t);
	glm::vec4 GetNormalisedRect();
};

#endif
