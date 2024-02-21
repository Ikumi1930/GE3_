﻿#include "DirectXCommon.h"
#include "Input.h"
#include "WinApp.h"
#include "SpriteCommon.h"
#include "Sprite.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ポインタ置き場
	Input* input_ = nullptr;
	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* spriteCommon = nullptr;

	// WindowsAPI初期化処理
	winApp_ = new WinApp();
	winApp_->Initialize();

	// DirectX初期化処理
	dxCommon_ = new DirectXCommon();
	dxCommon_->Initialize(winApp_);

	// Input生成、初期化
	input_ = new Input();
	input_->Initialize(winApp_);

	//スプライト共通部の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize();
	
	Sprite* sprite = new Sprite();
	sprite->Initialize();

	// ゲームループ
	while (true) {
		// 更新
		if (winApp_->Update() == true) {
			break;
		}
		// 入力
		input_->Update();

		// 更新前処理
		dxCommon_->RreDraw();

		// 更新後処理
		dxCommon_->PostDraw();
	}
	delete input_;

	delete dxCommon_;

	winApp_->Finalize();
	delete winApp_;

	delete spriteCommon;
	delete sprite;

	return 0;
}
