#include "game.h"
#include "surface.h"
#include "player.h"
#include "Circle.h"

// Rewritten from Jdavidx9's youtube video: Programming Balls #1 Circle Vs Circle Collisions

namespace Tmpl8
{
	bool running = false;

	void Player::Animate(float dt, Surface* screen)
	{
	


	if (!dead)
	{
		if (m_smacked)
			m_action = SMACKED;

		int start_frame = m_animation[m_action].start_frame;
		float frame_count = m_animation[m_action].frame_count;
		float framerate = m_animation[m_action].framerate;

		m_aTime += (dt / 1000);

		if (m_aTime > (frame_count / framerate))
			m_aTime = 0;

		if (!m_direction)
			m_aFrame = start_frame + m_aTime * framerate;
		else
			m_aFrame = 24 - (start_frame + m_aTime * framerate);





		m_sheet[0 + m_direction].SetFrame(m_aFrame);
		m_sheet[0 + m_direction].DrawScaled(m_x, m_y, m_scalefactor, screen);

		if (m_attacking)
		{
			if (m_direction != m_weapon.m_direction)
				m_weapon.m_direction = m_direction;
			int frame = m_weapon.Swing(dt, screen);
			if (frame == 0)
				m_attacking = false;
			else if (frame == 8)
				m_active_attack = true;
		}
	}
	else
	{
		BlowUp(dt, screen);
	}
		
	}

	const float gravity = 900.81f;

	void Player::Simulate(float dt, Surface* screen)
	{
		dt = dt / 1000;
	
		if (m_smacked)
		{
			m_stun_timer += dt;
			m_dx += m_smackspeed * (0.4 - m_stun_timer);
			if (m_stun_timer >= 0.4)
			{
				m_smacked = false;
				m_stun_timer = 0;
			}
		}
		

		m_y += m_dy * dt;
		m_x += m_dx * dt;

		if (m_boundCircle && m_y + ytop + p_height < screen->GetHeight() )
		{
			m_dy = 0;

			m_x += m_boundCircle->m_dx * dt;
			m_y += m_boundCircle->m_dy * dt;
		}
		else
			m_dy += gravity * dt;


		// Walk along a circumference
		if (m_boundCircle)
		{

			int x = 0;							 // Right Foot
			if (m_x + xleft2 + p_width >= m_boundCircle->m_x - m_boundCircle->m_radius &&
				m_x + xleft2 + p_width <= m_boundCircle->m_x)
			{
				x = m_x + xleft2 + p_width;
			}								 // Flat Across the Middle
			else if (m_x + xleft2 + p_width > m_boundCircle->m_x &&
				m_x + xleft2 <= m_boundCircle->m_x)
			{
			}
			// Left Foot
			else if (m_x + xleft2 > m_boundCircle->m_x &&
				m_x <= m_boundCircle->m_x + m_boundCircle->m_radius - xleft2)
			{
				x = m_x + xleft2;
			}
			else
				m_boundCircle = 0;

			if (x != 0)
			{
				x -= m_boundCircle->m_x;
				//if (x >= player->m_boundCircle->m_x - player->m_boundCircle->m_radius &&
					//x <= player->m_boundCircle->m_x + player->m_boundCircle->m_radius)
				{
					// int y = sqrt(r * r - x * x) + k;
					int y = -sqrt((m_boundCircle->m_radius * m_boundCircle->m_radius) - (x * x)) + m_boundCircle->m_y;

					m_y = y - p_height - ybottom;
				}
			}
		}

		if (m_boundCircle)
			if (m_y <= -m_boundCircle->m_radius)
				Die(dt, screen, DOWN);

		if (m_y >= screen->GetHeight())
			Die(dt, screen, DOWN);

		if (m_x + xright2 >= screen->GetWidth())
			Die(dt, screen, RIGHT);

		if (m_x + xleft2 <= 0)
			Die(dt, screen, LEFT);

		if (m_direction == RIGHT)
			m_weapon.m_x = m_x - 45;
		else
			m_weapon.m_x = m_x - 45;
			m_weapon.m_y = m_y - 45;

		m_active_attack = false;

		if (m_action == JUMP && m_boundCircle)
			m_action == IDLE;
	}
	
	int lazy_failsafe = -1;

	// The idea here is to animate the bat, and then return 'true' during the active frames for the attack input logic
	int Weapon::Swing(float dt, Surface* screen)
	{

		m_aTime += (dt / 1000);

		if (m_aTime > ((frame_count + 4) / framerate))
		{
			m_aTime = 0;
			lazy_failsafe = -1;
			return 0;
		}

		m_aFrame = m_aTime * framerate;

		if (!m_direction)
			m_aFrame = m_aTime * framerate;
		else
			m_aFrame = 8 - (m_aTime * framerate);

		if ((m_aTime * framerate) > 8 && (m_aTime * framerate) < 14)
		{
			if (m_direction == RIGHT)
			{
				m_sprite[0 + m_direction].SetFrame(7);
				m_sprite[0 + m_direction].DrawScaled(m_x + 8 * 7, m_y, 2, screen);
			}
			else 
			{
				m_sprite[0 + m_direction].SetFrame(0);
				m_sprite[0 + m_direction].DrawScaled(m_x, m_y, 2, screen);
			}
			
			
			lazy_failsafe++;
			if (lazy_failsafe == 0)
				return 8;
			
		}
		else if ((m_aTime * framerate) >= 1 && (m_aTime * framerate) < 8)
		{
			m_sprite[0 + m_direction].SetFrame(m_aFrame);
			m_sprite[0 + m_direction].DrawScaled(m_x + 8 * m_aFrame, m_y , 2, screen);
		}
		return 1;
	}

	void Player::Die(float dt, Surface* screen, int direction)
	{
		dead = true;
		m_boundCircle = 0;
		printf("you died");
		m_aTime = 0;
		m_dy = 0;
		m_dx = 0;
		m_stun_timer = 0;
		m_smacked = false;
		m_jump_counter = 0;
		m_direction = direction;
	}

	void Player::BlowUp(float dt, Surface* screen)
	{
		

		int start_frame = 0;
		float frame_count = 12;
		float framerate = 15;

		

		m_aFrame = start_frame + m_aTime * framerate;

		m_explosion.SetFrame(m_aFrame);

		if (m_direction == DOWN)
			m_explosion.DrawRotated(m_x, 512 - m_explosion.GetHeight(), 1, screen, DOWN);
		else if (m_direction == RIGHT)
			m_explosion.DrawRotated(800 - m_explosion.GetWidth(), m_y + 100, 1, screen, RIGHT);
		else if (m_direction == LEFT)
			m_explosion.DrawRotated(0, m_y - 20, 1, screen, LEFT);
		
		m_aTime += (dt / 1000);

		if (m_aTime > (frame_count / framerate))
		{
			m_aTime = 0;
			m_life_counter--;
			if (m_life_counter >= 0)
			{
				dead = false;
				
					m_x = 150;
					m_y = 100;

				m_boundCircle = 0;
				m_direction = RIGHT;
			}
			else if (m_life_counter < 0)
			{
				running = false;
			}
		}

		m_boundCircle = 0;
	}
}