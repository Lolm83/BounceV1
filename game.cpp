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
	Player player2(300, 300, &mort_sheet, &mort_sheetL, &mort_head);

	Sprite background(new Surface("assets/_CloudsBack.png"), 1);

	void Game::Init()
	{
		player.m_x = 150; player2.m_x = 650;
		player.m_y = 100; player2.m_y = 100;
		
		srand(time(0));

		p_circles.push_back(new Circle());
		p_circles.push_back(new Circle(600.f, 50.f, 25, 200, 0));
		p_circles.push_back(new Circle(200.f, 400.f, 80, -180, 0));
		p_circles.push_back(new Circle(26.f, 50.f, 25, 200, 0));
		p_circles.push_back(new Circle(600.f, 400.f, 80, 180, 0));

	}
	
	Mouse mouse;

	void Game::MouseDown(int button)
	{
		if (button)
			mouse.clicked = true;
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

	Sprite borders(new Surface("assets/BORDERS.png"), 2);

	void Game::DrawIngameUI(Surface* screen, Player* p1, Player* p2)
	{
		borders.SetFrame(0);
		borders.DrawScaled(0, 0,2, screen);
		
		borders.SetFrame(1);
		borders.DrawScaled(800 - 2 * borders.GetWidth(),0, 2, screen);
		
		for (int i = 0; i < p1->m_life_counter; i++)
			doux_head.DrawScaled(25 + 73 * i, 35, 4, screen);

		for (int i = 0; i < p2->m_life_counter; i++)
			mort_head.DrawScaled(800 - (225 - i * 73), 35, 4, screen);
	}

	Sprite win_sheet(new Surface("assets/Win_Sheet.png"), 2);

	float wintime = 0;

	void Game::ResetGame()
	{

		player.m_x = 150; player2.m_x = 650;
		player.m_y = 100; player2.m_y = 100;

		player.m_dx = 0; player2.m_dx = 0;
		player.m_dy = 0; player2.m_dy = 0;
	
		player.m_jump_counter = 0; player2.m_jump_counter = 0;
		player.m_boundCircle = 0; player2.m_boundCircle = 0;
		player.m_aTime = 0; player2.m_aTime = 0;
		player.m_aFrame = 0; player2.m_aFrame = 0;
		player.m_jumping = 0; player2.m_jumping = 0;
		player.m_attacking = 0; player2.m_attacking = 0;
		player.m_active_attack = 0; player2.m_active_attack = 0;
		player.m_smacked = 0; player2.m_smacked = 0;
		player.m_stun_timer = 0; player2.m_stun_timer = 0;
		player.dead = 0; player2.dead = 0;
		player.m_direction = RIGHT; player2.m_direction = LEFT;

		p_circles.clear();
		p_circles.push_back(new Circle());
		p_circles.push_back(new Circle(600.f, 50.f, 25, 200, 0));
		p_circles.push_back(new Circle(200.f, 400.f, 80, -180, 0));
		p_circles.push_back(new Circle(26.f, 50.f, 25, 200, 0));
		p_circles.push_back(new Circle(600.f, 400.f, 80, 180, 0));
	}

	void Game::Tick(float dt)
	{

	if (running)
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

		// Circle Collision Detection
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

		// Attack Hit Detection
		if (player.m_active_attack)
		{
			for (Circle*& circle : p_circles)
				physics.WeaponCircleCollision(&player.m_weapon, circle);
			physics.WeaponPlayerCollision(&player.m_weapon, &player2);
		}
		if (player2.m_active_attack)
		{
			for (Circle*& circle : p_circles)
				physics.WeaponCircleCollision(&player2.m_weapon, circle);
			physics.WeaponPlayerCollision(&player2.m_weapon, &player);
		}

		// Move the players
		if (!player.dead)
			player.Simulate(dt, screen);
		if (!player2.dead)
			player2.Simulate(dt, screen);

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

		DrawIngameUI(screen, &player, &player2);

		if (!running)
			ResetGame();

		/*
		Weapon* weapon = &player.m_weapon;
		int x1 = weapon->m_x + weapon->hit_x1, x2 = weapon->m_x + weapon->hit_x2, y1 = weapon->m_y + weapon->hit_y1, y2 = weapon->m_y + weapon->hit_y2;
		screen->Box(x1, y1, x2, y2, 0x00ff00);

		weapon = &player.m_weapon;
		x1 -= 90, x2 -= 90;
		screen->Box(x1, y1, x2, y2, 0x00ff00);

		screen->Box(player.m_x + 12, player.m_y + 12, player.m_x + 3 * player.m_sheet[0].GetWidth() - 21, player.m_y + 3 * player.m_sheet[0].GetHeight() - 12, 0x00ff00);
		screen->Box(player2.m_x + 15, player2.m_y + 12, player2.m_x + 3 * player2.m_sheet[0].GetWidth() - 18, player2.m_y + 3 * player2.m_sheet[0].GetHeight() - 12, 0xffff00);
		*/
	}
	else
	{
		// Get Mouse Position
		SDL_GetRelativeMouseState(&(mouse.dx), &(mouse.dy));

		mouse.x += mouse.dx;
		mouse.y += mouse.dy;

		// Render Menu
		screen->Clear(0x949291);

		if (player.m_life_counter == -1 || player2.m_life_counter == -1)
		{
			if (wintime <= 4)
			{
				wintime += dt / 1000;
				if (player.m_life_counter == -1)
				{
					win_sheet.SetFrame(0);
					win_sheet.Draw(screen, 0, 0);
				}
				else if (player2.m_life_counter == -1)
				{
					win_sheet.SetFrame(1);
					win_sheet.Draw(screen, 0, 0);
				}
			}
			else
				player.m_life_counter = 3, player2.m_life_counter = 3, wintime = 0;
		}
		else
		{
			if (mouse.x >= 213 && mouse.x <= 213 + borders.GetWidth() * 3 &&
				mouse.y >= 330 && mouse.y <= 330 + borders.GetHeight() * 3)
			{
				borders.SetFrame(0);
				if (mouse.clicked)
					running = true;
			}
			else
				borders.SetFrame(0);
			borders.DrawScaled(210 + 3, 330, 3, screen);

			screen->Print("Start Game", 248, 395, 0xffffff, 5);

			borders.SetFrame(1);
			borders.DrawScaled(90, 44, 5, screen);
			screen->Print("BUBBLE", 248, 120, 0xffffff, 9);
			screen->Print("BATTLE", 248, 180, 0xffffff, 9);

			mouse.clicked = false;
		}
	}
	}



};