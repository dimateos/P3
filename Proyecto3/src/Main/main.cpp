#include <iostream>
#include <OgreInterface.h>

int main()
{
	OgreInterface* renderInterface = new OgreInterface();
	renderInterface->initApp();
	system("pause");
	delete renderInterface;
	return 0;
}
