#include "font.h"

Font::~Font()
{
	delete mat->diffuse;
	delete mat;
}
