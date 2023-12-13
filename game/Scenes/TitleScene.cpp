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
	pointY = { 2.1f,-0.9f,1.3f };
}

void TitleScene::Update()
{
	Rotation = createQuaternion(0.45f,Normalise({ 1.0f,0.4f,-0.2f }));
	
	Vector4 rot = QuaternionToVEctor4(Rotation);
	rotateMatrix= quaternionToMatrix(Normalize( Rotation));
	rotateByMatrix = vectorTransform(pointY, rotateMatrix);
	rotateByQuaternion = Rotatevector(pointY, Rotation);
	std::string result;
	ImGui::Begin("");
	ImGui::InputFloat4("rotation",&rot.x );
	ImGui::InputFloat3("rotateByMatrix", &rotateByMatrix.x);
	ImGui::InputFloat3("rotateByQuaternion", &rotateByQuaternion.x);
	
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			result += std::to_string(rotateMatrix.m[y][x]) + ", ";
			
		}
		result += "\n";
	
	}
	ImGui::Text("matrix\n");
	ImGui::Text("%s", result.c_str());
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
