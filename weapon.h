#ifndef MECHANICS
#include "mechanics.h"
#endif

weapon::weapon(entity * h, weapon_type t)
{
	target = h;
	type = t;
	switch(t)
	{
		case SWORD: // regular sword 
		objecter.object = new image("assets/sprites/sword.png",3);
		break;
		case BLADES: // two blades
		objecter.object = new image("assets/sprites/buster.png",3);
		break;
	}
	
	objecter.angle = 40;
};

// switching weapons
void weapon::switchWeapon()
{
	if(status != THROWN)
	{
		delete objecter.object;
		switch(type)
		{
			case SWORD: // regular sword 
			type = BLADES;
			objecter.object = new image("assets/sprites/buster.png",3);
			break;
		
			case BLADES: // two blades
			type = SWORD;
			objecter.object = new image("assets/sprites/sword.png",3);
			break;
		}
	}
};

void weapon::input_handler() // handles input when concerning weapon
{
	if(status == IDLE)
	{
		if(target->velocity > 0)
			momentum = true;
		else
			momentum = false;
						
		objecter.angle = 80;
		if(type == SWORD)
			status = SLASH;
		else if(type == BLADES)
		{	
			if(target->face == left_f)
				objecter.velocity = -10;
			else
				objecter.velocity = 10;
			objecter.velocity_y = 10*(0-(rand()%2-1));
			status = THROWN;
		}
	}
	else if(status == SLASH)
		status = IDLE;
	else if(status == THROWN)
	{
		if(boxHit(*target,objecter,objecter.object->getWidth(),objecter.object->getHeight()))
			status = IDLE;
	}
	else if(type == BLADES && objecter.angle < 360)
	{
		spinSlash = true;
	}
}

void weapon::status_handler() // handles objecter state
{
	// handles objecter based on the state it is in 
	switch(status)
	{
		case THROWN:
		objecter.x += objecter.velocity;
		objecter.y += objecter.velocity_y;
		
		objecter.angle = objecter.angle%360;
		
		break;
		case SLASH:
		if(!reachedDestination)
		{
			if(target->face == left_f)
				destinationX = target->x-100;
			else
				destinationX = target->x+100;
		
			if(target->face != left_f && objecter.x <= destinationX)
			{
				if(!momentum)
					objecter.velocity=8/(SCREEN_FPS/30);
				else	
					objecter.velocity+=4/(SCREEN_FPS/30);
			}
			else if(target->face == left_f && objecter.x > destinationX)
			{
				if(!momentum)
					objecter.velocity=-8/(SCREEN_FPS/30);
				else	
					objecter.velocity-=4/(SCREEN_FPS/30);
			}
			else
			{
				startSlash = true;
				reachedDestination = true;
				objecter.angle = 30;
			}		
		}
		else
		{
			if(target->face == left_f)
				objecter.x = target->x-100;
			else
				objecter.x = target->x+100;
		
			if(startSlash)
			{
				if(objecter.angle < 120 && type == SWORD)
					objecter.angle+=(8/(SCREEN_FPS/30));
				else if(objecter.angle < 360 && type == BLADES)
					objecter.angle+=(16/(SCREEN_FPS/30));	
				else 
				{
					if(type == SWORD)
						objecter.angle = 10;
					else
						objecter.angle = 0;
				}
			}
			objecter.velocity = 0;
		}
		objecter.x+=objecter.velocity;
		objecter.y = target->y-30;
				
		break;
		case IDLE:		
		objecter.x = target->x;
		objecter.y = target->y-30;
		break;
	}
			
	// sets boundaries 
	if(objecter.x <= 0 || (objecter.y <=0) || 
	  (objecter.x+objecter.object->getWidth()*objecter.object->getScale() >= SCREEN_WIDTH) 
	  ||(objecter.y+objecter.object->getHeight()*objecter.object->getScale() >= SCREEN_HEIGHT))
	{
		if(status != THROWN)
			status = IDLE;
		else
		{
			if(objecter.x <= 0 || objecter.x+objecter.object->getWidth()*objecter.object->getScale() >= SCREEN_WIDTH)
				objecter.velocity *= -1;
			
			if(objecter.y <= 0 || objecter.y+objecter.object->getHeight()*objecter.object->getScale() >= SCREEN_HEIGHT)  
				objecter.velocity_y *= -1;
			
			if(objecter.velocity_y < 0)
				objecter.velocity_y+=2;
			if(objecter.velocity < 0)
				objecter.velocity+=2;
			
			if(objecter.velocity_y > 0)
				objecter.velocity_y-=2;
			if(objecter.velocity > 0)
				objecter.velocity--;
		}
	}
};

void weapon::display() // displays the weapon 
{
	if(target->face == left_f && status != THROWN) 	
		objecter.object->renderRotate(objecter.x,objecter.y,NULL,objecter.angle,0,NULL);
	else	
		objecter.object->renderRotate(objecter.x,objecter.y,NULL,objecter.angle,0,NULL,SDL_FLIP_VERTICAL);
			
	if(status == IDLE)
		objecter.angle+=(3/(SCREEN_FPS/30)); 
	else if(status == THROWN && (objecter.velocity > 0 || objecter.velocity_y > 0))
		objecter.angle+=(20/(SCREEN_FPS/30)); 	
};
