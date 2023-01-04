#include "game.h"
#include "surface.h"
#include "Player.h"

// Rewritten from Jdavidx9's youtube video: Programming Balls #1 Circle Vs Circle Collisions

namespace Tmpl8
{
	std::vector<std::pair<Circle*, Circle*>> colliding_pairs;

			// Collision Detection and Instantaneous Collision Resolution

	void Physics::CircleMotion(Circle* circle, float dt, Surface* screen)
	{
		dt = dt / 1000; 
		int terminal_dy = 200;
		int gravity = 100;
		float friction = 0;

		// The classic
		/*
		circle->m_ddx = -circle->m_dx * friction * circle->m_radius;
		circle->m_ddy = -circle->m_dy * friction * circle->m_radius;
		*/
		circle->m_dx += circle->m_ddx * dt;
		circle->m_dy += circle->m_ddy * dt + gravity * dt;
		circle->m_x += circle->m_dx * dt;
		
			if (circle->m_dy > terminal_dy - circle->m_radius)
				circle->m_dy = terminal_dy - circle->m_radius;
				
		circle->m_y += circle->m_dy * dt;

		// If velocity is negligible, it becomes zero.
		if (fabs(circle->m_dy * circle->m_dy + circle->m_dx * circle->m_dx) <= 0.01f)
		{
			circle->m_dx = 0;
			circle->m_dy = 0;
		}

		// Edge of the screen stuff
		if (circle->m_x < 0 + circle->m_radius)
		{
			circle->m_dx = -circle->m_dx;
		}
		if (circle->m_x > screen->GetWidth() - circle->m_radius) 
		{
			circle->m_dx = -circle->m_dx;
		}
		if (circle->m_y < 0 - circle->m_radius) circle->m_y = screen->GetHeight() + circle->m_radius;
		if (circle->m_y > screen->GetHeight() + circle->m_radius) 
		{
			circle->Shuffle();
			circle->m_y = 0 - circle->m_radius;
		}

	}

	void Physics::CircleCircleCollision(Circle* circle, Circle* target)
	{
		// If the distance between the centers is less than the sum of the radiuses, they are colliding
		
		float x1 = circle->m_x, y1 = circle->m_y, r1 = circle->m_radius;
		float x2 = target->m_x, y2 = target->m_y, r2 = target->m_radius;

		bool colliding = (fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (r1 + r2) * (r1 + r2));

		if (colliding)
		{
			// Distance between centers
			float distance = sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

			float overlap = 0.5f * (distance - r1 - r2);

			// Displace Circles so they aren't colliding anymore
			circle->m_x -= overlap * (x1 - x2) / distance;
			circle->m_y -= overlap * (y1 - y2) / distance;

			target->m_x += overlap * (x1 - x2) / distance;
			target->m_y += overlap * (y1 - y2) / distance;

			colliding_pairs.push_back({ circle, target });
		}
	}

	void Physics::CircleLineCollision(Circle* circle, Line* line)
	{

	}

	bool Physics::PlayerCircleCollision(Player* player, Circle* circle)
	{
		float x2 = 0, y2 = 0;
		
		// Right Foot
		if (player->m_x + player->xleft2 + player->p_width >= circle->m_x - circle->m_radius &&
			player->m_x <= circle->m_x - player->xleft2)
			x2 = player->m_x + player->xleft2 + player->p_width, y2 = player->m_y + player->p_height + player->ytop;

		// Left Foot
		else if (player->m_x + player->xleft2 > circle->m_x &&
			player->m_x <= circle->m_x + circle->m_radius - player->xleft2)
			x2 = player->m_x + player->xleft2, y2 = player->m_y + player->p_height + player->ytop;
		else
			return false;

		float x1 = circle->m_x, y1 = circle->m_y, r1 = circle->m_radius;

		if (player->m_y + player->ytop >= y1)
		{
			y2 = player->m_y + player->ytop;
			if (player->m_x > circle->m_x)
				x2 = player->m_x + player->xleft2;
			else
				x2 = player->m_x + player->xleft2 + player->p_width;
		}

		bool colliding = (fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= r1 * r1);
		
		if (colliding)
		{
			if (y2 <= y1)
			{
				player->m_boundCircle = circle;
				if (player->m_action == JUMP)
					player->m_action = IDLE;
				player->m_jump_counter = 0;
				player->m_dy = 0;
				return true;
			}

			// Distance between center and Dino's foot
			float distance = sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

			float overlap = (distance - r1);

			// Displace player so they aren't colliding anymore
			player->m_x += overlap * (x1 - x2) / distance;
			player->m_y += overlap * (y1 - y2) / distance;
		}	

		return false;
	}

