#include "game.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <SDL.h>
#include "Player.h"

#define u32 unsigned int

namespace Tmpl8
{
	

	// The architecture of this is straight from Programming Patterns by Robert Nystrom

	class Player;

	class Command
	{
	public:
		virtual ~Command() {}; // My first time using destructors
		virtual bool Execute(Circle* circle) = 0; // And my first real try at inheritance outside of Michael Dawson
		virtual bool Execute(Player* player) = 0;
	};

	// I completely didn't need any of this.

	class Mouse
	{
	public:
		int x = 0, y = 0;
		int dx = 0, dy = 0;
		bool clicked = false;
	};
	Mouse cursor;

	class DragShootCommand : public Command
	{
	public:
		DragShootCommand() {};
		virtual bool Execute(Circle* circle, float dt);
		virtual bool Execute(Player* player, float dt) { return 0; };
		bool m_held_down = false;
		Mouse* mouse = &cursor;
	};
	bool DragShootCommand::Execute(Circle* circle, float dt)
	{
		SDL_GetRelativeMouseState(&(mouse->dx), &(mouse->dy));

		mouse->x += mouse->dx;
		mouse->y += mouse->dy;

		// These are square-shaped click hitboxes... Fix that.
		if (mouse->y >= (circle->m_y - circle->m_radius) && mouse->y <= (circle->m_y + circle->m_radius))
			if (mouse->x >= (circle->m_x - circle->m_radius) && mouse->x <= (circle->m_x + circle->m_radius))
			{
				circle->m_x = mouse->x;
				circle->m_y = mouse->y;

				return true;
			}
			else
			{
				return false;
			}
		else
		{
			return false;
		}

	}

	class DragMoveCommand : public Command
	{
	public:
		DragMoveCommand() {};
		virtual bool Execute(Circle* circle, float dt);
		virtual bool Execute(Player* player, float dt) { return 0; };
		bool m_held_down;
		Mouse* mouse = &cursor;
	};
	bool DragMoveCommand::Execute(Circle* circle, float dt)
	{
		SDL_GetRelativeMouseState(&(mouse->dx), &(mouse->dy));

		mouse->x += mouse->dx;
		mouse->y += mouse->dy;
		
		// These are square-shaped click hitboxes... Fix that.
		if (mouse->y >= (circle->m_y - circle->m_radius) && mouse->y <= (circle->m_y + circle->m_radius))
			if (mouse->x >= (circle->m_x - circle->m_radius) && mouse->x <= (circle->m_x + circle->m_radius))
			{
				circle->m_x = mouse->x;
				circle->m_y = mouse->y;

				return true;
			}
			else
				return false;
		else
			return false;
	}
	

	class JumpCommand : public Command {
	public:
		virtual bool Execute(Circle* circle) { return 0; };
		virtual bool Execute(Player* player);
	};
	bool JumpCommand::Execute(Player* player)
	{
		player->m_x -= 50;

		return true;
	}

	class UpCommand : public Command {
	public:
		virtual bool Execute(Player* player ) ;
		virtual bool Execute(Circle* circle ) { return 0; };
	};
	bool UpCommand::Execute(Player* player )
	{
		if(!player->dead)
			if (player->m_jump_counter < player->m_max_jumps)
			{
				player->m_action = JUMP;
				player->m_dy = -480;

				if (player->m_boundCircle)
					player->m_boundCircle = 0;

				player->m_jump_counter++;
			}
		
		return true;
	}

	class DownCommand : public Command {
	public:
		virtual bool Execute(Player* player );
		virtual bool Execute(Circle* circle ) { return 0; };
	};
	bool DownCommand::Execute(Player* player )
	{
		if (!player->m_boundCircle)
			player->m_dy = 80;

		return true;
	}

	class LeftCommand : public Command {
	public:
		virtual bool Execute(Player* player );
		virtual bool Execute(Circle* circle ) { return 0; };
	};
	bool LeftCommand::Execute(Player* player )
	{
		if (!player->dead)
		{
			player->m_dx -= 200;
			player->m_direction = LEFT;

			if (player->m_action != JUMP)
				player->m_action = RUN;
		}
			return true;
	}

	class RightCommand : public Command {
	public:
		virtual bool Execute(Player* player );
		virtual bool Execute(Circle* circle ) { return 0; };
	};
	bool RightCommand::Execute(Player* player )
	{
		if (!player->dead)
		{
			player->m_dx += 200;
			player->m_direction = RIGHT;

			if (player->m_action != JUMP)
				player->m_action = RUN;
		}
			return true;		
	}
	
	struct ButtonState {
		bool is_down = false;
		bool changed = false;
	};

	enum Buttons
	{
		BUTTON_W,
		BUTTON_A,
		BUTTON_S,
		BUTTON_D,
		BUTTON_E,

		BUTTON_UP,
		BUTTON_DOWN,
		BUTTON_LEFT,
		BUTTON_RIGHT,
		BUTTON_SPACE,

