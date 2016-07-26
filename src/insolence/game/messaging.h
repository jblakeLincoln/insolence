#ifndef MESSAGING_H
#define MESSAGING_H

#include <vector>
#include <unordered_map>

struct Entity;
struct EntityManager;
struct MessageManager
{
private:
	std::unordered_map<uint32_t, std::vector<uint32_t>>::iterator it;
	// Message ID, Entity ID
	std::unordered_map<uint32_t, std::vector<uint32_t>> messages;
	EntityManager *manager;

public:
	MessageManager(EntityManager *mgr);

	void Add(uint32_t event, Entity *entity);
	Entity* Get(uint32_t event);
	void Clear();
};

#endif
