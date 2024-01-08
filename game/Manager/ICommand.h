#pragma once
#include "../Objects/ICharactor.h"
class ICommand
{
public:

	virtual ~ICommand() = default;
	virtual void Exec() = 0;
};
class CharaMoveCommand :public ICommand {
public:
	CharaMoveCommand(ICharactor* chara);
	void Exec()override;
private:
	ICharactor* chara_;
};
class PassCommand :public ICommand {
public:
	PassCommand(ICharactor* chara);
	void Exec()override;
private:
	ICharactor* chara_;
};
