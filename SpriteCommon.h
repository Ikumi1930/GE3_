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
	//������
	void Initialize(DirectXCommon* dxCommon);
	void SpritePreDraw();

	//Getter
	ID3D12RootSignature* GetRootSignature() { return rootsignature.Get(); }
	ID3D12PipelineState* GetPipelineState() { return pipelineState.Get(); }
	DirectXCommon* GetDirectXCommon()       { return dxCommon_; }

	//�摜�ǂݍ���
	DirectX::ScratchImage LoadTexture(const std::wstring& filePath);
	//�ǂݍ��񂾉摜��GPU�i�V�F�[�_�[�ɑ���j
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImage);

private://�����o�֐��͂�����
	IDxcBlob* CompileShader(
		//�R���p�C������V�F�[�_�[�t�@�C���ւ̃p�X
	    const std::wstring& filePath,
		//�R���p�C���Ɏg�p����v���t�@�C��
		const wchar_t* profile,
		//�������Ő����������̂�3��
		IDxcUtils* dxcUtils,
	    IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

private://�����o�ϐ��͂�����
	DirectXCommon* dxCommon_ = nullptr;

	ComPtr<ID3D12RootSignature> rootsignature;
	ComPtr<ID3D12PipelineState> pipelineState;

};
