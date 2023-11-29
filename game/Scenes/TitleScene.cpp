#include "TitleScene.h"
#include "ImGuiManger.h"
#include <string>
TitleScene::~TitleScene()
{
}
void TitleScene::Initialize()
{	
	input = Input::GetInstance();
	count = 0;

}

void TitleScene::Update()
{
	Matrix4x4 rotatematrix0 =DirectionToDirection(Normalise({1.0f,0.0f,0.0f}),Normalise({-1.0f,0.0f,0.0f}));
	Matrix4x4 rotateMatrix1 = DirectionToDirection(from0,to0);
	Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);

	std::string result;

	std::string result1;

	std::string result2;
	ImGui::Begin("matrix");
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			result += std::to_string(rotatematrix0.m[y][x])+", ";
			result1 += std::to_string(rotateMatrix1.m[y][x]) + ", ";
			result2 += std::to_string(rotateMatrix2.m[y][x]) + ", ";
		
		}
		result += "\n";
		result1 += "\n";
		result2 += "\n";
	}
	ImGui::Text("matrix0\n" );
	ImGui::Text("%s", result.c_str());
	ImGui::Text("matrix1\n");
	ImGui::Text("%s", result1.c_str());
	ImGui::Text("matrix2\n");
	ImGui::Text("%s", result2.c_str());
	ImGui::End();
	ImGui::Begin("SceneManager");
	ImGui::InputInt("SceneNum", &sceneNum);
	ImGui::Text("count %d",count);
	ImGui::End();
	if (input->PushKey(DIK_SPACE)) {
		sceneNum = GAME_SCENE;
		count++;
	}
	XINPUT_STATE joyState;
	if (!input->GetJoystickState(0, joyState)) {
		return;
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		sceneNum = GAME_SCENE;
	}
	if (sceneNum < 0) {
		sceneNum = 0;
		
	}
	/*if (count >= 60) {
		sceneNum=GAME_SCENE;
	}*/
}

void TitleScene::Draw()
{
	ImGui::Begin("TITLE");
	ImGui::Text("PushA:Start");
	ImGui::End();
}

void TitleScene::Finalize()
{
}
