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
	Matrix4x4 rotatematrix = MakeRotateAxisAngle(axis, angle);
	std::string result;
	ImGui::Begin("matrix");
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			result += std::to_string(rotatematrix.m[y][x])+", ";


		
		}
		result += "\n";
	}
	ImGui::Text("%s", result.c_str());
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
