#pragma once

#include "game.h"
#include "surface.h"



namespace Tmpl8
{

	Sprite baseball(new Surface("assets/Baseball.png"), 8);

	class Weapon
	{
	public:
		Weapon(int px = 45, int py = 45)
		{
			m_x = px - 45, m_y = py - 45;
			m_aFrame = 0;
			m_aTime = 0;
		}

		int m_x, m_y;

		Sprite m_sprite = baseball;
		float m_aFrame = 0;
		float m_aTime = 0;

	private:
		float frame_count = 8;
		float framerate = 30;

		int scale_factor = 2;

	public:
		/*
		 (x1,y1)_______(x2,y1)
			|             |
			|			  |
		 (x1,y2)-------(x2,y2)
		*/
		int swing_distance = 8 * 7;

		int hit_x1 = swing_distance + (26 * scale_factor), hit_x2 = swing_distance + (26 + 14) * scale_factor,
			hit_y1 = 20 + (30 * scale_factor), hit_y2 = 20 + (29 + 6) * scale_factor;

		float force = 15000;

		int Swing(float dt, Surface* screen);
	};

	Sprite doux_sheet(new Surface("assets/Dinos/sheets/DinoSprites - doux.png"), 24);
	Sprite doux_sheetL(new Surface("assets/Dinos/sheets/DouxL.png"), 24);
	Sprite mort_sheet(new Surface("assets/Dinos/sheets/DinoSprites - mort.png"), 24);
	Sprite mort_sheetL(new Surface("assets/Dinos/sheets/MortL.png"), 24);


	struct AnimationInfo
	{
		AnimationInfo(int start = 0, int count = 0, int rate = 0)
		{
			start_frame = start, frame_count = count, framerate = rate;
		}

		int start_frame, frame_count, framerate;
	};

	enum ANIMATIONS
	{
		IDLE,
		RUN,
		JUMP,
		HURT,
		ANIMATION_COUNT
	};


	enum DIRECTION
	{
		RIGHT,
		LEFT
	};

	class Player
	{
	public:
		Player(int x = 100, int y = 100, Sprite* sheet = &doux_sheet, Sprite* sheetL = &doux_sheetL)
		{
			m_x = x; m_dx = 0;
			m_y = y; m_aFrame = 0;
			m_aTime = 0;

			m_sheet[0] = *sheet, m_sheet[1] = *sheetL;
		}



		float m_x, m_y;
		float m_dx = 0, m_dy = 0;

		int m_scalefactor = 3;

		int m_jump_counter = 0;
		const int m_max_jumps = 3;

		Command* m_command = 0;

		void Animate(float dt, Surface* screen);
		void Simulate(float dt);

		Sprite m_sheet[2] = { doux_sheet, doux_sheetL };
		float m_aTime;
		float m_aFrame;
		int m_direction = LEFT;

		Circle* m_boundCircle = 0;

		int m_action = IDLE;
		AnimationInfo m_animation[ANIMATION_COUNT] = {
			AnimationInfo(1, 3, 5),
			AnimationInfo(4, 6, 10),
			AnimationInfo(4, 1, 0),
			AnimationInfo(15, 3, 10)
		};

		Weapon m_weapon;
		bool m_attacking = false;
		bool m_active_attack = false;
	};
}