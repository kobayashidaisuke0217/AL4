#include "InputHandler.h"
#include "Input.h"
InputHandler::InputHandler() {

}

InputHandler::~InputHandler()
{
}

ICommand* InputHandler::PlayerHandleInput(ICharactor* player)
{
	if (Input::GetInstance()->PushButtun(0,XINPUT_GAMEPAD_A)) {
		return new PassCommand(player);
	}
	else {
		return new CharaMoveCommand(player);
	}
	return nullptr;
}
