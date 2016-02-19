#include "samples/multiple_renderer/sample.h"

int main()
{
	SampleMultipleRenderer *world = new SampleMultipleRenderer();
	world->Run();

	delete world;
	return 0;
}
