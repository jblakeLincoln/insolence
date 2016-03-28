#ifndef SYSTEM_ANIMATION_H
#define SYSTEM_ANIMATION_H

static void ProgressAnimation(Entity* e, const TimeSpan& frame_time)
{
	Animation *a = e->Get<Animation>();

	if(a == NULL)
		return;

	a->timer += frame_time;

	if(a->timer.ElapsedMilliseconds() >= a->frame_time * a->frames)
	{
		a->timer.SetTime(Milliseconds((int)a->timer.ElapsedMilliseconds() %
					(int)(a->frame_time * a->frames)));
	}

	a->current_frame = (double)a->timer.ElapsedMilliseconds() / a->frame_time;

	float current_col = a->current_frame % a->cols;
	float current_row = a->current_frame / a->cols;

	a->rect.x = a->offset.x + (a->size.x * (float)current_col);
	a->rect.y = a->offset.y + (a->size.y * (float)current_row);
	a->rect.z = a->size.x;
	a->rect.w = a->size.y;
}

static glm::vec4 GetAnimationRectangle(Entity *e, Texture *t)
{
	Animation *a = e->Get<Animation>();
	//Texture *t = e->Get<Texture>();

	if(a == NULL || t == NULL)
		return glm::vec4();


	return glm::vec4(a->rect.x / t->width, a->rect.y / t->height,
			a->rect.z / t->width, a->rect.w / t->height);
}

#endif
