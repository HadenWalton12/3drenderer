//#pragma once
#ifndef display_h
#define display_h

#include "pch.h"
////pointer to an array of uint32 elements
extern uint32_t* color_buffer; //pointer to first address position of the array, to make color buffer useful to sdl, we set buffer to sdl_texture
extern SDL_Texture* color_buffer_tex; //represents pixel data , storing color buffer data
////global window variable 
extern SDL_Window* g_window;

// renderer variable
extern SDL_Renderer* g_renderer;
extern bool is_running;
extern int frame;
extern int g_window_width;
extern int g_window_height;
//dynamic initialisation of window creation - query sdl to get adapter fullscreen max width and height
extern SDL_DisplayMode display_mode;
//function protypes / signatures
void render_color_buffer();
bool initialise_window(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_grid(void);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void clear_color_buffer(uint32_t color);
#endif // !display_h

