#include <SDL.h>
#include <cstdio>

int main(int arg, char* argv[])
{
	SDL_Init(SDL_INIT_TIMER);

	printf("Hello\n");

	SDL_Delay(100);

	SDL_Quit();

	return 1;
}