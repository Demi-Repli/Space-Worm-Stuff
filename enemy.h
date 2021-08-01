#ifndef MECHANICS
#include "mechanics.h"
#endif

enemy::enemy(player * j)
{
	target = j;
	enem = new image("assets/sprites/dummy.png",2);
	local = new struct entity;
	
	local->object = enem;
	local->x = SCREEN_WIDTH-50;
	local->y = 200;
	local->visible = true;
};

void enemy::display()
{
	behavior();
	if(local->visible)
		enem->render(local->x,local->y);
	
};

void enemy::behavior()
{
	if(boxHit(*local,target->getInfo()))
		local->visible = false;
	else if(boxHit(*local,target->getWeaponInfo()))
		local->visible = false;
	local->x-=5;
	if(local->x < 0)
		local->x = SCREEN_WIDTH;
};

void enemy::spawnLocation()
{
	local->visible = true;
	numEnem++;
};
