#ifndef SDL_MAIN_HANDLED
#include "graphical.h"
#include "input.h"
#include "text.h"
#include "image.h"
#endif 

#ifndef MECHANICS	
#define MECHANICS

// for directions a sprite is facing 
enum direction {	left_f=LEFT, 
					right_f=RIGHT, 
					up_f=UP, 
					down_f=DOWN, 
					
					downr_f = DOWN_RIGHT,
					downl_f = DOWN_LEFT,
					
					upr_f = UP_RIGHT,
					upl_f = UP_LEFT,
					
					none = 0};

struct entity // structure of entities in the game and how they move 
{
	image * object = NULL;
	int x = 0;
	int y = 0;
	int acceleration = 0;
	int velocity = 0;
	int velocity_y = 0;
	bool visible; // used for enemies 
	
	direction face;
	
	SDL_Point rotate_point;
	
	int angle = 0;
};

// class for handling levels the player is in 
class level
{
	public:
		level(){}
		void display()
		{
			// displaying background and scrolling it 
			for(int i= 0;i<(SCREEN_WIDTH/960)+2;i++)
				back.render(s+i*960,0);
			s-=(4/(SCREEN_FPS/30));
			
			// resets placement of background 
			if(s == -960)
				s = 0;
		}
	private:
		// background image 
		image back = image("assets/sprites/background.png");
		
		// counter for scrolling background 
		int s = 0;
};

// weapon types in the game 
enum weapon_type {SWORD,
				  BLADES,
				  GUNBLADE,
				  SPEAR};

// states the weapon can be in 
enum weapon_state {THROWN,
				   SLASH,
				   IDLE};

// class for handling weapons 
class weapon
{
	public:
	weapon(){}
	weapon(entity * h, weapon_type t);
	void status_handler();
	void input_handler();
	void display();
	void switchWeapon();
	struct entity getInfo()
	{
		return objecter;
	}
	
	private:
	// weapon information
	weapon_type type; 
	weapon_state status;
	
	// location of the weapon 
	entity objecter;
	
	bool momentum = false; // does the player have momentum when the sword is first used?
	bool reachedDestination = false; // has the weapon reached its max distance?
	bool startSlash = false; // has the weapon started slashing?
	bool spinSlash = false; // is the player holding the enter key when the slash attack occurs?
	int destinationX; // location the weapon ends up at 
		
	// the specific entity that the weapon is being used by 
	entity * target;
};

// For handling the player 
class player
{
	public:
		player(weapon_type t);
		void handle(); // handles player events/input 
		void display(); // displays player 
		struct entity getInfo()
		{
			return p;
		}
		struct entity getWeaponInfo()
		{
			return currentWeapon.getInfo();
		}
	private:
		// handles player input 
		input_handler h = input_handler();
	
		// handling input with the enumeration
		input_type val;
			
		// the weapon the player has 
		weapon currentWeapon;
		
		// holds object info/physics in the game environment 
		struct entity p; // player coordinate/physics info 
};

// For handling enemies on the screen
class enemy
{
	public:
	enemy(player * j);
	void display(); // displays enemies 
	void behavior(); // handles behavior of enemy 
	void spawnLocation(); // spawning an enemy 
	
	private:
	image * enem; // what an enemy looks like 
	int numEnem = 0; // number of enemies 
	struct entity * local; // holds enemy info 
	player * target; // the player that the enemies are targetting 
};

bool boxHit(const entity one, const entity two, int extraX=0, int extraY=0) // are two objects hitting?
{
	bool condition = false;
	/*
		thing for visualizing math because I suck
				0,1            0+getWidth(),1
				   ------------
				   |          |
				   |          |
				   |          |
				   |          |
			       |          |
				   |          |
				   ------------
	0,1+getHeight()		 	   2,3
	*/
	
	// two object in one's x range 
	if(two.x >= one.x && two.x <= one.x+one.object->getWidth()+extraX)
	{
		// two in one's y range 
		if(two.y >= one.y && two.y <= one.y+one.object->getHeight()+extraY)
			condition = true;
	}
	// one object in two's x range
	else if(one.x >= two.x && one.x <= two.x+one.object->getWidth()+extraX)
	{
		// one in two's y range 
		if(one.y >= two.y && one.y <= two.y+two.object->getHeight()+extraY)
			condition = true;	
	}

	return condition;
}

#endif