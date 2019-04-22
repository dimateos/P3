//Nap_Time_Studios
#ifndef MESSAGES_H_
#define MESSAGES_H_

class nap_transform;

enum MessageId // DIFFERENT MESSAGE IDs
{
	//PHYSX
	PX_USERPTR,

	//UI
	ACTIVATE_UI,
	ADD_SCORE,

	//ENEMY GAMELOGIC
	HP_RESET,

	BUTTON_START_GAME
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
	inline Msg_ADD_SCORE(int score) : Message(ADD_SCORE), score_(score) { }
	int score_;
};

#endif /* MESSAGES_H_ */
