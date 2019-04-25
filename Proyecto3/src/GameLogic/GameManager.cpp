#include "GameManager.h"
#include "GOFactory.h"
#include "OverlayComponent.h"
#include "GameStateMachine.h"
#include "HPComponent.h"
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <RenderSystemInterface.h>
#include <LogSystem.h>
#include <OgreCamera.h>
#include <OgreViewport.h>

using namespace Ogre;

// updates player HP and score information
void GameManager::updateUI()
{
	if(playerHP_ != nullptr)
		HPText->setCaption("HP: " + std::to_string(playerHP_->getHP()));
	else 
		LogSystem::Log("Componente HPComponent no establecido. No es posible mostrar HP", LogSystem::GM);

	ScoreText->setCaption("SCORE: " + std::to_string(score));
}

void GameManager::setUp() {
	if (isInited()) return;
	setInited();

	// hide of death panel
	overlayComp = static_cast<OverlayComponent*>(owner_->getComponent("canvas"));
	overlayComp->hidePanelByName("DEATH_PANEL");

	// player HP and score
	HPText = static_cast<TextAreaOverlayElement*>(RenderSystemInterface::getSingleton()->getOverlayElement("HP_Text"));
	ScoreText = static_cast<TextAreaOverlayElement*>(RenderSystemInterface::getSingleton()->getOverlayElement("SCORE_Text"));

	player_ = GameStateMachine::getSingleton()->currentState()->getPlayer();
	playerHP_ = static_cast<HPComponent*>(player_->getComponent("hp_component"));

	// scope
	Ogre::OverlayElement* scope = RenderSystemInterface::getSingleton()->getOverlayElement("Scope");
	RenderSystemInterface::getSingleton()->setOverlayElementCenteredPosition
	(scope, RenderSystemInterface::getSingleton()->getCamera()->getViewport()->getActualWidth() / 2,
		RenderSystemInterface::getSingleton()->getCamera()->getViewport()->getActualHeight() / 2);

	updateUI();
}

void GameManager::update(GameObject * o, double time) {
	if(playerHP_ != nullptr && playerHP_->getHP() <= 0) // EN UN FUTURO ESTO PUSEHARA UN ESTADO DE MUERTE
		overlayComp->showPanelByName("DEATH_PANEL");

	// or if the player has completed the round (NEXT ROUND state)
}

void GameManager::receive(Message * msg)
{
	if (msg->id_ == ADD_SCORE) {
		addScore(static_cast<Msg_ADD_SCORE*>(msg)->score_);
		updateUI();
	}
}


REGISTER_TYPE(GameManager);