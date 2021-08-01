#include "mechanics.h"
#include "weapon.h"
#include "player.h"
#include "enemy.h"

/* 	
	g++ movement.cpp -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -static-libstdc++ -std=c++11 -Wl,-rpath,./ -o movement
*/

int main()
{
	init(&main_window);
	bool done = false;
	
	player user = player(SWORD);
	level l = level();
	enemy t = enemy(&user);
	
	playMusic(&main_window,1);
	while(!done)
	{
		clearDisplay(&main_window);
		
		user.handle();
		
		l.display();
		user.display();
		
		t.display();
		
		display(&main_window);
	}
	
	close(&main_window);
}