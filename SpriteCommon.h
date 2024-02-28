#pragma once
#include <Windows.h>
#include <wrl.h>
#include <string>
#include <dxcapi.h>
#include <d3d12.h>

#include "DirectXCommon.h"

#include <DirectXTex.h>

class SpriteCommon {
private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	//初期化
	void Initialize(DirectXCommon* dxCommon);
	void SpritePreDraw();

	//Getter
	ID3D12RootSignature* GetRootSignature() { return rootsignature.Get(); }
	ID3D12PipelineState* GetPipelineState() { return pipelineState.Get(); }
	DirectXCommon* GetDirectXCommon()       { return dxCommon_; }

	//画像読み込み
	DirectX::ScratchImage LoadTexture(const std::wstring& filePath);
	//読み込んだ画像をGPU（シェーダーに送る）
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImage);

private://メンバ関数はここへ
	IDxcBlob* CompileShader(
		//コンパイルするシェーダーファイルへのパス
	    const std::wstring& filePath,
		//コンパイルに使用するプロファイル
		const wchar_t* profile,
		//初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
	    IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

private://メンバ変数はここへ
	DirectXCommon* dxCommon_ = nullptr;

	ComPtr<ID3D12RootSignature> rootsignature;
	ComPtr<ID3D12PipelineState> pipelineState;

};
