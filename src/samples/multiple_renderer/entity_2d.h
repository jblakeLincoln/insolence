struct TestEntity2D
{
	Texture *tex;
	RenderManager2D *renderer;
	Animation animation;
	MovementComponent move;

	TestEntity2D() {}

	TestEntity2D(Texture *t, const glm::vec3 &pos, RenderManager2D *r)
	{
		tex = t;
		renderer = r;
		move.Move(pos);
		move.Scale(glm::vec3(2.f));
		animation = Animation(t, 3, 3, glm::vec4(30, 64, 180, 180), 100);
	}

	virtual void Update(const TimeSpan& t)
	{
		animation.Update(t);
	}

	virtual void Draw()
	{
		renderer->Add(tex, move.GetModelMatrix(), glm::vec4(1.f),
				animation.GetNormalisedRect());
	}
};
