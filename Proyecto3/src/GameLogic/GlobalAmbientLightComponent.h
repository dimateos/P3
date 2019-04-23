#ifndef GLOBALAMBIENTLIGHTCOMPONENT_H_
#define GLOBALAMBIENTLIGHTCOMPONENT_H_

#include "Component.h"

namespace Ogre {
	class SceneNode;
}

class GlobalAmbientLightComponent : public Component
{
private:
	void setLight();
	nap_vector3 in_ambientLight;

public:
	GlobalAmbientLightComponent() {};
	virtual ~GlobalAmbientLightComponent() {};
	GlobalAmbientLightComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();
	virtual void lateSetUp();
};

#endif //GLOBALAMBIENTLIGHTCOMPONENT_H_