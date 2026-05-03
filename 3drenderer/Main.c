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

#define FOV (900) //Field of View - Used in scalar calculations to bring object furter or closer in projected plane
//Array of vectors to define cube
#define POINTS (9 * 9 * 9)//NO MAGIC NUMBERS! Bad practise - Use macro
vec3_t cube[POINTS];

vec2_t projected_cube[POINTS];

//Origin viewpoint for  perspective projection
vec3_t camera_pos = {.x = 0.0, .y = 0.0, .z = -5};
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

	int point_counter = 0;
	//Load and create vec array in 3D cartesian range -1 to 1
	for (float x = -1; x <= 1; x += 0.25)
	{
		for (float y  = -1; y <= 1; y += 0.25)
		{
			for (float z = -1; z <= 1; z += 0.25)
			{
				//Create new point each iteration
				vec3_t new_point = { .x = x , .y = y, .z = z };
				
				//Add to array
				cube[point_counter++] = new_point;
			}//INNER LOOP

		}//MIDDLE LOOP

	}//OUTER LOOP


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

////Basic orthographic projection - we ignore the Z
//vec2_t project(vec3_t point)
//{
//	vec2_t projected_point = {
//		.x = (FOV * point.x),
//		.y = (FOV *  point.y) 
//
//		//MODIFIED TO USE SCALAR MULTIPLICATION TO DENORMALISE THE VECTOR VALUES
//
//	};
//
//	return projected_point;
//}


//Basic Projective prospection - Convert 3D point in 2D space, using the depth to scale position of point in projection space(the monitor)
//Still orthographic projection (convert 3D to 2D) however we didnt ignore the Z value
//However this has issues, it doesnt really look like a cube, regardless, we added projection.
vec2_t project(vec3_t point)
{
	vec2_t projected_point = {
		.x = (FOV * point.x) / point.z,
		.y = (FOV * point.y) / point.z
		//NOTE - FOV is used to scale point closer or further in projected space
	};

	return projected_point;
}
void update(void)
{

	
	//Project cube point
	for (int i = 0; i < POINTS; i++)
	{
		vec3_t point = cube[i];

		point.z -= camera_pos.z;//Move points away from camera
		//Save projected 2D vector (point)
		projected_cube[i] = project(point);
	}

}

void render(void)
{
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	SDL_RenderClear(g_renderer);//Clear render target

	//draw_grid();//Draw to color buffer before presenting to tex object

	//draw_rect(200, 200, 200, 100, rand());
	//draw_rect(rand() % g_window_width, rand() % g_window_height, 200, 100, rand()); //Draws without no bounds checking
	
	//for (int i = 0; i < 2500; i++)
	//{
	//	draw_pixel(5, i, rand());

	//}

	//We can use draw pixel function to draw each point projected - loop through them
	for (int i = 0; i < POINTS; i++)
	{
		vec2_t p_point = projected_cube[i];
		draw_rect(p_point.x + (g_window_width / 4), p_point.y + (g_window_height / 4), 4, 4, rand());
	}
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