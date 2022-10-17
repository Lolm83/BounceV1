#include "game.h"
#include "surface.h"
#include <cstdio> //printf

#include <cmath>
#include "Circle.cpp"

namespace Tmpl8
{

	void Game::Init()
	{
		
	}
	
	Circle* p = new Circle();
	Circle* p2 = new Circle(600.f,50.f,25);
	Circle* p3 = new Circle(200.f, 400.f, 80);

	void Game::Shutdown()
	{
	}

	// Remember, (x-h)^2 + (y-k)^2 = r^2
	void Game::DrawCircle(Surface* surface, Circle* circle)
	{
		int h = circle->m_x, k = circle->m_y, r = circle->m_radius;
		Pixel* p = screen->GetBuffer();

		for (int x = h - r; x <= h + r; x++)
		{
			int y = sqrt((r * r) - (x - h) * (x - h)) + k;
			p[x + (800 * y)] = 0xffffff;
			y = -sqrt((r * r) - (x - h) * (x - h)) + k;
			p[x + (800 * y)] = 0xffffff;
		}

		for (int y = k - r; y <= k + r; y++)
		{
			int x = sqrt((r * r) - (y - k) * (y - k)) + h;
			p[x + (800 * y)] = 0xffffff;
			x = -sqrt((r * r) - (y - k) * (y - k)) + h;
			p[x + (800 * y)] = 0xffffff;
		}


	}

	void Game::Tick(float deltaTime)
	{
		screen->Clear(0);
		DrawCircle(screen, p);
		DrawCircle(screen, p2);
		DrawCircle(screen, p3);

	}
};