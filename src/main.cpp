#include "samples/multiple_renderer/sample.h"
#include "samples/text/sample.h"

int main()
{
	//SampleMultipleRenderer *world = new SampleMultipleRenderer();
	SampleText *world = new SampleText();
	world->Run();

	delete world;
	return 0;
}
