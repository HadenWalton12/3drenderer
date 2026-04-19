#include <stdio.h>
#include <stdbool.h>//C by default doesnt have "bool" data type,
#include <SDL.h>

//Pointers? - Object that holds memory address to data at given location.
 
//Global window variable 
SDL_Window* g_window = NULL;

//Global Renderer variable
SDL_Renderer* g_renderer = NULL;

bool is_running = true;

bool initialise_window(void) //We put void inside parameter list since in c, if parameter list empty, compiler thinks any number of arguments can be passed.Explicitly declares this function takes no parameters
{
	//Initialises SDL, overloaded method to declare different SDL components
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)//We declare components of SDL (audio, input, graphics ect...) check if it can declare everything, if it fails, it means we have a permission error
	{
		fprintf(stderr, "Error : Could'nt initialise SDL\n");
		return false;
	}

	//Create SDL Window
	g_window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		800, 
		600, 
		SDL_WINDOW_BORDERLESS
	);

	//Did window initialise? (if window is nullptr)
	if (!g_window)
	{
		fprintf(stderr, "Error : Creating SDL Window\n");
		return false;
	}

	//Create SDL Renderer - Accompanies window
	g_renderer = SDL_CreateRenderer(
	g_window, //Window renderer should accompany
	-1, //Display window is default
	0 //No unqiue settings/flags
	);

	if (!g_renderer)
	{
		fprintf(stderr, "Error : Creating SDL Renderer\n");
		return false;
	}

	return true;
}

void setup(void)
{

}

void process_input(void)
{
	//Event 
	SDL_Event event;
	
	//Get event
	SDL_PollEvent(&event);

	//Event handler
	switch (event.type)
	{
	 case SDL_QUIT:
		is_running = false;
		break;
	 case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			is_running = false;
		}
		break;

	}
}

void update(void)
{

}

void render(void)
{
	SDL_SetRenderDrawColor(g_renderer, 0, 0,0 ,255);
	SDL_RenderClear(g_renderer);//Clear render target

	SDL_RenderPresent(g_renderer);//Present window

}

int main(void)
{
	//Creation of SDL Window
	is_running = initialise_window();

	setup();
	
	//Initialise Game Loop - Basis of game logic (render frame by frame)
	//Game Loop. 1 - Set up, LOOP -> process_input -> update -> render . REPEAT LOOP
	//EACH FRAME OF GAME, HANDLE INPUT, UPDATE GAME LOGIC, THEN RENDER WHICH SHOWS DIFFERENCE FROM PREVIOUS FRAME WITH CURRENT INPUT
	while (is_running)
	{
		process_input();
		update();
		render();

	}
	return 0;
}