#include "DirectXCommon.h"
#include "Input.h"
#include "WinApp.h"
#include "SpriteCommon.h"
#include "Sprite.h"

#include "ImGuiManager.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ポインタ置き場
	Input* input_ = nullptr;
	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	//SpriteCommon* spriteCommon = nullptr;

	// WindowsAPI初期化処理
	winApp_ = new WinApp();
	winApp_->Initialize();

	// DirectX初期化処理
	dxCommon_ = new DirectXCommon();
	dxCommon_->Initialize(winApp_);

	// Input生成、初期化
	input_ = new Input();
	input_->Initialize(winApp_);

	ImGuiManager* imgui = ImGuiManager::Create();
	ImGuiManager::Initialize(winApp_->GetHwnd(), dxCommon_);

	//スプライト共通部の初期化
	SpriteCommon* common = new SpriteCommon;
	common->Initialize(dxCommon_);
	Sprite* sp = new Sprite();
	sp->Initialize(dxCommon_, common);



	// ゲームループ
	while (true) {
		// 更新
		if (winApp_->Update() == true) {
			break;
		}
		ImGuiManager::NewFrame();
		imgui->ShowDemo();

		// 入力
		input_->Update();

		// 更新前処理
		ImGuiManager::CreatCommand();
		dxCommon_->RreDraw();

		sp->Draw();

		// 更新後処理
		ImGuiManager::CommandsExcute(dxCommon_->GetCommandList());
		dxCommon_->PostDraw();
	}
	delete sp;
	delete common;

	delete imgui;

	delete input_;

	delete dxCommon_;

	winApp_->Finalize();
	delete winApp_;

	return 0;
}
