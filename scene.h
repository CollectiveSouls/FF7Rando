/*
 * scene.h
*/

#ifndef SCENE_H_
#define SCENE_H_

// std libs
#include <string>

class Scene {
	public:
		Scene();
		Scene(std::string str_target);
		void present(std::string str_target);
};

Scene::Scene(std::string str_target) {
	// self-test
	present(str_target);
}

void Scene::present(std::string str_target) {
	std::string returnString = "Scene is present and targeting: " + str_target;
	std::cout << returnString << std::endl;
}

#endif /* SCENE_H_ */
