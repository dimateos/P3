//Nap_Time_Studios
#ifndef MESSAGES_H_
#define MESSAGES_H_

class nap_transform;

enum MessageId // DIFFERENT MESSAGE IDs
{
	//STATES
	PUSH_STATE,
	POP_STATE,

	//UI
	ACTIVATE_UI,
	ADD_SCORE,
	CHECK_HP,
	END_INV,
	HP_DAMAGE,
	ENEMY_DAMAGE,
	PLAYER_DEAD,

	//PHYSX
	PX_USERPTR,

	//ENEMY GAMELOGIC
	HP_RESET,
	RESET_PULL,
	RESET_HP,

	//CHANGE STATES
	STATE_CHANGED,

	//SOUNDS
	PLAY_SOUND,
	STOP_SOUND
};

struct Message {
	inline Message(MessageId id) : id_(id) { }
	MessageId id_;
};

struct Msg_PX_userPtr: public Message
{
	inline Msg_PX_userPtr(nap_transform *trans) : Message(PX_USERPTR), trans_(trans) {}
	nap_transform *trans_;
};

struct Msg_ADD_SCORE:public Message {
	inline Msg_ADD_SCORE(int score_) : Message(ADD_SCORE), score_(score_) { }
	int score_;
};

struct Msg_HP_DAMAGE :public Message {
	inline Msg_HP_DAMAGE(int damage) : Message(HP_DAMAGE), damage_(damage) { }
	int damage_;
};

struct Msg_PLAYER_DEAD :public Message {
	inline Msg_PLAYER_DEAD(std::string name, int score) : Message(PLAYER_DEAD), name_(name), score_(score) { }
	std::string name_;
	int score_;
};

#endif /* MESSAGES_H_ */
