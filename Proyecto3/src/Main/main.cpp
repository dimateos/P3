#include <iostream>
#include <OgreInterface.h>

int main(int argc, char *argv[])
{
	OgreInterface* renderInterface = new OgreInterface();
	renderInterface->initApp();
	delete renderInterface;
	return 0;
}
