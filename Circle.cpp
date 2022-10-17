#include "game.h"
#include <ctime>

namespace Tmpl8
{
	class Circle
	{
	public:
		Circle(float x = 400, float y = 200, float r = 100);
	
		float m_x, m_y;
		float m_dx = 0, m_dy = 0;
		float m_ddx = 0, m_ddy = 0;
		float m_radius;

		static int s_Total;
		int m_id = 0;
	};
	int Circle::s_Total = 0;

	Circle::Circle(float x, float y, float r) :
		m_x(x), m_y(y), m_radius(r)
	{
		++s_Total;
		m_id = s_Total; // Doesn't take into account removing circles then adding more. Fix as needed.
	}

};