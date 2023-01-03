#pragma once

namespace Tmpl8 {

	Physics physics;
	std::vector<Circle*> p_circles;

	Sprite red(new Surface("assets/Bubble_1.png"), 1);
	Sprite green(new Surface("assets/Bubble_2.png"), 1);
	Sprite yellow(new Surface("assets/Bubble_4.png"), 1);
	Sprite blue(new Surface("assets/Bubble_5.png"), 1);
	Sprite pink(new Surface("assets/Bubble_3.png"), 1);
	Sprite black(new Surface("assets/Bubble_6.png"), 1);

	enum COLOR
	{
		RED,
		GREEN,
		YELLOW,
		BLUE,
		PINK,
		BLACK,

		COLOR_COUNT
	};

	class Circle
	{
	public:
		Circle(float x = 400, float y = 200, float r = 100, float dx = 0, float dy = 0);

		float m_x, m_y;
		float m_dx = 0, m_dy = 0;
		float m_ddx = 0, m_ddy = 10;
		float m_radius;
		float m_mass = m_radius * m_radius * 3.14f;

		Sprite m_sprites[COLOR_COUNT]{ red, green, yellow, blue, pink, black };
		int m_color = RED;

		static int s_Total;
		int m_id = 0;

		void Shuffle();
	};
};