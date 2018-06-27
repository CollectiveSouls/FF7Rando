/*
 * yamada.h
*/

#ifndef YAMADA_H_
#define YAMADA_H_

// std libs
#include <string>

class Yamada {
	public:
		Yamada();
		Yamada(std::string str_target);
		void present(std::string str_target);
};

Yamada::Yamada(std::string str_target) {
	// self-test
	present(str_target);
}

void Yamada::present(std::string str_target) {
	std::string returnString = "Yamada is present and targeting: " + str_target;
	std::cout << returnString << std::endl;
}

#endif /* YAMADA_H_ */
