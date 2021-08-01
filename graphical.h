
// HEADER FOR BASE GRAPHICAL IMPLEMENTATION

// Programmed by Demi
// 4/1/2021 - 

// v0.1

#ifndef SDL_MAIN_HANDLED

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include <cmath>
#include <cstdlib>
#include <ctime>

#include <string.h>
#include <string>

#include <random>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// Color constants 
#define WHITE 255,255,255
#define BLACK 0,0,0

//Screen dimension constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 720;

int SCREEN_FPS = 30; // max FPS 
int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS; // calculates ticks per frame for timers

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

//Game Controller 1 handler
SDL_Joystick* gGameController = NULL;

bool debug = false; // is the game in debug mode?

class image;
class handler;

class timer // class for handling timers
{
	public:
		timer()
		{
			started = false;
			paused = false;
			
			startTicks = 0;
			pauseTicks = 0;
		}
		void start()
		{
			started = true;
			paused = false;
			
			startTicks = SDL_GetTicks();
			pauseTicks = 0;
		}
		void stop()
		{
			started = false;
			paused = false;
			
			startTicks = 0;
			pauseTicks = 0;	
		}
		void pause()
		{
			if(started && !paused)
			{
				paused = true;
				
				pauseTicks = SDL_GetTicks() - startTicks;
				startTicks = 0;
			}
		}
		void resume()
		{
			if(started && paused)
			{
				paused = false;
				
				startTicks = SDL_GetTicks() - pauseTicks;
				pauseTicks = 0;
			}	
		}
		Uint32 getTicks()
		{
			Uint32 t = 0;
			if(started)
			{
				if(paused)
					t = pauseTicks;
				else
					t = SDL_GetTicks() - startTicks;
			}
			return t;
		}
		bool isRunning()
		{
			return started;
		}
		bool isPaused()
		{
			return paused && started;
		}
	private:
		Uint32 startTicks = 0;
		Uint32 pauseTicks = 0;
		bool paused = false;
		bool started = false;
	
};

struct window // main structure used for handling the window
{
	bool success = false; // was the window sucessfully made?
		
	Uint8 colors[4] = {WHITE,0}; // background color of the window
	
	int currentTrack = 0; // current song being played on the window
	Mix_Music * gMusic; // for playing music	
	
	SDL_Window* gWindow = NULL; // window 
	SDL_Renderer* gRenderer = NULL; // window renderer
	
	SDL_Surface* text; // surfaces used for holding images and text
	SDL_Surface* images; 
	
	SDL_Texture* gTexture = NULL; // displayed texture
	
	timer framekeeper; // timers used for frame rate information and capping framerate
	timer capkeeper;
	
	int frames = 0; // frame counter
	double avgFPS; // the average FPS calculated throughout the window operation
};

// WINDOW FUNCTIONS---------------------------------------------------------------

// open and close 

void init(struct window * win) // creates a window 
{
	srand((unsigned)time(NULL));
	
	if( SDL_Init( IMG_INIT_JPG | IMG_INIT_PNG | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) < 0 )
	{
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}		

		//Check for joysticks
		if( SDL_NumJoysticks() < 1 )
		{
		}
		else
		{
			//Load joystick
			gGameController = SDL_JoystickOpen( 0 );
			if( gGameController == NULL )
			{
				printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
			}
		}

		//Create window
		win->gWindow = SDL_CreateWindow( "Graphics", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if(win->gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			win->success = false;
		}
		else
		{
			//Create renderer for window
			win->gRenderer = SDL_CreateRenderer(win->gWindow, -1, SDL_RENDERER_ACCELERATED );
			if(win->gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				win->success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(win->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					win->success = false;
				}
						
				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					win->success = false;
				}
						
				//Initialize SDL_mixer
				if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024*2 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					win->success = false;
				}
			}
		}
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // sets render quality to where images aren't blurry when resized
	win->framekeeper.start();
}

void close(struct window * win) // closes a window
{
	SDL_DestroyTexture( win->gTexture );
	win->gTexture = NULL;
	
	//Destroy window	
	SDL_DestroyRenderer( win->gRenderer );
	SDL_DestroyWindow( win->gWindow );
	win->gWindow = NULL;
	win->gRenderer = NULL;
			
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
			
	exit(0);
}

// display

void display(struct window * win) // refreshes window and displays what's on the renderer
{

	win->avgFPS = win->frames / (win->framekeeper.getTicks() / 1000.f);
	if( win->avgFPS > 2000000 )
	{
		win->avgFPS = 0;
	}
	
	
	SDL_RenderPresent(win->gRenderer);
	++(win->frames); // adds to frame tally
	
	int framet = win->capkeeper.getTicks(); // for capping frame rate
	if( framet < SCREEN_TICK_PER_FRAME)
	{
		SDL_Delay( SCREEN_TICK_PER_FRAME - framet );
	}
}

void setBackColor(struct window * win, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	win->colors[0] = r;
	win->colors[1] = g;
	win->colors[2] = b;
	win->colors[3] = a;
	SDL_SetRenderDrawColor(win->gRenderer, r, g, b, a);
	SDL_RenderClear(win->gRenderer );
}

void clearDisplay(struct window * win) // clears the screen 
{
	SDL_SetRenderDrawColor( win->gRenderer, win->colors[0], win->colors[1], win->colors[2], win->colors[3]);
	SDL_RenderClear(win->gRenderer );
	win->capkeeper.start();
}

// music 

void playMusic(struct window * win, int i) // plays music 
{
	if(i!= win->currentTrack)
	{
		Mix_HaltMusic();
		switch(i)
		{
			case 1:
			win->gMusic =  Mix_LoadMUS( "assets/music/The First Blood.wav" );
			break;
			case 2:
			win->gMusic =  Mix_LoadMUS( "assets/music/Weakling Song.wav" );
			break;
		}
		win->currentTrack = i;
		Mix_PlayMusic( win->gMusic, -1 ); // plays music
	}
}

void playMusicFile(struct window * win, string name) // plays music directly from file path
{
	Mix_HaltMusic();
	win->gMusic =  Mix_LoadMUS(name.c_str());
	
	win->currentTrack = 0;
	Mix_PlayMusic( win->gMusic, -1 ); // plays music
}

void changeVolume(int i) // 10 is max
{
	 Mix_VolumeMusic(i*5);
}

//-------------------------------------------------------------------------------

static struct window main_window; // main window 

#endif