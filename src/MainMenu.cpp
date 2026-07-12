#include <MainMenu.hpp>
#include <imgui.h>


void MainMenu::Update()
{}

void MainMenu::Render()
{
	ImGui::Begin("Test");

	ImGui::Text("Hello");
	ImGui::End();
}
