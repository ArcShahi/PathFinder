#ifndef APP_HPP
#define APP_HPP

#include <memory>
#include <scene.hpp>


class App
{
public:
	App();
	~App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;

	void Run();

private:
	void Update();
	void Render();

private:
	std::unique_ptr<Scene> m_CurrentScene{};
};


#endif // !APP_HPP
