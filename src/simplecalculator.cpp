#include "simplecalculator.hpp"
#include <string-math.hpp>

using namespace std;

#ifdef NO_GUI
#include <iostream>

int SimpleCalculator::run() noexcept {
	string buffer;
	getline(cin, buffer);
	while (buffer != "exit()") {
		Expression expression = buffer;
		cout << expression << " = " << expression.calculate() << endl;
		getline(cin, buffer);
	}
	return EXIT_SUCCESS;
}

#else

SimpleCalculator::SimpleCalculator(HINSTANCE appInstance) noexcept
		: appInstance(appInstance) {
	
	windowClass = { sizeof(WNDCLASS) };
	windowClass.hInstance = appInstance;
	windowClass.lpfnWndProc = windowProc;
	windowClass.lpszClassName = "Simple Calculator";
	windowClass.hbrBackground = CreateSolidBrush(RGB(51, 102, 153));
	
	RegisterClass(&windowClass);
	inputLineEditTextBuffer = new CHAR[bufferSize + 1];

	windowInstance = CreateWindowEx(WS_EX_TOPMOST, windowClass.lpszClassName,
		nullptr, WS_VISIBLE | WS_POPUP,
		GetSystemMetrics(SM_CXSCREEN) / 2 - 400 / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - 150 / 2,
		400, 150, nullptr, nullptr, windowClass.hInstance, nullptr);

	instances[windowInstance] = this;
	tie(inputLineEdit, outputLineEdit) = lineEdits.at(windowInstance);
}

SimpleCalculator::~SimpleCalculator() noexcept {
	if (windowInstance) {
		DestroyWindow(windowInstance);
		instances.erase(windowInstance);
		delete[] inputLineEditTextBuffer;
	}
}


INT SimpleCalculator::run() noexcept {
	MSG msg = {};
	while (GetMessage(&msg, nullptr, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<INT>(msg.message);
}

std::map<HWND, SimpleCalculator*> SimpleCalculator::instances;
std::map<HWND, std::tuple<HWND, HWND>> SimpleCalculator::lineEdits;

const RECT SimpleCalculator::inputLineEditRect { 100, 50, 300, 70 };
const RECT SimpleCalculator::outputLineEditRect { 100, 80, 300, 100 };
const HMENU SimpleCalculator::inputLineEditMessageId = reinterpret_cast<HMENU>(1);

const COLORREF SimpleCalculator::lineColor(RGB(204, 236, 255));
const COLORREF SimpleCalculator::backgroundColor(RGB(51, 102, 153));
const HBRUSH SimpleCalculator::lineBrush = CreateSolidBrush(lineColor);
const HBRUSH SimpleCalculator::backgroundBrush = CreateSolidBrush(backgroundColor);
const HPEN SimpleCalculator::pen = CreatePen(PS_SOLID, 2, lineColor);

LRESULT SimpleCalculator::windowProc(HWND windowInstance,
			UINT message, WPARAM wParam, LPARAM lParam) noexcept {
	switch (message) {
		case WM_CREATE: {
			createWindow(windowInstance);
			return NULL;
		}
		case WM_DESTROY: {
			PostQuitMessage(EXIT_SUCCESS);
			return NULL;
		}
		case WM_PAINT: {
			instances.at(windowInstance)->paintWindow();
			return NULL;
		}
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORSTATIC: {
			HDC deviceContext = reinterpret_cast<HDC>(wParam);
			SetBkColor(deviceContext, backgroundColor);
			SetTextColor(deviceContext, lineColor);
			return reinterpret_cast<INT_PTR>(backgroundBrush);
		}
		case WM_NCHITTEST: {
			return instances.at(windowInstance)
					->processMouseAction(message, wParam, lParam);
		}
		case WM_COMMAND: {
			if (HIWORD(wParam) == EN_UPDATE) {
				if (reinterpret_cast<HMENU>(LOWORD(wParam)) == inputLineEditMessageId) {
					UpdateWindow(windowInstance);
				}
				return EXIT_SUCCESS;
			}
		}
	}
	return DefWindowProc(windowInstance, message, wParam, lParam);
}

void SimpleCalculator::createWindow(HWND windowInstance) noexcept {
	HINSTANCE appInstance = reinterpret_cast<HINSTANCE>(
			GetWindowLongPtr(windowInstance, GWLP_HINSTANCE));
	lineEdits[windowInstance] = {
		CreateWindow("EDIT", nullptr,
				WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL,
				inputLineEditRect.left + 2, inputLineEditRect.top + 2,
				inputLineEditRect.right - inputLineEditRect.left - 4,
				inputLineEditRect.bottom - inputLineEditRect.top - 4,
				windowInstance, inputLineEditMessageId, appInstance, nullptr),
		CreateWindow("EDIT", nullptr,
				WS_VISIBLE | WS_CHILD | ES_CENTER | ES_READONLY,
				outputLineEditRect.left + 2, outputLineEditRect.top + 2,
				outputLineEditRect.right - outputLineEditRect.left - 4,
				outputLineEditRect.bottom - outputLineEditRect.top - 4,
				windowInstance, nullptr, appInstance, nullptr)
	};
	UpdateWindow(windowInstance);
}

void SimpleCalculator::paintWindow() noexcept {
	GetWindowText(inputLineEdit, inputLineEditTextBuffer, bufferSize);
	
	double result = calculate(inputLineEditTextBuffer);
	outputLineEditTextBuffer = Expression(to_string(result)).parseDefinition();
	SetWindowText(outputLineEdit, outputLineEditTextBuffer.c_str());

	RECT windowRect;
	HDC deviceContext = GetDC(windowInstance);

	GetClientRect(windowInstance, &windowRect);
	FrameRect(deviceContext, &windowRect, lineBrush);
	FrameRect(deviceContext, &inputLineEditRect, lineBrush);
	FrameRect(deviceContext, &outputLineEditRect, lineBrush);
	SelectObject(deviceContext, pen);
	MoveToEx(deviceContext, windowRect.right - 5, windowRect.top + 5, NULL);
	LineTo(deviceContext, windowRect.right - 15, windowRect.top + 15);
	MoveToEx(deviceContext, windowRect.right - 15, windowRect.top + 5, NULL);
	LineTo(deviceContext, windowRect.right - 5, windowRect.top + 15);
	ReleaseDC(windowInstance, deviceContext);
}

LRESULT SimpleCalculator::processMouseAction(UINT message,
			WPARAM wParam, LPARAM lParam) noexcept {
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	RECT windowRect;
	GetWindowRect(windowInstance, &windowRect);
	if (cursorPos.x > windowRect.right - 15 and cursorPos.x < windowRect.right - 5
		and cursorPos.y > windowRect.top + 5 and cursorPos.y < windowRect.top + 15) {
		if (GetKeyState(VK_LBUTTON) >> 1) {
			PostMessage(windowInstance, WM_DESTROY, NULL, NULL);
		}
		return DefWindowProc(windowInstance, message, wParam, lParam);
	}
	if (cursorPos.x > windowRect.right - 15 and cursorPos.x < windowRect.right - 5
		and cursorPos.y > windowRect.top + 5 and cursorPos.y < windowRect.top + 15) {
		if (GetKeyState(VK_LBUTTON) >> 1) {
			PostMessage(windowInstance, WM_DESTROY, NULL, NULL);
		}
		return DefWindowProc(windowInstance, message, wParam, lParam);
	}
	static LRESULT result;
	result = DefWindowProc(windowInstance, message, wParam, lParam);
	return result == HTCLIENT ? HTCAPTION : result;
}

#endif