			// Dynamic Collision Simulation

	void Physics::CCDynamicCollision(std::vector<std::pair<Circle*, Circle*>> colliding_pair)
	{
		for (auto c : colliding_pair)
		{
			Circle* c1 = c.first;
			Circle* c2 = c.second;

			float x1 = c1->m_x, y1 = c1->m_y, r1 = c1->m_radius;
			float x2 = c2->m_x, y2 = c2->m_y, r2 = c2->m_radius;

			float fDistance = sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

			// Normal to the Collision
			float nx = (x1 - x2) / fDistance;
			float ny = (y1 - y2) / fDistance;

			// Tangent
			float tx = -ny;
			float ty = nx;

			// Dot Product Tangent
			float dpTan1 = c1->m_dx * tx + c1->m_dy * ty;
			float dpTan2 = c2->m_dx * tx + c2->m_dy * ty;

			// Dot Product Normal
			float dpNorm1 = c1->m_dx * nx + c1->m_dy * ny;
			float dpNorm2 = c2->m_dx * nx + c2->m_dy * ny;

			// Conservation of momentum in 1D
			float m1 = (dpNorm1 * (c1->m_mass - c2->m_mass) + 2.0f * c2->m_mass * dpNorm2) / (c1->m_mass + c2->m_mass);
			float m2 = (dpNorm2 * (c2->m_mass - c1->m_mass) + 2.0f * c1->m_mass * dpNorm1) / (c1->m_mass + c2->m_mass);

			c1->m_dx = tx * dpTan1 + m1 * nx;
			c1->m_dy = ty * dpTan1 + m1 * ny;
			c2->m_dx = tx * dpTan2 + m2 * nx;
			c2->m_dy = ty * dpTan2 + m2 * ny;
		}
	}
	
	void Physics::WeaponCircleCollision(Weapon* weapon, Circle* circle)
	{
		int x1 = weapon->m_x + weapon->hit_x1, x2 = weapon->m_x + weapon->hit_x2, y1 = weapon->m_y + weapon->hit_y1, y2 = weapon->m_y + weapon->hit_y2;
		int r1 = circle->m_radius, xc = circle->m_x, yc = circle->m_y;

		if (weapon->m_direction == LEFT)
			x1 -= 2 * weapon->swing_distance, x2 -= 2 * weapon->swing_distance;

		bool colliding = (fabs((x1 - xc) * (x1 - xc) + (y1 - yc) * (y1 - yc)) <= r1 * r1 || fabs((x2 - xc) * (x2 - xc) + (y1 - yc) * (y1 - yc)) <= r1 * r1 
						  || fabs((x1 - xc) * (x1 - xc) + (y2 - yc) * (y2 - yc)) <= r1 * r1 || fabs((x2 - xc) * (x2 - xc) + (y2 - yc) * (y2 - yc)) <= r1 * r1);

		if (colliding)
		{
			if (weapon->m_direction == RIGHT)
				circle->m_dx += weapon->force / circle->m_radius + 20;
			else 
				circle->m_dx -= weapon->force / circle->m_radius + 20;
			
				

			if (circle->m_color == PINK)
			{
				circle->m_color = BLACK;
				circle->m_dx *= 2;
			}
		}

		
	}

	void Physics::WeaponPlayerCollision(Weapon* weapon, Player* player) 
	{
		int x1 = weapon->m_x + weapon->hit_x1, x2 = weapon->m_x + weapon->hit_x2, y1 = weapon->m_y + weapon->hit_y1, y2 = weapon->m_y + weapon->hit_y2;

		if (weapon->m_direction == LEFT)
			x1 -= 2 * weapon->swing_distance + 20, x2 -= 2 * weapon->swing_distance + 20;
		
		int px1 = player->m_x + player->xleft1, px2 = player->m_x + player->m_sheet[0].GetWidth() * player->m_scalefactor - player->xright1,
			py1 = player->m_y + player->ytop, py2 = player->m_y + player->m_sheet[0].GetHeight() * player->m_scalefactor - player->ybottom;

		if ((x1 < px2 && x2 > px1 && y1 < py2 && y2 > py1))
		{
			if (weapon->m_direction == RIGHT)
				player->m_smackspeed = weapon->force / 4;
			else
				player->m_smackspeed = -weapon->force / 4;
			player->m_smacked = true;
		}
	}

};