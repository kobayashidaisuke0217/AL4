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
	mul1 = QuaternionToVEctor4(Multiply(q1, q2));
	mul2 = QuaternionToVEctor4(Multiply(q2, q1));
	normal = QuaternionToVEctor4(Normalize(q1));
	norm = LengthQuaternion(q1);
	conj = QuaternionToVEctor4(Conjugate(q1));
	identity = QuaternionToVEctor4(IdentityQuaternion());
	inv = QuaternionToVEctor4(Inverse(q1));
	ImGui::Begin("Quaternion");
	ImGui::InputFloat4("Identity", &identity.x);
	ImGui::InputFloat4("Conjugate", &conj.x);
	ImGui::InputFloat4("Inverse", &inv.x);
	ImGui::InputFloat4("Normalise", &normal.x);
	ImGui::InputFloat4("Mul1", &mul1.x);
	ImGui::InputFloat4("Mul2", &mul2.x);
	ImGui::InputFloat("norm", &norm);

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
