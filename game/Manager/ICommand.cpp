#include "ICommand.h"

CharaMoveCommand::CharaMoveCommand(ICharactor* chara)
{
	chara_ = chara;
}

void CharaMoveCommand::Exec()
{
	chara_->Move(chara_->GetVelocity());
}

PassCommand::PassCommand(ICharactor* chara)
{
	chara_ = chara;
}

void PassCommand::Exec()
{
	chara_->Pass();
}
