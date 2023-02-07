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
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.02f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.19f, 0.05f, 0.20f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.04f, 0.20f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.70f, 0.26f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.45f, 0.22f, 0.66f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.45f, 0.11f, 0.54f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.76f, 0.35f, 0.86f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.99f, 0.08f, 0.26f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.75f, 0.26f, 0.98f, 0.40f);
	colors[ImGuiCol_Tab] = ImVec4(0.55f, 0.17f, 0.64f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.70f, 0.46f, 0.82f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.94f, 0.41f, 1.00f, 1.00f);


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

	ImGui::Begin("RatioHook DEV Build", &open);

	//ImGui::ShowStyleEditor();
	static bool BunnyHop = false;
	static bool Triggerbot = false;
	static bool Chams = false;

	ImGui::Checkbox("BunnyHop", &hacks::bunnyhop);
	ImGui::Checkbox("Triggerbot", &hacks::triggerbot);
	ImGui::Checkbox("Chams", &hacks::chams);
	ImGui::ColorEdit3("Chams Visible Color", hacks::chamsColorVisible);
	ImGui::ColorEdit3("Chams Hidden Color", hacks::chamsColorHidden);



	ImGui::End();

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
