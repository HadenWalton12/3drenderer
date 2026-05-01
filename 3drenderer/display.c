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
			//color_buffer[(g_window_width * (j + y)) + (i + x)] = color; 
	
			draw_pixel(x + i, y + j, color);

		}
	}
}

//Individually set a pixel
void draw_pixel(int x, int y, uint32_t color)
{
	//Check if values are negative first

	//bounds checking, dont want to draw pixel outside the screen size // ISSUE - DONT CHECK FOR NEGATIVE VALUES
	//Why must we do this? To protect color buffer storing redudant pixel data and to prevent out of bounds errors wih buffer
	if (x < g_window_width && y < g_window_height && x >= 0 && y >= 0)
	{
		color_buffer[(g_window_width * y) + x] = color;

	}
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



	g_window_width = display_mode.w ;
	g_window_height = display_mode.h ;

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
	//SDL_SetWindowBordered(g_window, SDL_WINDOW_RESIZABLE);

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
			//(WINDOW WIDTH * ROW) + Column = Pixel Address in memory (linear  /contiguous sequence of addresses, A 1D Array, NOT 2D
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