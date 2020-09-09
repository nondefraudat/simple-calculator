#include <windows.h>
#include "deps/string_math.h"

LRESULT CALLBACK MainWindowProc(HWND, UINT, WPARAM, LPARAM);
 
int WINAPI wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE, _In_ LPWSTR cmd_line, _In_ INT cmd_mod) {

	const wchar_t* const kApplicationName = L"Simple Calculator";

	WNDCLASSEX main_application_window_template {
		sizeof(WNDCLASSEX)
	};
	main_application_window_template.hInstance = instance;
	main_application_window_template.hbrBackground = CreateSolidBrush(0x00000000);
	main_application_window_template.lpszClassName = kApplicationName;
	main_application_window_template.lpfnWndProc = MainWindowProc;
	
	RegisterClassEx(&main_application_window_template);

	HWND main_application_window = CreateWindowEx(
		WS_EX_TOPMOST, kApplicationName, L"", WS_POPUPWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 150,
		NULL, NULL, instance, NULL
	);

	if (!main_application_window) {
		MessageBox(
			NULL, 
			L"Ошибка при инициализации окна", 
			L"Ошибка!", 
			MB_OK | MB_ICONERROR
		);
		return 1;
	}

	ShowWindow(main_application_window, cmd_mod);

	MSG message = {

	};
	while (GetMessage(&message, NULL, NULL, NULL)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}
	
LRESULT CALLBACK MainWindowProc(HWND window, UINT message, WPARAM wp_data, LPARAM lp_data) {

	static enum ApplicationCommands {
		AC_EXIT,
		AC_GET_RESULT,
		AC_GENERATE_RESULT
	};

	static HWND edit_control_for_input;
	static HWND exit_button;
	static HWND generate_result_button;

	switch (message) {

	case WM_CREATE:
		edit_control_for_input = CreateWindow(
			L"EDIT", NULL, WS_CHILD | WS_VISIBLE |
			ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL,
			100, 50, 200, 20,
			window, (HMENU)AC_GET_RESULT, (HINSTANCE)GetWindowLongPtr(window, GWLP_HINSTANCE), NULL
		);
		exit_button = CreateWindow(
			L"BUTTON", L"[E]", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			375, 5, 15, 15,
			window, (HMENU)AC_EXIT, (HINSTANCE)GetWindowLongPtr(window, GWLP_HINSTANCE), NULL
		);
		generate_result_button = CreateWindow(
			L"BUTTON", L"CALCULATE", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			150, 80, 100, 20,
			window, (HMENU)AC_GENERATE_RESULT, (HINSTANCE)GetWindowLongPtr(window, GWLP_HINSTANCE), NULL
		);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	case WM_NCHITTEST:
		static LRESULT result;
		result = DefWindowProc(window, message, wp_data, lp_data);
		return result == HTCLIENT ? HTCAPTION : result;

	case WM_COMMAND:
		
		switch (wp_data) {

		case AC_EXIT:
			PostMessage(window, WM_DESTROY, NULL, NULL);
			break;

		case AC_GENERATE_RESULT:
			try {
				static wchar_t* buffer = new wchar_t[1000];
				static const wchar_t* result;

				using namespace string_math;

				GetWindowText(edit_control_for_input, buffer, 1000);
				result = ExtractSolution(buffer);
				SetWindowText(edit_control_for_input, result);
			}
			catch (...) {
				MessageBox(
					window, 
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
		return DefWindowProc(window, message, wp_data, lp_data);
	}
}