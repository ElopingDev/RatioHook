#include "../headers/gui.h"
#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_dx9.h"
#include "../ext/imgui/imgui_impl_win32.h"
#include "../headers/globals.h"
#include <stdexcept>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool gui::SetupWindowClass(const char* windowClassName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;

	if (!RegisterClassEx(&windowClass))
		return false;
	return true;
}
void gui::DestroyWindowClass() noexcept
{
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::SetupWindow(const char* windowName) noexcept
{
	window = CreateWindow(windowClass.lpszClassName, windowName, WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, 0, 0, windowClass.hInstance, 0);
	if (!window)
		return false;
	return true;
}

void gui::DestroyWindow() noexcept
{
	if (window)
		DestroyWindow(window);
}

bool gui::SetupDirectX() noexcept
{
	const auto handle = GetModuleHandle("d3d9.dll");
		if (!handle)
			return false;
	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);
	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(handle, "Direct3DCreate9"));
	if (!create)
		return false;
	d3d9 = create(D3D_SDK_VERSION);
	if (!d3d9)
		return false;

	D3DPRESENT_PARAMETERS params = { };
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = NULL;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = window;
	params.Windowed = 1;
	params.EnableAutoDepthStencil + 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = NULL;
	params.FullScreen_RefreshRateInHz + 0;
	params.PresentationInterval = 0;

	if (d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, window, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &params, &device) < 0) 
		return false;
	return true;
}

void gui::DestroyDirectX() noexcept
{
	if (device)
	{
		device->Release();
		device = NULL;
	}
	
	if (d3d9)
	{
		d3d9->Release();
		d3d9 = NULL;
	}
}

void gui::Setup()
{
	if (!SetupWindowClass("hackClas001"))
		throw std::runtime_error("Failed to create window class.");

	if (!SetupWindow("Hack Window"))
		throw std::runtime_error("Failed to create window.");

	if (!SetupDirectX())
		throw std::runtime_error("Failed to create device.");
	DestroyWindow();
	DestroyWindowClass();
}

void gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{
	auto params = D3DDEVICE_CREATION_PARAMETERS{ };
	device->GetCreationParameters(&params);
	window = params.hFocusWindow;
	originalWindowProcess = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess)));

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\CascadiaCode.ttf", 16.0f);
	//ImGui::PushFont(io.Fonts->Fonts[0]);
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.02f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.19f, 0.05f, 0.20f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.04f, 0.20f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.70f, 0.26f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.45f, 0.22f, 0.66f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.45f, 0.11f, 0.54f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.76f, 0.35f, 0.86f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.99f, 0.08f, 0.26f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.63f, 0.24f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.98f, 0.26f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.75f, 0.26f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.33f, 0.26f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.26f, 0.94f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.89f, 0.16f, 0.74f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.55f, 0.17f, 0.64f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.70f, 0.46f, 0.82f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.94f, 0.41f, 1.00f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);





	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
	setup = true;
}

void gui::Destroy() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(originalWindowProcess));
	DestroyDirectX();
}

void gui::Render() noexcept
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowSize(ImVec2(800, 400));

	//ImGui::ShowStyleEditor();
	if (ImGui::Begin("RatioHook DEV Build"), ImGuiWindowFlags_NoResize)
	{

		if (ImGui::BeginTabBar("Tabs"))
		{
			if (ImGui::BeginTabItem("Chams"))
			{
				
				ImGui::Checkbox("Chams", &hacks::chams);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Change enemy models color");
				ImGui::Checkbox("Through Walls", &hacks::walls);
				ImGui::ColorEdit3("Chams Visible Color", hacks::chamsColorVisible);
				ImGui::ColorEdit3("Chams Hidden Color", hacks::chamsColorHidden);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("BunnyHop"))
			{
				ImGui::Checkbox("BunnyHop", &hacks::bunnyhop);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Automatically bunnyhops when you hold space");
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Aimbot"))
			{
				ImGui::Checkbox("Aimbot", &hacks::aimbot);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Automatically aim for you when you shoot.");
				ImGui::InputFloat("FOV", &hacks::fov, 0.1f, 360.0f);
				ImGui::InputFloat("Smoothing", &hacks::smoothing, 0.f, 100.f);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Triggerbot"))
			{
				ImGui::Checkbox("Triggerbot", &hacks::triggerbot);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Automatically shoots when an enemy is in your crosshair");
				ImGui::SliderInt("Triggerbot Delay", &hacks::triggerDelay, 0, 500);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
			
		}
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
		gui::open = !gui::open;

	if (gui::open && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return 1L;

	return CallWindowProc(
		gui::originalWindowProcess, hWnd, msg, wParam, lParam);
	
}
