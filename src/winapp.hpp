#pragma once
#include <Windows.h>

class WinApp {
public:
	WinApp(_In_ HINSTANCE appInstance) noexcept;
	~WinApp() noexcept;

	int run() noexcept;

private:
	HWND windowInstance;
	
	static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg,
			WPARAM wParam, LPARAM lParam) noexcept;
};
