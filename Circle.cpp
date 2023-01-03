#include "game.h"
#include "surface.h"
#include <ctime>
#include "Circle.h"

namespace Tmpl8
{
	// Global Variables

	int Circle::s_Total = 0;

	Circle::Circle(float x, float y, float r, float dx, float dy) :
		m_x(x), m_y(y), m_radius(r), m_dx(dx), m_dy(dy)
	{
		++s_Total;
		m_id = s_Total; // Doesn't take into account removing circles then adding more. Fix as needed.
	}

	void Circle::Shuffle()
	{
		m_dx = (rand() % 240) - 120;
		m_dy = 0;
		m_radius = rand() % 80 + 40;
		m_mass = m_radius * m_radius * 3.14f;
		
		if (m_x <= m_radius)
			m_x = m_radius + 1;
		if (m_x >= 800 - m_radius)
			m_x = 800 - m_radius - 1;

		int sp = (rand() % 100) + 1;
		if (sp <= 90)
			m_color = rand() % 4;
		else if (sp > 90 && sp <= 99)
			m_color = PINK;
		else if (sp > 99 && sp <= 100)
			m_color = PINK;

	}

};