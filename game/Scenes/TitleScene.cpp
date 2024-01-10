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
	Rotation = createQuaternion(0.3f,Normalise({ 0.71f,0.71f,0.0f }));
	rotation1 = createQuaternion(3.141592f, { 0.71f,0.0f,0.71f });
	interpolate[0] = Slerp(0.0f, Rotation, rotation1);
	interpolate[1] = Slerp(0.3f, Rotation, rotation1);
	interpolate[2] = Slerp(0.5f, Rotation, rotation1);
	interpolate[3] = Slerp(0.7f, Rotation, rotation1);
	interpolate[4] = Slerp(1.0f, Rotation, rotation1);
	Vector4 rot = QuaternionToVEctor4(Rotation);
	Vector4 inter[5];
	ImGui::Begin("");
	ImGui::InputFloat4("rotation", &rot.x);
	for (int i = 0; i < 5; i++) {
		inter[i] = QuaternionToVEctor4(interpolate[i]);
		ImGui::InputFloat4("interpolate", &inter[i].x);
	}
	ImGui::End();
	//std::string result;
	
	
	//
	//for (int y = 0; y < 4; y++) {
	//	for (int x = 0; x < 4; x++) {
	//		result += std::to_string(rotateMatrix.m[y][x]) + ", ";
	//		
	//	}
	//	result += "\n";
	//
	//}
	//ImGui::Text("matrix\n");
	//ImGui::Text("%s", result.c_str());
	//ImGui::End();
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
