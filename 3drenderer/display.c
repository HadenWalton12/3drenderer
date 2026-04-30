#include "display.h"
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

	SDL_GetCurrentDisplayMode(0, &display_mode);//Reference display mode to update display mode contents with adapter/display properties



	g_window_width = display_mode.w;
	g_window_height = display_mode.h;

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

	//Change to true fullscreen
	SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN);

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


void cleanup(void)
{
	//Free - Deallocates objects
	free(color_buffer);
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	SDL_Quit();

}