#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <raylib.h>

class Star
{
public:
	Star()
	{
		const auto Width{ static_cast<float>(GetScreenWidth()) };
		const auto Height{ static_cast<float>(GetScreenHeight()) };

		x = GetRandomValue(-Width / 2, Width / 2);
		y = GetRandomValue(-Height / 2, Height / 2);
		z = GetRandomValue(0, Width);
		pz = z;
	}

public:
	void Update(float dt);
	void Render();

private:
	float Map(float value, float InMin, float InMax, float OutMin, float OutMax);
	void Reset();

private:
	float x{}, y{}, z{}, pz{}, speed{ 500.f };
};

inline float Star::Map(float value, float InMin, float InMax, float OutMin, float OutMax)
{
	return (value - InMin) / (InMax - InMin) * (OutMax - OutMin) + OutMin;
}

inline void Star::Reset()
{
	const float Width = static_cast<float>(GetScreenWidth());
	const float Height = static_cast<float>(GetScreenHeight());

	x = static_cast<float>(GetRandomValue(-Width / 2, Width / 2));
	y = static_cast<float>(GetRandomValue(-Height / 2, Height / 2));

	z = Width;
	pz = z;
}

inline void Star::Update(float dt)
{

	const float width = static_cast<float>(GetScreenWidth());

	speed = Map(GetMousePosition().x+100,
		0.0f,
		width,
		10.0f,
		1000.0f);

	z -= speed * dt;

	if (z < 1.0f)
	{
		Reset();
	}
}

inline void Star::Render()
{

	const auto Width = GetScreenWidth();
	const auto Height = GetScreenHeight();

	// Center horizontally and  vertically
	float sx{ Map(x / z, 0.0, 1.0f, 0.0f, Width) + Width *0.5f };
	float sy{ Map(y / z, 0.0, 1.0f, 0.0f, Height) + Height*0.5f };
	float r = Map(z, 0.0, Width, 3.f, 0.f);
	DrawCircle(sx, sy, r, WHITE);

	float px{ Map(x / pz, 0.0f, 1.0f, 0.0f, Width) + Width *0.5f };
	float py{ Map(y / pz, 0.0f, 1.0f, 0.0, Height) + Height *0.5f };

	pz = z;

	DrawLine(px, py, sx, sy, SKYBLUE);
}



#endif // !ANIMATION_HPP
