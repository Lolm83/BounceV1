#include "game.h"
#include "surface.h"
#include "player.h"

// Rewritten from Jdavidx9's youtube video: Programming Balls #1 Circle Vs Circle Collisions

namespace Tmpl8
{
	

	void Player::Animate(float dt, Surface* screen)
	{
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
			int frame = m_weapon.Swing(dt, screen);
			if (frame == 0)
				m_attacking = false;
			else if (frame == 8)
				m_active_attack = true;
		}
				
		
		
	}

	const float gravity = 900.81f;

	void Player::Simulate(float dt)
	{
		dt = dt / 1000;
		
		m_y += m_dy * dt;
		m_x += m_dx * dt;

		

		if (m_boundCircle)
		{
			m_dy = 0;

			m_x += m_boundCircle->m_dx * dt ;
			m_y += m_boundCircle->m_dy * dt ;
		}
		else
			m_dy += gravity * dt;

		// fun variables
		int p_height = 17 * m_scalefactor, p_width = 10 * m_scalefactor;

					int ybottom = 3 * m_scalefactor;
					int ytop = 4 * m_scalefactor;
					int xleft2 = 6 * m_scalefactor;
					int xright2 = 8 * m_scalefactor;
		
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

		if (m_y + p_height + ytop >= 512)
		{
			m_boundCircle = 0;
			m_y = 512 - p_height - ytop;
		}

		
		m_weapon.m_x = m_x - 45; m_weapon.m_y = m_y - 45;
		m_active_attack = false;
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

		if (m_aFrame > 8 && m_aFrame < 12)
		{
			baseball.SetFrame(7);
			baseball.DrawScaled(m_x + 8 * 7, m_y, 2, screen);
			
			lazy_failsafe++;
			if (lazy_failsafe == 0)
				return 8;
			
		}
		else if (m_aFrame >= 1 && m_aFrame < 8)
		{
			baseball.SetFrame(m_aFrame);
			baseball.DrawScaled(m_x + 8 * m_aFrame, m_y , 2, screen);
		}
		return 1;
	}

	
}