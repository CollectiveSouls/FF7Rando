/*
 * window.h
*/

#ifndef WINDOW_H_
#define WINDOW_H_

// std libs
#include <string>

class Window {
	public:
		Window();
		Window(std::string str_target);
		void present(std::string str_target);
};

Window::Window(std::string str_target) {
	// self-test
	present(str_target);
}

void Window::present(std::string str_target) {
	std::string returnString = "Window is present and targeting: " + str_target;
	std::cout << returnString << std::endl;
}

#endif /* WINDOW_H_ */
