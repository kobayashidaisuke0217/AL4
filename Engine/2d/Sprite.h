#pragma once
#include "DirectXCommon.h"
#include"MyMath.h"
#include "BlueMoon.h"
#include "worldTransform.h"
#include "Texturemanager.h"
#include"DirectionalLight.h"
class Sprite
{
public:
	void Initialize(uint32_t texindex);


	void Draw(const Transform& uvTransform, const  Vector4& material);

	void Finalize();
	Vector2 texLeftTop = { 0.0f,0.0f };
	Vector2 textureSize = { 100.0f,100.0f };
	// 画像の中心点
	Vector2 anchorPoint_ = { 0.5f,0.5f };

	// スプライトの縦幅、横幅
	Vector2 size_ = { 10.0f,10.0f };

	float rotation = 0.0f;

	Vector2 position = { 50.0f,50.0f };
	uint32_t texindex_;
private:

	DirectXCommon* dxCommon_;
	Texturemanager* textureManager_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	VertexData* vertexData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;
	BlueMoon* engine_;
	DirectionalLight* directionalLight_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	uint32_t* indexData_;
	Matrix4x4* wvpData_;
	D3D12_RESOURCE_DESC resDesc_{};


private:
	void CreateVartexData();
	void CreateTransform();
	void SetColor();
	void AdjustTextureSize();
};

