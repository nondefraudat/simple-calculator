#pragma once
#ifndef NO_GUI
# include <Windows.h>
# include <map>
# include <string>
# include <tuple>
#endif

class SimpleCalculator {
public:
#ifdef NO_GUI
	int run() noexcept;
#else
	SimpleCalculator(_In_ HINSTANCE appInstance) noexcept;
	~SimpleCalculator() noexcept;
	INT run() noexcept;

private:
	static std::map<HWND, SimpleCalculator*> instances;
	static std::map<HWND, std::tuple<HWND, HWND>> lineEdits;

	static const RECT inputLineEditRect;
	static const RECT outputLineEditRect;
	static const HMENU inputLineEditMessageId;

	static const COLORREF lineColor;
	static const COLORREF backgroundColor;
	static const HBRUSH lineBrush;
	static const HBRUSH backgroundBrush;
	static const HPEN pen;

	static const UINT bufferSize = 2048;

	WNDCLASS windowClass;
	HINSTANCE appInstance;
	HWND windowInstance;
	
	HWND inputLineEdit;
	HWND outputLineEdit;

	CHAR* inputLineEditTextBuffer;
	std::string outputLineEditTextBuffer;

	static LRESULT CALLBACK windowProc(HWND windowInstance,
				UINT message, WPARAM wParam, LPARAM lParam) noexcept;
	static void createWindow(HWND windowInstance) noexcept;
	void paintWindow() noexcept;
	LRESULT processMouseAction(UINT message,
			WPARAM wParam, LPARAM lParam) noexcept;
#endif
};
