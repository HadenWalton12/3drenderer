#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>//C by default doesnt have "bool" data type,
#include <SDL.h>

//Pointers? - Object that holds memory address to data at given location.
 
//Malloc - Dynamically allocate number of bytes in heap
//Calloc - Dyanmically allocates number of bytes to heap, also sets allocated memory to zero
//sizeOF - C Language operator


//ColourBuffer - Type of pixel buffer that stores pixel color data. Must create a data structure to represent and 
//store this data. Data structure will be an array of elements
//NOTE - COLOR AND FRAME BUFFER REFER TO SAME THING
//uint32_t -Unsigned int data type, provides exactly 4 bytes

//Pointer to an array of uint32 elements
uint32_t* color_buffer = NULL; //Pointer to first address position of the array, to make color buffer useful to SDL, we set buffer to SDL_Texture
SDL_Texture* color_buffer_tex = NULL; //Represents pixel data , storing color buffer data
//Global window variable 
SDL_Window* g_window = NULL;

//Global Renderer variable
SDL_Renderer* g_renderer = NULL;

bool is_running = true;

int g_window_width = 800;
int g_window_height = 600;


void render_color_buffer()
{
	//Update texture with raw data
	SDL_UpdateTexture(
		color_buffer_tex, //Buffer to copy to
		NULL,
		color_buffer, //Raw Data - Copy to color buffer
		(g_window_width * sizeof(uint32_t))// Pitch - memory layout of image data
	);

	SDL_RenderCopy(g_renderer, color_buffer_tex, NULL, NULL);//Do not need to copy anything
}
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
		g_window_width, 
		g_window_height,
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

void clear_color_buffer(uint32_t color)
{
	//Loop through each pixel value, set color
	for (int row = 0; row < g_window_height; row++)
	{
		for (int column = 0; column < g_window_width; column++)
		{
			color_buffer[(g_window_width * row) + column] = color;//Since color-buffer is pointer to array of addresses, use this calculation to get to the next address in memory
			//(WINDOW WIDTH * ROW) + Column = Pixel Address in memory (linear sequence of addresses
		}
	}

}



void setup(void)
{

	//Allocate requires bytes of memory for colour buffer - How many bytes do we need?
	//Size of render target window (width * height);
	//Malloc ? C style dynamic memory allocation - We create a colour buffer where each element is casted to a address, where data
	//type is size of uint32_t (exactly 4 bytes), number of elements is determentant of width and height of window
	//NOTE - This isnt a matrix or 2d array, its a sequential, contigous block of addressible eleents (linear sequence of color values)
	//NOTE 2 - MALLOC IS POWERFUL - be careful using it since we need to manage lifetime of dynamically allocated objects
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * g_window_width * g_window_height);
	
	//Simple modification of color buffer
	//color_buffer[0] = 0xFFFF0000;

	//TO access pixels - (WINDOW WIDTH * ROW) + COLUMN - Convert 2D coordinates into 1D array
	//Pixel at row 10, colum 20 set color red
	//color_buffer[(g_window_width * 10) + 20] = 0xFFFF0000;

	//Create Color Buffer
	color_buffer_tex = SDL_CreateTexture(
		g_renderer,
		SDL_PIXELFORMAT_ARGB8888, //Format type of storing pixel data
		SDL_TEXTUREACCESS_STREAMING, //Texture will change frequently, per-frame
		g_window_width, //Texture will cover width of screen
		g_window_height //Texture will cover height of screen
	);
}

void cleanup(void)
{
	//Free - Deallocates objects
	free(color_buffer);
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	SDL_Quit();

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

	render_color_buffer();
	clear_color_buffer(0XFFFFFF00);
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

	cleanup();

	return 0;
}