#include "ImGuiManager.h"

#include "External/imgui/imgui.h"
#include "External/imgui/imgui_impl_dx12.h"
#include "External/imgui/imgui_impl_win32.h"

void ImGuiManager::Initialize(HWND hwnd, DirectXCommon* dxCommon) {
	//Imgui‚Ì‰Šú‰»ˆ—
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init()

}
