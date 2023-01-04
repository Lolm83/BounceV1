#pragma once
#include "Circle.h"
#include "game.h"
#include "surface.h"



namespace Tmpl8
{
	enum ANIMATIONS
	{
		IDLE,
		RUN,
		JUMP,
		SMACKED,
		DEATH,
		ANIMATION_COUNT
	};

	enum DIRECTION
	{
		RIGHT,
		LEFT,
		DOWN
	};

	Sprite baseball(new Surface("assets/Baseball.png"), 8);
	Sprite baseballL(new Surface("assets/BaseballL.png"), 8);

	class Weapon
	{
	public:
		Weapon(int px = 45, int py = 45, int direction = LEFT)
		{
			m_x = px - 45, m_y = py - 45;
			m_aFrame = 0;
			m_aTime = 0;
			m_direction = direction;
		}

		int m_x, m_y;

		Sprite m_sprite[2] = { baseball, baseballL};
		float m_aFrame = 0;
		float m_aTime = 0;
		int m_direction = LEFT;

		int scale_factor = 2;

	private:
		float frame_count = 8;
		float framerate = 40;

		

	public:
		/*
		 (x1,y1)_______(x2,y1)
			|             |
			|			  |
		 (x1,y2)-------(x2,y2)
		*/
		int swing_distance = 8 * 7;

		int hit_x1 = swing_distance + (26 * scale_factor), hit_x2 = swing_distance + (26 + 24) * scale_factor,
			hit_y1 = 20 + (30 * scale_factor), hit_y2 = 20 + (29 + 6) * scale_factor;

		float force = 15000;

		int Swing(float dt, Surface* screen);
	};

	Sprite doux_head(new Surface("assets/Dinos/Doux_Head.png"), 1);
	Sprite mort_head(new Surface("assets/Dinos/Mort_Head.png"), 1);

	Sprite doux_sheet(new Surface("assets/Dinos/sheets/DinoSprites - doux.png"), 24);
	Sprite doux_sheetL(new Surface("assets/Dinos/sheets/DouxL.png"), 24);
	Sprite mort_sheet(new Surface("assets/Dinos/sheets/DinoSprites - mort.png"), 24);
	Sprite mort_sheetL(new Surface("assets/Dinos/sheets/MortL.png"), 24);
	
	Sprite kaboom(new Surface("assets/explosion-4.png"), 12);

	struct AnimationInfo
	{
		AnimationInfo(int start = 0, int count = 0, int rate = 0)
		{
			start_frame = start, frame_count = count, framerate = rate;
		}

		int start_frame, frame_count, framerate;
	};

	class Player
	{
	public:
		Player(int x = 100, int y = 100, Sprite* sheet = &doux_sheet, Sprite* sheetL = &doux_sheetL, Sprite* head = &doux_head)
		{
			m_x = x; m_dx = 0;
			m_y = y; m_aFrame = 0;
			m_aTime = 0;

			p_total++;
			player_id = p_total;

			m_sheet[0] = *sheet, m_sheet[1] = *sheetL, m_sheet[2] = *head;
		}

		float m_x, m_y;
		float m_dx = 0, m_dy = 0;

		int m_scalefactor = 3;

		int p_height = 17 * m_scalefactor, p_width = 10 * m_scalefactor;

		int ybottom = 3 * m_scalefactor;
		int ytop = 4 * m_scalefactor;	   int ytop2 = 12 * m_scalefactor;
		int xleft1 = 4 * m_scalefactor;    int xleft2 = 6 * m_scalefactor;
		int xright1 = 4 * m_scalefactor;   int xright2 = 8 * m_scalefactor;

		int m_jump_counter = 0;
		const int m_max_jumps = 3;

		int m_life_counter = 3;

		Command* m_command = 0;

		void Animate(float dt, Surface* screen);
		void Simulate(float dt, Surface* screen);
		void Die(float dt, Surface* screen, int direction);

		Sprite m_sheet[3] = { doux_sheet, doux_sheetL, doux_head };
		
		float m_aTime;
		float m_aFrame;
		int m_direction = LEFT;

		Circle* m_boundCircle = 0;

		int m_action = IDLE;
		AnimationInfo m_animation[ANIMATION_COUNT] = {
			AnimationInfo(1, 3, 5),  // Idle
			AnimationInfo(4, 6, 10), // Run
			AnimationInfo(4, 1, 0),  // Jump
			AnimationInfo(14, 3, 10) // Smacked
		};
		bool m_jumping = false;

		Weapon m_weapon;
		bool m_attacking = false;
		bool m_active_attack = false;

		bool m_smacked = false;
		float m_stun_timer = 0;
		int m_smackspeed = 0;

		bool dead = false;
		Sprite m_explosion = kaboom;
		void BlowUp(float dt, Surface* screen);

	private:
		int player_id;
		static int p_total;
	};

	int Player::p_total = 0;
}