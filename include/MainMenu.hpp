#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP
#include <Scene.hpp>

class MainMenu : public Scene
{
public:
	void Update() override;
	void Render() override;
};

#endif // !MAIN_MENU_HPP
