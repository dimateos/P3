#include <ctype.h>
#include <PxPhysicsAPI.h>
#include "callbacks.hpp"

using namespace physx;

PxDefaultAllocator gAllocator;
PxDefaultErrorCallback gErrorCallback;

PxFoundation *gFoundation = NULL;
PxPhysics *gPhysics = NULL;

PxMaterial *gMaterial = NULL;
PxMaterial *bouncyMaterial = NULL;

PxPvd *gPvd = NULL;

PxDefaultCpuDispatcher *gDispatcher = NULL;
PxScene *gScene = NULL;

// Collisions
ContactReportCallback gContactReportCallback;

// Initialize physics engine
// Add custom code at the end of the function
void initPhysics(bool interactive) {
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport *transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f); //not color -> friction etc
	bouncyMaterial = gPhysics->createMaterial(0.5f, 0.5f, 1.0f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.gravity = { 0, -9.8, 0 };
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	// sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);
	// ------------------------------------------------------

	// Add custom application code
	//end of custom code
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t) {
	PX_UNUSED(interactive);

	//scene nvidia physics
	gScene->simulate(t);
	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive) {
	PX_UNUSED(interactive);

	// Add custom application code
	//end custom

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------

	gPhysics->release();

	PxPvdTransport *transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}
