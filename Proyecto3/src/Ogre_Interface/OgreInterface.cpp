#include "OgreInterface.h"
#include <iostream>
#include <SDL_video.h>
#include <SDL_syswm.h>

OgreInterface* OgreInterface::instance_ = nullptr;

void OgreInterface::createRoot()
{
	#if _DEBUG
		mRoot = new Ogre::Root("plugins_d.cfg");
	#else
		mRoot = new Ogre::Root("plugins.cfg");
	#endif
}

void OgreInterface::setupResources()
{
	Ogre::ConfigFile cf;
	#if _DEBUG
		cf.load("resources_d.cfg");
	#else
		cf.load("resources.cfg");
	#endif

	Ogre::String sec, type, arch;
	Ogre::ConfigFile::SettingsBySection_::const_iterator seci;

	//secciones
	for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci)
	{
		sec = seci->first;
		const Ogre::ConfigFile::SettingsMultiMap& settings = seci->second;
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

		//elementos de cada seccion
		for (i = settings.begin(); i != settings.end(); ++i)
		{
			type = i->first;
			arch = FileSystemLayer::resolveBundlePath(i->second);
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
		}
	}

	sec = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

	const Ogre::ResourceGroupManager::LocationList genLocs = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(sec);
}

bool OgreInterface::setConfiguration()
{
	RenderSystemList l = mRoot->getAvailableRenderers();
	rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");

	mRoot->setRenderSystem(rs);
	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");

	return true;
}

void OgreInterface::createWindow()
{
	uint32_t w, h;
	Ogre::NameValuePairList miscParams;

	Ogre::ConfigOptionMap ropts = mRoot->getRenderSystem()->getConfigOptions();

	std::istringstream mode(ropts["Video Mode"].currentValue);
	Ogre::String token;
	mode >> w; // width
	mode >> token; // 'x' as seperator between width and height
	mode >> h; // height

	miscParams["FSAA"] = ropts["FSAA"].currentValue;
	miscParams["vsync"] = ropts["VSync"].currentValue;
	miscParams["gamma"] = ropts["sRGB Gamma Conversion"].currentValue;

	if (!SDL_WasInit(SDL_INIT_VIDEO)) SDL_InitSubSystem(SDL_INIT_VIDEO);


	Uint32 flags = SDL_WINDOW_RESIZABLE;

	if (ropts["Full Screen"].currentValue == "Yes")  flags = SDL_WINDOW_FULLSCREEN;

	SDL_win = SDL_CreateWindow(projectName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(SDL_win, &wmInfo);

	miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));

	mWindow = mRoot->createRenderWindow(projectName, w, h, false, &miscParams);
}

void OgreInterface::initializeResources()
{
	
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void OgreInterface::createSceneManager()
{
	mSceneMgr = mRoot->createSceneManager();
}

void OgreInterface::setupScene()
{
	//Camara
	mCamera = mSceneMgr->createCamera("MainCam");

	mCamera->setNearClipDistance(1);
	mCamera->setFarClipDistance(10000);
	mCamera->setAutoAspectRatio(true);

	mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("nCam");
	mCamNode->attachObject(mCamera);

	mCamNode->setPosition(0, 0, 1000);
	mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

	//Viewport
	vp = mWindow->addViewport(mCamera);

	vp->setBackgroundColour(ColourValue(1, 1, 1));
	mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

	//Lights
	mSceneMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));
	mainLight = mSceneMgr->createLight("MainLight");
	mainLight->setType(Ogre::Light::LT_DIRECTIONAL);
	mainLight->setDiffuseColour(0.75, 0.75, 0.75);
	mLightNode = mCamNode->createChildSceneNode("nLuz");
	mLightNode->attachObject(mainLight);

	mLightNode->setDirection(Ogre::Vector3(0, 0, -1));


	//finally something to render yas (Sinbad de prueba)
	Entity* ogreEntity = mSceneMgr->createEntity("Sinbad.mesh");;
	SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode->attachObject(ogreEntity);
	ogreNode->setScale(Vector3(35, 35, 35));

	mRoot->addFrameListener(this);
}

OgreInterface* OgreInterface::getSingleton()
{
	if (instance_ == nullptr) {
		instance_ = new OgreInterface();
	}

	return instance_;
}

void OgreInterface::shutdown()
{
	//mShaderGenerator->removeSceneManager(mSM);
	//mSM->removeRenderQueueListener(mOverlaySystem);
	
	mRoot->destroySceneManager(mSceneMgr);

	if (mWindow != nullptr)
	{
		mRoot->destroyRenderTarget(mWindow);
		mWindow = nullptr;
	}

	//overlay (?)

	if (SDL_win != nullptr)
	{
		SDL_DestroyWindow(SDL_win);
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		SDL_win = nullptr;
	}

	delete mRoot;
	mRoot = nullptr;

	delete instance_;
}

void OgreInterface::initApp()
{
	createRoot();
	setupResources();
	setConfiguration();
	mRoot->initialise(false);
	createWindow();
	initializeResources();
	createSceneManager();
	setupScene();
	mRoot->startRendering();
}

void OgreInterface::pollEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mRoot->queueEndRendering();
			break;
		case SDL_WINDOWEVENT:
			if (event.window.windowID == SDL_GetWindowID(SDL_win)) {
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					Ogre::RenderWindow* win = mWindow;
					//win->resize(event.window.data1, event.window.data2);  // IG2: ERROR 
					win->windowMovedOrResized();
					windowResized(win);
				}
			}
			break;
		default:
			//_fireInputEvent(convert(event));
			break;
		}
	}
}