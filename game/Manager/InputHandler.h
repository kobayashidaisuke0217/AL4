#pragma once
#include "../Objects/ICharactor.h"
#include "ICommand.h"
class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	ICommand* PlayerHandleInput(ICharactor* player);
};

