#include "simplecalculator.hpp"

#ifndef NO_GUI
INT WINAPI WinMain(_In_ HINSTANCE appInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ INT) {
	SimpleCalculator app(appInstance);
#else
int main() {
	SimpleCalculator app;
#endif
	return app.run();
}
	