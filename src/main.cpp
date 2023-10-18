#ifdef NO_GUI

#include "consoleapp.hpp"

int main() {
	ConsoleApp app;
	return app.run();
}

#else

#include "winapp.hpp"

LRESULT CALLBACK MainWindowProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI wWinMain(_In_ HINSTANCE appInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ INT) {
	WinApp app(appInstance);
	return app.run();
}

#endif
	