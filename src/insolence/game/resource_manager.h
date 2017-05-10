#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <stdint.h>
#include <unordered_map>

/*
 * TODO: Add callbacks to create requested resources on attempted Get.
 */
template<typename T, typename Lookup = uint32_t>
struct ResourceManager
{
	std::unordered_map<Lookup, T*> resources;

	ResourceManager() {}

	bool Add(const Lookup &key, T *val) {
		if(val == nullptr)
			return false;

		resources[key] = val;
		return true;
	}

	bool Has(const Lookup &key) {
		return resources.count(key) != 0;
	}

	T* Get(const Lookup &key) {
		if(resources.count(key) == 0)
			return nullptr;

		return resources[key];
	}

	T* Remove(const Lookup &key) {
		if(resources.count(key) == 0)
			return nullptr;

		T *ret = resources[key];
		resources.erase(key);
		return ret;
	}

	~ResourceManager() {
		for(auto it = resources.begin(); it != resources.end(); ++it)
			delete it->second;
	}
};

#endif
