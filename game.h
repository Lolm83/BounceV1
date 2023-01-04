#pragma once

#include <vector>

namespace Tmpl8 {

	

class Surface;

class Player;
class Weapon;
class Command;
class InputHandler;
class Circle;

class Line
{
public:
	int m_x1, m_y1, m_x2, m_y2;
	unsigned int pixel;
};
class Physics
{
public:
	void CircleCircleCollision(Circle* circle, Circle* target);
	void CircleLineCollision(Circle* circle, Line* line);
	void CCDynamicCollision(std::vector<std::pair<Circle*, Circle*>> colliding_pair);
	bool PlayerCircleCollision(Player* player, Circle* circle);
	void WeaponCircleCollision(Weapon* weapon, Circle* circle);
	void WeaponPlayerCollision(Weapon* weapon, Player* player);

	void CircleMotion(Circle* circle, float dt, Surface* screen);
};

class Game
{
public:

	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float dt );
	void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseDown(int button);
	void MouseMove( int x, int y ) { /* implement if you want to detect mouse movement */ }
	void KeyUp(int key);
	void KeyDown(int key);

	void DrawCircle(Surface* surface, Circle* circle);
	

	void DrawIngameUI(Surface* screen, Player* p1, Player* p2);
	void DrawCredits();
	void ResetGame();

private:
	Surface* screen;

};




}; // namespace Tmpl8