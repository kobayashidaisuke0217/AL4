#include "Fade.h"
#include "Engine/base/ImGuiManger.h"
Fade* Fade::GetInstance()
{
	static Fade Instance;
	return &Instance;
}
void Fade::Initialize()
{
	blueMoon_ = BlueMoon::GetInstance();
	texturManager_ = Texturemanager::GetInstance();
	sprite_ = std::make_unique<Sprite>();
	tex = texturManager_->Load("Resource/wi.png");
	sprite_->Initialize(tex);
	
	saikorotransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	SpriteuvTransform =
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	sprite_->position = { 640.0f,360.0f };
	sprite_->size_ = { 40.0f,40.0f };

}

void Fade::Update()
{
	Move();
	if (material.w < 0.0f) {
		moveFlag_ = false;
		arfaUp_ = true;
		material.w = 0.0f;
	}
	if (material.w >= 1.0f) {
		ChangeFlag_ = true;
		arfaUp_ = false;
	}
	ImGui::Begin("change");
	ImGui::DragFloat4("mat", &sprite_->position.x);
	ImGui::End();
}

void Fade::SelectUpdate()
{

	if (selectmoveFlag_ == true) {
		if (selectarfaUp_ == true) {
			material.w += 0.02f;
		}
		else {
			material.w -= 0.02f;
		}

	}

	if (material.w < 0.3f) {
		selectmoveFlag_ = false;
		selectarfaUp_ = true;
		material.w = 0.3f;
	}
	if (material.w >= 1.0f) {

		selectarfaUp_ = false;
	}
	ImGui::Begin("change");
	ImGui::DragFloat4("translate", &material.x);
	ImGui::End();
}

void Fade::Draw()
{
	blueMoon_->SpritePreDraw();
	blueMoon_->SetBlendMode(0);
	sprite_->Draw( SpriteuvTransform, material);

}

void Fade::Move()
{
	ChangeFlag_ = false;
	if (moveFlag_ == true) {
		if (arfaUp_ == true) {
			material.w += 0.01f;
		}
		else {
			material.w -= 0.01f;
		}

	}

}