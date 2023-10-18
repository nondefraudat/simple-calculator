#include "consoleapp.hpp"
#include <string-math.hpp>
#include <iostream>

using namespace std;

int ConsoleApp::run() noexcept {
	string expression;
	while (true) {
		cin >> expression;
		if (expression == "exit()") {
			return EXIT_SUCCESS;
		}
		cout << calculate(expression);
	}
}
