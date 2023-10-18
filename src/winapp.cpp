#include "winapp.hpp"
#include <string-math.hpp>

using namespace std;

WinApp::WinApp(_In_ HINSTANCE appInstance) noexcept {
	WNDCLASS cWnd = { sizeof(WNDCLASS) };
	cWnd.hInstance = appInstance;
	cWnd.lpfnWndProc = windowProc;
	cWnd.lpszClassName = "Simple Calculator";
	cWnd.hbrBackground = CreateSolidBrush(RGB(51, 102, 153));
	RegisterClass(&cWnd);
	windowInstance = CreateWindowEx(WS_EX_TOPMOST,
			cWnd.lpszClassName, nullptr,
			WS_VISIBLE | WS_POPUP,
			GetSystemMetrics(SM_CXSCREEN) / 2 - 400 / 2,
			GetSystemMetrics(SM_CYSCREEN) / 2 - 150 / 2,
			400, 150,
			nullptr, nullptr,
			cWnd.hInstance,
			nullptr);
}

WinApp::~WinApp() noexcept {
	if (windowInstance) {
		DestroyWindow(windowInstance);
	}
}

int WinApp::run() noexcept {
	MSG msg = {};
	while (GetMessage(&msg, nullptr, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<INT>(msg.message);
}

LRESULT WinApp::windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept {
	const HMENU IDC_EDITIN = reinterpret_cast<HMENU>(1);

	static HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hWnd, GWLP_HINSTANCE));

	static COLORREF nColor = RGB(204, 236, 255);
	static COLORREF nBkColor = RGB(51, 102, 153);

	static HBRUSH hBrush = CreateSolidBrush(nColor);
	static HBRUSH hBkBrush = CreateSolidBrush(nBkColor);

	static HPEN hPen = CreatePen(PS_SOLID, 2, nColor);

	static RECT rectWnd;
	static RECT rectClient;

	static RECT rectEdit{ 100, 50, 300, 70 };
	static RECT rectStatic{ 100, 80, 300, 100 };

	static HWND hEditIn;
	static HWND hEditOut;

	static POINT pCursorPos;

	static HDC hDC;

	static char* lpszBuffer = new char[7000];
	static const char* lpszResult;

	switch (uMsg) {

	case WM_CREATE:
		hEditIn = CreateWindow("EDIT",
			nullptr,
			WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL,
			rectEdit.left + 2,
			rectEdit.top + 2,
			rectEdit.right - rectEdit.left - 4,
			rectEdit.bottom - rectEdit.top - 4,
			hWnd, IDC_EDITIN,
			hInstance,
			NULL);
		hEditOut = CreateWindow("EDIT",
			nullptr,
			WS_VISIBLE | WS_CHILD | ES_CENTER | ES_READONLY,
			rectStatic.left + 2,
			rectStatic.top + 2,
			rectStatic.right - rectStatic.left - 4,
			rectStatic.bottom - rectStatic.top - 4,
			hWnd, nullptr,
			hInstance,
			nullptr);
		UpdateWindow(hWnd);
		return NULL;

	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		return NULL;

	case WM_PAINT:
		GetWindowText(hEditIn, lpszBuffer, 7000);
		try {
			double result = calculate(lpszBuffer);
			static string strResult;
			strResult = to_string(result);
			lpszResult = strResult.c_str();
			SetWindowText(hEditOut, lpszResult);
		}
		catch (...) {
			SetWindowText(hEditOut, "ERROR");
		}
		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &rectClient);
		FrameRect(hDC, &rectClient, hBrush);
		FrameRect(hDC, &rectEdit, hBrush);
		FrameRect(hDC, &rectStatic, hBrush);
		SelectObject(hDC, hPen);
		MoveToEx(hDC, rectClient.right - 5, rectClient.top + 5, NULL);
		LineTo(hDC, rectClient.right - 15, rectClient.top + 15);
		MoveToEx(hDC, rectClient.right - 15, rectClient.top + 5, NULL);
		LineTo(hDC, rectClient.right - 5, rectClient.top + 15);
		ReleaseDC(hWnd, hDC);
		return NULL;

	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORSTATIC:
		hDC = reinterpret_cast<HDC>(wParam);
		SetBkColor(hDC, nBkColor);
		SetTextColor(hDC, nColor);
		return reinterpret_cast<INT_PTR>(hBkBrush);

	case WM_NCHITTEST:
		GetCursorPos(&pCursorPos);
		GetWindowRect(hWnd, &rectWnd);
		if (pCursorPos.x > rectWnd.right - 15 and pCursorPos.x < rectWnd.right - 5
			and pCursorPos.y > rectWnd.top + 5 and pCursorPos.y < rectWnd.top + 15) {
			if (GetKeyState(VK_LBUTTON) >> 1) {
				PostMessage(hWnd, WM_DESTROY, NULL, NULL);
			}
			break;
		}
		if (pCursorPos.x > rectWnd.right - 15 and pCursorPos.x < rectWnd.right - 5
			and pCursorPos.y > rectWnd.top + 5 and pCursorPos.y < rectWnd.top + 15) {
			if (GetKeyState(VK_LBUTTON) >> 1) {
				PostMessage(hWnd, WM_DESTROY, NULL, NULL);
			}
			break;
		}
		static LRESULT result;
		result = DefWindowProc(hWnd, uMsg, wParam, lParam);
		return result == HTCLIENT ? HTCAPTION : result;

	case WM_COMMAND:
		switch (HIWORD(wParam)) {
		case EN_UPDATE:
			if (reinterpret_cast<HMENU>(LOWORD(wParam)) == IDC_EDITIN) {
				UpdateWindow(hWnd);
			}
			return EXIT_SUCCESS;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
