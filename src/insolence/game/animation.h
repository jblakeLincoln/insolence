#ifndef COMPONENT_ANIMATION_H
#define COMPONENT_ANIMATION_H

#include "../insolence_dll.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "../game/timespan.h"

struct Texture;
struct INSOLENCE_API Animation {
private:
	uint32_t current_frame;
	glm::vec4 rect;
	TimeSpan timer;

public:
	glm::vec2 offset;
	glm::vec2 size;
	uint32_t cols = 1;
	uint32_t frames = 0;
	float frame_time = 0;

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

	void Setup(Texture *t);
	void SetFrame(int frame);

	void Progress(const TimeSpan& frametime);

	/**
	 * Returns a normalised rectangle based on the animation and the texture
	 * height.
	 */
	glm::vec4 GetAnimationRectangle(const Texture *t) const;
};

#endif
