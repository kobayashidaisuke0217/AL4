#include "Sprite.h"

void Sprite::Initialize(uint32_t texindex = UINT32_MAX)
{
	texindex_ = texindex;
	dxCommon_ = DirectXCommon::GetInstance();
	engine_ = BlueMoon::GetInstance();
	textureManager_ = Texturemanager::GetInstance();
	CreateVartexData();
	SetColor();
	CreateTransform();
	if (texindex != UINT32_MAX) {
		texindex_ = texindex;
		AdjustTextureSize();
		size_ = textureSize;
	}
	ID3D12Resource* textureBuffer = textureManager_->GetTextureResource(texindex_).Get();

	// 指定番号の画像が読み込み済みなら
	if (textureBuffer) {
		//// テクスチャ情報取得
		resDesc_ = textureBuffer->GetDesc();
		// UVの頂点
		float tex_left = texLeftTop.x / resDesc_.Width;
		float tex_right = (texLeftTop.x + textureSize.x) / resDesc_.Width;
		float tex_top = texLeftTop.y / resDesc_.Height;
		float tex_bottom = (texLeftTop.y + textureSize.y) / resDesc_.Height;
		// 頂点のUVに反映
		vertexData_[0].texcoord = { tex_left, tex_bottom };
		vertexData_[0].normal = { 0.0f,0.0f,-1.0f };
		vertexData_[1].texcoord = { tex_left, tex_top };
		vertexData_[1].normal = { 0.0f,0.0f,-1.0f };
		vertexData_[2].texcoord = { tex_right, tex_bottom };
		vertexData_[2].normal = { 0.0f,0.0f,-1.0f };
		vertexData_[3].texcoord = { tex_right, tex_top };
		vertexData_[3].normal = { 0.0f,0.0f,-1.0f };
	}

}
void Sprite::SetColor() {
	materialResource_ = DirectXCommon::CreateBufferResource(dxCommon_->GetDevice().Get(), sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->uvTransform = MakeIdentity4x4();
}
void Sprite::Draw(const Transform& uvTransform, const Vector4& material)
{

	AdjustTextureSize();

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;


	ID3D12Resource* textureBuffer = textureManager_->GetTextureResource(texindex_).Get();
	//// テクスチャ情報取得
	resDesc_ = textureBuffer->GetDesc();
	// UVの頂点
	float tex_left = texLeftTop.x / resDesc_.Width;
	float tex_right = (texLeftTop.x + textureSize.x) / resDesc_.Width;
	float tex_top = texLeftTop.y / resDesc_.Height;
	float tex_bottom = (texLeftTop.y + textureSize.y) / resDesc_.Height;
	// 頂点のUVに反映
	vertexData_[0].position = { left,bottom, 0.0f, 1.0f };// 左下
	vertexData_[1].position = { left,top, 0.0f, 1.0f };// 左上
	vertexData_[2].position = { right,bottom, 0.0f, 1.0f };// 右下
	vertexData_[3].position = { right,top, 0.0f, 1.0f };// 右上
	vertexData_[0].texcoord = { tex_left, tex_bottom };
	vertexData_[1].texcoord = { tex_left, tex_top };
	vertexData_[2].texcoord = { tex_right, tex_bottom };
	vertexData_[3].texcoord = { tex_right, tex_top };
	Matrix4x4 worldMatrix = MakeAffineMatrix({ size_.x,size_.y,1.0f }, { 0.0f,0.0f,rotation }, { position.x,position.y,0.0f });
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	Matrix4x4 projectionmatrix = MakeOrthographicMatrix(0.0f, 0.0f, (float)dxCommon_->GetWin()->kClientWidth, (float)dxCommon_->GetWin()->kClientHeight, 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionmatrix));

	Transform uvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	Matrix4x4 uvtransformMtrix = MakeScaleMatrix(uvTransform.scale);
	uvtransformMtrix = Multiply(uvtransformMtrix, MakeRotateZMatrix(uvTransform.rotate.z));
	uvtransformMtrix = Multiply(uvtransformMtrix, MakeTranslateMatrix(uvTransform.translate));

	*materialData_ = { material,false };
	materialData_->uvTransform = uvtransformMtrix;
	*wvpData_ = worldViewProjectionMatrix;

	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView);
	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定すると考えておけばいい
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(texindex_));
	dxCommon_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	//dxCommon_->GetCommandList()->DrawInstanced(6, 1, 0, 0);

}


void Sprite::Finalize()
{

}

void Sprite::CreateVartexData()
{
	vertexResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice().Get(), sizeof(VertexData) * 6);

	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;

	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	indexResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice().Get(), sizeof(uint32_t) * 6);

	indexBufferView.BufferLocation = indexResource_->GetGPUVirtualAddress();

	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;

	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	vertexData_[0].position = { left,bottom, 0.0f, 1.0f };// 左下
	vertexData_[1].position = { left,top, 0.0f, 1.0f };// 左上
	vertexData_[2].position = { right,bottom, 0.0f, 1.0f };// 右下
	vertexData_[3].position = { right,top, 0.0f, 1.0f };// 右上

	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;
	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;



}

void Sprite::CreateTransform()
{

	wvpResource_ = DirectXCommon::CreateBufferResource(dxCommon_->GetDevice().Get(), sizeof(Matrix4x4));
	wvpResource_->Map(0, NULL, reinterpret_cast<void**>(&wvpData_));
	*wvpData_ = MakeIdentity4x4();
}
void Sprite::AdjustTextureSize() {
	ID3D12Resource* textureBuffer = textureManager_->GetTextureResource(texindex_).Get();

	//テクスチャの情報を取得
	resDesc_ = textureBuffer->GetDesc();
	//テクスチャサイズの初期化
	textureSize = { float(resDesc_.Width),float(resDesc_.Height) };
}