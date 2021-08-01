#include "graphical.h"

enum input_type {	
					NONE = 0,
					EXIT = -1,
						
					SELECT = -2,
					CANCEL = -3,
					
					UP = 1,
					DOWN = 2,
					LEFT = 3,	
					RIGHT = 4,
					
					DOWN_LEFT = 5, // DOWN + LEFT 
					DOWN_RIGHT = 6, // DOWN + RIGHT 
					
					UP_LEFT = 7, // UP + LEFT + 3
					UP_RIGHT = 8, // UP + RIGHT + 3 
				};

class input_handler // handles inputs
{
	public:
		input_handler() // constructor
		{
			e = new SDL_Event;
		}
		
		void update() // polls events 
		{
			input_type x = NONE; 
			
			const Uint8* keyStates = SDL_GetKeyboardState(NULL);
			while(SDL_PollEvent(e) != 0) // grabs input events 
			{			
				switch(e->type)
				{
					case SDL_JOYBUTTONDOWN: // for controller input
					
					switch(e->jbutton.button)
					{
						case SDL_CONTROLLER_BUTTON_DPAD_UP:
							x=UP;
						break;
						case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
							x=DOWN;
						break;
						case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
							x=LEFT;
						break;
						case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
							x=RIGHT;
						break;
						case SDL_CONTROLLER_BUTTON_A:
							x=SELECT;
						break;
						case SDL_CONTROLLER_BUTTON_B:
							x=CANCEL;
						break;
					}
					
					break;
					
					case SDL_KEYDOWN: // for keyboard input
					
					switch(e->key.keysym.sym)
					{
						case SDLK_RETURN:
							x = SELECT;
						break;
						case SDLK_BACKSPACE:
							x = CANCEL;
						break;
						
						case SDLK_d:
						case SDLK_RIGHT:
							x = RIGHT;
						break;
						
						case SDLK_a:
						case SDLK_LEFT:
							x = LEFT;
						break;
						
						case SDLK_w:
						case SDLK_UP:
							x = UP;
						break;
						
						case SDLK_s:
						case SDLK_DOWN:
							x = DOWN;
						break;
					}
					break;
	
					case SDL_QUIT: // clicking the x window button
					x = EXIT;
					break;
					
					default:
					x = NONE;
					break;
				}
			}

			data = static_cast<input_type>(x); // sets data to value correlating to x
		}
		input_type getEvents() // receives events from the event handler and returns a value correlating with a specific input
		{
			return data;
		}
		~input_handler() // destructor
		{
			delete e;
		}
	private:
		bool held = false;
		input_type data; // input state 
		SDL_Event * e; // gets event inputs
};
