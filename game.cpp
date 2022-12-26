#include "game.h"
#include "surface.h"
#include <cstdio> //printf

#include <cmath>
#include "Circle.cpp"
#include "Command.cpp"
#include "Physics.cpp"

#include "Player.cpp"

namespace Tmpl8
{
	
	std::vector<Circle*>::iterator circle_iter;
	Player player;
	Player player2(300, 300, &mort_sheet, &mort_sheetL);

	Sprite background(new Surface("assets/_CloudsBack.png"), 1);


	void Game::Init()
	{
		
		srand(time(0));

		p_circles.push_back(new Circle());
		p_circles.push_back(new Circle(600.f, 50.f, 25, 200, 0));
		p_circles.push_back(new Circle(200.f, 400.f, 80, -180, 0));
		p_circles.push_back(new Circle(26.f, 50.f, 25, 200, 0));
		p_circles.push_back(new Circle(400.f, 400.f, 80, -180, 0));

		player.m_boundCircle = p_circles[0];
		player.m_y = player.m_boundCircle->m_y - player.m_boundCircle->m_radius - player.m_sheet[0].GetHeight();
		player.m_x = player.m_boundCircle->m_x;

	}
	
	void Game::Shutdown()
	{
	}

	// Remember, (x-h)^2 + (y-k)^2 = r^2
	void Game::DrawCircle(Surface* surface, Circle* circle)
	{
		float circle_diameter = (2 * circle->m_radius);
		float sprite_diameter = circle->m_sprites[circle->m_color].GetWidth();

		int center_x = circle->m_x - circle->m_radius;
		int center_y = circle->m_y - circle->m_radius;
	
		for (int x = 0; x < circle_diameter; x++) for (int y = 0; y < circle_diameter; y++)
		{
			int u = (int)((float)x * ((float)sprite_diameter / (float)circle_diameter));
			int v = (int)((float)y * ((float)sprite_diameter / (float)circle_diameter));
			Pixel color = circle->m_sprites[circle->m_color].GetBuffer()[u + v * circle->m_sprites[circle->m_color].GetPitch()];
			if (center_x + x >= 0 && center_x + x <= surface->GetWidth())
				if (center_y + y >= 0 && center_y + y <= surface->GetHeight())
					if (color & 0xffffff) surface->GetBuffer()[center_x + x + ((center_y + y) * surface->GetPitch())] = color;
		}

		/* Before I had sprites for the circles, I would draw them myself
		
		int h = circle->m_x, k = circle->m_y, r = circle->m_radius;
		Pixel* p = screen->GetBuffer();

		for (int x = h - r; x <= h + r; x++)
		{
			int y = sqrt((r * r) - (x - h) * (x - h)) + k;
			if (x + (800 * y) >= 0 && x + (800 * y) <= (800 + (512*800)))
				p[x + (800 * y)] = 0x000000;

			y = -sqrt((r * r) - (x - h) * (x - h)) + k;
			if (x + (800 * y) >= 0 && x + (800 * y) <= (800 + (512 * 800)))
             	p[x + (800 * y)] = 0x000000;
		}

		for (int y = k - r; y <= k + r; y++)
		{
			int x = sqrt((r * r) - (y - k) * (y - k)) + h;
			if (x + (800 * y) >= 0 && x + (800 * y) <= (800 + (512 * 800)))
				p[x + (800 * y)] = 0x000000;

			x = -sqrt((r * r) - (y - k) * (y - k)) + h;
			if (x + (800 * y) >= 0 && x + (800 * y) <= (800 + (512 * 800)))
				p[x + (800 * y)] = 0x000000;
		}
		*/

	}
	

	void Game::Tick(float dt)
	{

		// Input
		player.m_dx = 0;
		player2.m_dx = 0;

		input_handler.HandleInput(&player, &player2);
		
		for (int i = 0; i < BUTTON_COUNT; i++)
			input_handler.buttons[i].changed = false;


		// Logic


			// Circle Acceleration and Velocity
		for (Circle*& circle : p_circles)
		{
			physics.CircleMotion(circle, dt, screen);
		}

		// Collision Detection
		for (Circle*& circle : p_circles)
		{
			for (Circle*& target : p_circles)
			{
				if (circle->m_id != target->m_id)
				{
					physics.CircleCircleCollision(circle, target);
				}
			}
		}

		// Dynamic Circle Collision
		physics.CCDynamicCollision(colliding_pairs);

		if (player.m_active_attack)
		{
			for (Circle*& circle : p_circles)
				physics.WeaponCircleCollision(&player.m_weapon, circle);
		}
		if (player2.m_active_attack)
		{
			for (Circle*& circle : p_circles)
				physics.WeaponCircleCollision(&player2.m_weapon, circle);
		}


			player.Simulate(dt);
			player2.Simulate(dt);

		// Player Circle Collisions
			for (Circle*& circle : p_circles)
			{
				(physics.PlayerCircleCollision(&player, circle));
				(physics.PlayerCircleCollision(&player2, circle));
			}

	

		

		// Render


		screen->Clear(0);

		background.DrawScaled(0, 0, screen->GetWidth(), screen->GetHeight(), screen);



		for (circle_iter = p_circles.begin(); circle_iter != p_circles.end(); circle_iter++)
			DrawCircle(screen, *circle_iter);

		// Draw Cool lines to indicate colliding pairs 
		for (auto c : colliding_pairs)
		{
			Circle* c1 = c.first;
			Circle* c2 = c.second;

			screen->Line(c1->m_x, c1->m_y, c2->m_x, c2->m_y, 0xff0000);
		}
		colliding_pairs.clear();

		player.Animate(dt, screen);
		player2.Animate(dt, screen);

		Weapon* weapon = &player.m_weapon; 
		int x1 = weapon->m_x + weapon->hit_x1, x2 = weapon->m_x + weapon->hit_x2, y1 = weapon->m_y + weapon->hit_y1, y2 = weapon->m_y + weapon->hit_y2;
		screen->Box(x1, y1, x2, y2, 0x00ff00);

		weapon = &player2.m_weapon;
		x1 = weapon->m_x + weapon->hit_x1, x2 = weapon->m_x + weapon->hit_x2, y1 = weapon->m_y + weapon->hit_y1, y2 = weapon->m_y + weapon->hit_y2;
		screen->Box(x1, y1, x2, y2, 0x00ff00);
	}



};