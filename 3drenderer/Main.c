#include "display.h"


//Pointer to an array of uint32 elements
uint32_t* color_buffer = NULL; //Pointer to first address position of the array, to make color buffer useful to SDL, we set buffer to SDL_Texture
SDL_Texture* color_buffer_tex = NULL; //Represents pixel data , storing color buffer data
//Global window variable 
SDL_Window* g_window = NULL;

//Global Renderer variable
SDL_Renderer* g_renderer = NULL;

bool is_running = true;
int frame = 0;
int g_window_width;
int g_window_height;

//Dynamic Initialisation of Window Creation - Query SDL to get adapter fullscreen max width and height
SDL_DisplayMode display_mode;




void draw_grid(void)
{

	//Loop through each pixel value, set color
	for (int row = 0; row < g_window_height; row++)
	{
		for (int column = 0; column < g_window_width; column++)
		{
			//We want to draw grid between pixels, we space out between every 30 on row and column.
			//If that pixel iteration is divisible by 10, then we modify colour of pixel.
			//Must use || since we want to modify position on row and column
			if (row % 100 == 0 || column % 100 == 0)
			{
				color_buffer[(g_window_width * row) + column] = 0X000000;
			}
		}
	}

}

void draw_rect(int x, int y, int width, int height, uint32_t color)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			//Calculate pixel offsets in buffer
			color_buffer[(g_window_width * (j + y)) + (i + x)] = color;

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
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * (g_window_width * g_window_width));

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
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	SDL_RenderClear(g_renderer);//Clear render target

	//draw_grid();//Draw to color buffer before presenting to tex object

	draw_rect(200, 200, 200, 100, 0X0FFF00);
	//draw_rect(rand() % g_window_width, rand() % g_window_height, 200, 100, 0X000000); Draws without no bounds checking

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