#pragma once
#include "DirectXCommon.h"
#include <d3d12.h>
#include <wrl.h>

#include "SpriteCommon.h"

class Sprite {
private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	//������
	void Initialize(DirectXCommon* dxCommon, SpriteCommon* common);

	void Draw();

private:
	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* common_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	ComPtr<ID3D12Resource> vertexResource;
};