#include "Framework.h"

#include <Windows.h>

LRESULT CALLBACK MainWindowProc(HWND, UINT, WPARAM, LPARAM);
 
INT WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ INT) {
	WNDCLASS cWnd = { sizeof(WNDCLASS) };
	cWnd.hInstance = hInstance;
	cWnd.lpfnWndProc = MainWindowProc;
	cWnd.lpszClassName = L"MAIN";
	cWnd.hbrBackground = CreateSolidBrush(RGB(51, 102, 153));
	RegisterClass(&cWnd);
	HWND hWnd = CreateWindowEx(WS_EX_TOPMOST,
		cWnd.lpszClassName, nullptr,
		WS_POPUP,
		GetSystemMetrics(SM_CXSCREEN)/2 - 400/2,
		GetSystemMetrics(SM_CYSCREEN)/2 - 150/2,
		400, 150,
		nullptr, nullptr,
		cWnd.hInstance, 
		nullptr);
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	MSG msg = {};
	while (GetMessage(&msg, nullptr, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<INT>(msg.message);
}
	
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT message, WPARAM wp_data, LPARAM lp_data) {

	// color for elements: RGB(204, 236, 255)

	static POINT pCursorPos;
	static RECT rectWnd;

	enum ApplicationCommands : size_t {
		AC_EXIT,
		AC_GET_RESULT,
		AC_GENERATE_RESULT
	};

	static HWND edit_control_for_input;
	static HWND generate_result_button;

	switch (message) {

	case WM_CREATE:
		edit_control_for_input = CreateWindow(
			L"EDIT", NULL, WS_CHILD | WS_VISIBLE |
			ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL,
			100, 50, 200, 20,
			hWnd, (HMENU)AC_GET_RESULT, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL
		);
		generate_result_button = CreateWindow(
			L"BUTTON", L"CALCULATE", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			150, 80, 100, 20,
			hWnd, (HMENU)AC_GENERATE_RESULT, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL
		);
		return EXIT_SUCCESS;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	case WM_PAINT:
		static HDC hDC;
		hDC = GetDC(hWnd);
		static RECT rectClient;
		GetClientRect(hWnd, &rectClient);
		static HPEN hPen = CreatePen(PS_SOLID, 2, RGB(204, 236, 255));
		SelectObject(hDC, hPen);
		MoveToEx(hDC, rectClient.right - 5, rectClient.top + 5, NULL);
		LineTo(hDC, rectClient.right - 15, rectClient.top + 15);
		MoveToEx(hDC, rectClient.right - 15, rectClient.top + 5, NULL);
		LineTo(hDC, rectClient.right - 5, rectClient.top + 15);
		ReleaseDC(hWnd, hDC);

		return EXIT_SUCCESS;

	case WM_NCHITTEST:
		GetCursorPos(&pCursorPos);
		GetWindowRect(hWnd, &rectWnd);
		if (pCursorPos.x > rectWnd.right - 15 and pCursorPos.x < rectWnd.right - 5
			and pCursorPos.y > rectWnd.top + 5 and pCursorPos.y < rectWnd.top + 15) {
			if (GetKeyState(VK_LBUTTON)) {
				PostMessage(hWnd, WM_DESTROY, NULL, NULL);
			}
			break;
		}
		static LRESULT result;
		result = DefWindowProc(hWnd, message, wp_data, lp_data);
		return result == HTCLIENT ? HTCAPTION : result;

	case WM_COMMAND:
		
		switch (wp_data) {

		case AC_EXIT:
			PostMessage(hWnd, WM_DESTROY, NULL, NULL);
			break;

		case AC_GENERATE_RESULT:
			try {
				static wchar_t* buffer = new wchar_t[1000];
				static const wchar_t* result;

				GetWindowText(edit_control_for_input, buffer, 1000);
				result = Calculate(buffer);
				SetWindowText(edit_control_for_input, result);
			}
			catch (...) {
				MessageBox(
					hWnd, 
					L"Что-то пошло не так, попробуйте еще раз...", 
					L"Ошибка!", 
					MB_OK | MB_ICONERROR
				);
			}
			break;

		default:
			break;
		}
		return 0;

	default:
		return DefWindowProc(hWnd, message, wp_data, lp_data);
	}
}