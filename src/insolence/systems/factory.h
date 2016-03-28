#ifndef SYSTEM_FACTORY_H
#define SYSTEM_FACTORY_H


struct EntityFactory {
private:
	std::vector<Entity*> entities;
	std::vector<RenderManager*> renderers;

public:
	void Update(const GameTime& gametime)
	{
		for(int i = 0; i < entities.size(); ++i)
		{
			Entity *e = entities[i];

			if(e->Has<Animation>())
				ProgressAnimation(e, gametime.GetFrameTime());
			else if(e->Has<Mesh>())
				SyncMovementToRigidBody(e);
		}
	}

	void Draw()
	{

	}
};

#endif
