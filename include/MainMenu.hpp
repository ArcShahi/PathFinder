#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include <State.hpp>
#include <Star.hpp>
#include <vector>

class MainMenu : public State
{
public:
	MainMenu();
	void Update() override;
	void Render() override;
	void DrawCenteredText(const char* text, int y, int FontSize, Color color);
private:
	std::vector<Star> m_Stars{ 500 };
	Algorithms m_RunningAlgo{ Algorithms::AStar };
};

#endif // !MAIN_MENU_HPP
