#ifndef MATERIAL_H
#define MATERIAL_H

#include "../insolence_dll.h"

#include "texture.h"

struct INSOLENCE_API Material {
private:
	static Material *mat_default;

public:

	static Material* GetDefault() {
		if(mat_default == 0)
			mat_default = new Material();

		return mat_default;
	}

	Texture *diffuse;
	Texture *normal;

	Material(Texture *diffuse=Texture::GetDefault(),
			Texture *normal=0)
	:	diffuse(diffuse),
		normal(normal)
	{}
};

#endif
