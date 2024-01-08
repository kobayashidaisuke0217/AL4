#pragma once
class MoveMap
{
public:
	void Init();
	void Update();
	static MoveMap* GetInstance();

	int AtackLB[7][11];
	int AtackRB[7][11];
	int AtackMF[7][11];
	int AtackFW[7][11];
	int DefenceLB[5][9];
	int DefenceRB[5][9];
	int DefenceMF[5][9];
	int DefenceFW[5][9];
};

