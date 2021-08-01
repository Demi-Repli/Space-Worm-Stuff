#ifndef MECHANICS
#include "mechanics.h"
#include "weapon.h"
#endif
							
player::player(weapon_type t)
{
	// set entity image variables
	p.object = new image("assets/sprites/gundam.png",2);
		
	currentWeapon = weapon(&p,t);
			
	p.x = 400;
	p.y = 400;
		
	p.face = right_f;
};

void player::handle() // handles player events/input 
{
	val = h.getEvents();
		
	// processes player input from input event handler  
	if(static_cast<int>(val) > 0)
	{
		p.face = static_cast<direction>(val);
		
		if(p.acceleration < 30)
			p.acceleration+=14*(SCREEN_FPS/30);
				
		if(p.velocity <= 40)
			p.velocity = p.acceleration^2;
		else
			p.velocity = 40;
	}
	else // decreases momentum if there's no movement happening 
	{
		if(val == SELECT) // enter key attacks 
			currentWeapon.input_handler();
		else if(val == CANCEL)
			currentWeapon.switchWeapon();
		
		if(p.velocity > 0)
			p.velocity--;
			
		if(p.acceleration > 0)
			p.acceleration-=2/(SCREEN_FPS/30);
			
		if(val == EXIT)
			close(&main_window);
	}
					
	// moves player based on direction and acceleration 
	switch(p.face)
	{
		case up_f:
			p.y-=p.velocity/(SCREEN_FPS/30);
		break;
			
		case down_f:
			p.y+=p.velocity/(SCREEN_FPS/30);
		break;
			
		case left_f:
			p.x-=p.velocity/(SCREEN_FPS/30);
		break;
			
		case right_f:
			p.x+=p.velocity/(SCREEN_FPS/30);
		break;
			
		default:
		break;
	}
			
	// sets boundaries 
	if(p.x <= 0)
	{
		p.x=0;
		p.velocity = 0;
		p.acceleration = 0;
	}
	else if(p.y <=0)
	{	
		p.y=0;
		p.velocity = 0;
		p.acceleration = 0;
	}
	else if(p.x+p.object->getWidth()*p.object->getScale() >= SCREEN_WIDTH)
	{
		p.x = SCREEN_WIDTH-p.object->getWidth()*p.object->getScale();
		p.velocity = 0;
		p.acceleration = 0;
	}
	else if(p.y+p.object->getHeight()*p.object->getScale() >= SCREEN_HEIGHT)
	{
		p.y = SCREEN_HEIGHT-p.object->getHeight()*p.object->getScale();
		p.velocity = 0;
		p.acceleration = 0;
	}	
		
	currentWeapon.status_handler();
};
		
void player::display() // displays player 
{			
	// displays player image based on direction 
	currentWeapon.display();
			
	if(p.face == left_f) 	
		p.object->renderRotate(p.x,p.y,NULL,0,0,NULL,SDL_FLIP_HORIZONTAL);
	else	
		p.object->render(p.x,p.y);
			
	h.update();
};
	