#include "SpriteCommon.h"
#include <cassert>

#pragma comment(lib, "dxcompiler.lib")

void SpriteCommon::Initialize(DirectXCommon* dxCommon)
{
	HRESULT result{};
	
	dxCommon_ = dxCommon;

	//DXC������
	ComPtr<IDxcUtils> dxcUils;
	ComPtr<IDxcCompiler3> dxcCompiler;
	result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUils));
	assert(SUCCEEDED(result));
	result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(result));

	ComPtr<IDxcIncludeHandler> includeHandler;
	result = dxcUils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(result));

	//RootSignature�쐬
	D3D12_ROOT_SIGNATURE_DESC descriptorRootSignature;
	descriptorRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//�V���A���C�Y�Ƃ��ăo�C�i���ɂ���
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob =  nullptr;
	result = D3D12SerializeRootSignature(&descriptorRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(result)) {
		assert(false);
	}

	//�o�C�i������ɍ쐬
	result = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));

	assert(SUCCEEDED(result));

	//InPutLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[1] = {};
	inputElementDesc[0].SemanticName = "POSITION";
	inputElementDesc[0].SemanticIndex = 0;
	inputElementDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDesc[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDesc;
	inputLayoutDesc.NumElements = _countof(inputElementDesc);

	//BlendState
	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//Rasterrizer
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//���ʂ�`�悵�Ȃ�
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//�h��Ԃ�
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//�ǂݍ��ݏ���
	ComPtr<IDxcBlob> vertexShaderBlob = CompileShader(
	    L"Resources/shaders/SpriteVS.hlsl", L"vs_6_0", dxcUils.Get(), dxcCompiler.Get(),
	    includeHandler.Get());

	ComPtr<IDxcBlob> pixelShaderBlob = CompileShader(
	    L"Resources/shaders/SpritePS.hlsl", L"vs_6_0", dxcUils.Get(), dxcCompiler.Get(),
	    includeHandler.Get());

	//PipelibeState
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootsignature.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize()};
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize()};
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	result = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

}

IDxcBlob* SpriteCommon::CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler) {
	
   HRESULT result{};

   //hlsl�ǂݍ���
   IDxcBlobEncoding* shaderSource = nullptr;
   result = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);

   //�ǂ߂Ȃ�������~�߂�
   assert(SUCCEEDED(result));

   //�ǂݍ��񂾃t�@�C���̓��e��ݒ肷��
   DxcBuffer shaderSourceBuffer;
   shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
   shaderSourceBuffer.Size = shaderSource->GetBufferSize();
   shaderSourceBuffer.Encoding = DXC_CP_UTF8;

   LPCWSTR arguments[] = {
	   filePath.c_str(), // �R���p�C���Ώۂ�hlsl�t�@�C����
	   L"-E",L"main", // �G���g���[�|�C���g�̎w��A��{���C���ȊO
	   L"-T",profile, // shaderProfile�̐ݒ�
	   L"-Zi",L"-Qembed_debug", // �f�o�b�O�p�̏��𖄂ߍ���
	   L"-Od",           // �œK�����O���Ă���
	   L"-Zpr",          // ���������C�A�E�g�͍s�D��
   };

   //���ۂ�shader���R���p�C������
   IDxcResult* shaderResult = nullptr;
   result = dxcCompiler->Compile(
	   &shaderSourceBuffer, //�ǂݍ��񂾃t�@�C��
	   arguments, //�R���p�C���I�v�V����
	   _countof(arguments), //�R���p�C���I�v�V�����̐�
	   includeHandler,//include���܂܂ꂽ���X
	   IID_PPV_ARGS(&shaderResult)//�R���p�C������
   );

   //�R���p�C���G���[�ł͂Ȃ�dxc���N���ł��Ȃ��Ȃǒv���I�ȏ�
   assert(SUCCEEDED(result));

   //�x���E�G���[���o�Ă��烍�O�ɏo���Ď~�߂�
   IDxcBlobUtf8* shaderError = nullptr;
   shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
   if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
	   assert(false);
   }

   //�R���p�C�����ʂ�����s�p�̃o�C�i���������擾
   IDxcBlob* shaderBlob = nullptr;
   result = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
   assert(SUCCEEDED(result));

   //�����g��Ȃ����\�[�X�����
   shaderSource->Release();
   shaderResult->Release();
   //���s�p�̃o�C�i����ԋp
   return shaderBlob;
}