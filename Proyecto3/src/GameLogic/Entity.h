#ifndef ENTITY_H_
#define ENTITY_H_

#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include <vector>

using namespace std;

class Entity     // clase que acepta componentes (contenedor vacio esperando funcionalidad)
{
private:
	bool active; // indica si la entidad esta activa o no

	// componentes de tres tipos
	vector<InputComponent*> inputComp_;
	vector<PhysicsComponent*> physicsComp_;
	vector<RenderComponent*> renderComp_;

public:
	Entity();

	inline void setActive(bool active_) { active = active_; }
	inline bool getActive() { return active; }

	// actualizan los componentes
	virtual void handleInput(float time);
	virtual void update(float time);
	virtual void render(float time);

	// a�aden componentes
	virtual void addInputComponent(InputComponent* ic/*, const SDL_Event& event*/);
	virtual void addPhysicsComponent(PhysicsComponent* pc);
	virtual void addRenderComponent(RenderComponent* rc);

	// quitan componentes
	virtual void delInputComponent(InputComponent* ic);
	virtual void delPhysicsComponent(PhysicsComponent* pc);
	virtual void delRenderComponent(RenderComponent* rc);

	virtual ~Entity();
};

#endif /* ENTITY_H_ */
