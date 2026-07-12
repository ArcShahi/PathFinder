#ifndef SCENE_HPP
#define SCENE_HPP



class Scene
{
public:
	virtual ~Scene() = default;

	virtual void OnEnter() {}
	virtual void OnExit() {}

	virtual void Update() = 0;
	virtual void Render() = 0;
};
#endif // !SCENE_HPP