		BUTTON_COUNT
	};

	class InputHandler
	{
	public:
		InputHandler() {
		//	Leftclick = new DragShootCommand;
		//	Rightclick = new DragMoveCommand;

			Up = new UpCommand;
			Left = new LeftCommand;
			Down = new DownCommand;
			Right = new RightCommand;
		};
		void HandleInput(Player* p1, Player* p2 );

		Command* Up;
		Command* Left;
		Command* Down;
		Command* Right;

		ButtonState buttons[BUTTON_COUNT];

	//	DragShootCommand* Leftclick;
	//	DragMoveCommand* Rightclick;
		
		
		
	};

	InputHandler input_handler;
	bool jumping1 = false;
	bool jumping2 = false; 


	void Game::KeyUp(int key)
	{
#define ProcessKeyUp(b, SCANCODE_B)						  \
		case SCANCODE_B:								   \
			input_handler.buttons[b].is_down = false;       \
			input_handler.buttons[b].changed = true; break;  \



		switch (key) {
			
		case SDL_SCANCODE_W:	
			jumping1 = false;
			input_handler.buttons[BUTTON_W].is_down = false;       
			input_handler.buttons[BUTTON_W].changed = true; break;
		case SDL_SCANCODE_UP:
			jumping2 = false;
			input_handler.buttons[BUTTON_UP].is_down = false;
			input_handler.buttons[BUTTON_UP].changed = true; break;

			ProcessKeyUp(BUTTON_A, SDL_SCANCODE_A);
			ProcessKeyUp(BUTTON_D, SDL_SCANCODE_D);
			ProcessKeyUp(BUTTON_E, SDL_SCANCODE_E);

			ProcessKeyUp(BUTTON_LEFT, SDL_SCANCODE_LEFT);
			ProcessKeyUp(BUTTON_RIGHT, SDL_SCANCODE_RIGHT);
			ProcessKeyUp(BUTTON_SPACE, SDL_SCANCODE_SPACE);
		}

	}

	void Game::KeyDown(int key)
	{
#define ProcessKeyDown(b, SCANCODE_B)					 \
		 case SCANCODE_B:								  \
			input_handler.buttons[b].is_down = true;   	   \
			input_handler.buttons[b].changed = true; break; \

		switch(key) {

		case SDL_SCANCODE_W:
			if (!jumping1)
			{
				input_handler.buttons[BUTTON_W].is_down = true;
				input_handler.buttons[BUTTON_W].changed = true;
				jumping1 = true;
			}
			break;
		case SDL_SCANCODE_UP:
			if (!jumping2)
			{
				input_handler.buttons[BUTTON_UP].is_down = true;
				input_handler.buttons[BUTTON_UP].changed = true;
				jumping2 = true;
			}
			break;

			ProcessKeyDown(BUTTON_A, SDL_SCANCODE_A);
			ProcessKeyDown(BUTTON_D, SDL_SCANCODE_D);
			ProcessKeyDown(BUTTON_E, SDL_SCANCODE_E);

			ProcessKeyDown(BUTTON_LEFT, SDL_SCANCODE_LEFT);
			ProcessKeyDown(BUTTON_RIGHT, SDL_SCANCODE_RIGHT);
			ProcessKeyDown(BUTTON_SPACE, SDL_SCANCODE_SPACE);
			}
	}



	void InputHandler::HandleInput(Player* p1, Player* p2 )
	{
#define IsDown(b) buttons[b].is_down
#define Pressed(b) (buttons[b].is_down && buttons[b].changed)
#define Released(b) (!buttons[b].is_down && buttons[b].changed)

		bool input1 = false;
		bool input2 = false;

		if (Pressed(BUTTON_W))
		{
			Up->Execute(p1);
		}
		if (IsDown(BUTTON_A))
		{
			Left->Execute(p1);
			
		}
		if (IsDown(BUTTON_D))
		{
			Right->Execute(p1);
		}
		if (Pressed(BUTTON_E))
		{
			p1->m_attacking = true;
		}
		else
		{
			for (int i = 0; i < BUTTON_COUNT / 2; i++)
				if (IsDown(i))
					input1 = true;
			if (!input1)
				p1->m_action = IDLE;
		}


		if (Pressed(BUTTON_UP))
		{
			Up->Execute(p2);
		}
		if (IsDown(BUTTON_LEFT))
		{
			Left->Execute(p2);
		}
		if (IsDown(BUTTON_RIGHT))
		{
			Right->Execute(p2);
		}
		if (Pressed(BUTTON_SPACE))
		{
			p2->m_attacking = true;
		}
		// micro-optimization with the else :)
		else
		{
			for (int i = BUTTON_COUNT / 2; i < BUTTON_COUNT; i++)
				if (IsDown(i))
					input2 = true;
			if (!input2)
				p2->m_action = IDLE;
		}


	}

};