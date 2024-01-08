#pragma once
#include "../Objects/Player/Player.h"
class PlayerManager
{
public:
	void Initialize(const std::vector<Model*>& models);
	void Update();
	void Draw(const ViewProjection& view);
private:
	std::unique_ptr<Player> player_[5];
	int selectNum_;
};

