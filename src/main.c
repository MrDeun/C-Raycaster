#include "SDL_events.h"
#include "SDL_surface.h"
#include "SDL_video.h"
#include <SDL.h>
#include <SDL_stdinc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 800
#define HEIGHT 600

#define true 1
#define false 0

typedef struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} Color;

Color create_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  Color new_color;
  new_color.r = r;
  new_color.g = g;
  new_color.b = b;
  new_color.a = a;
  return new_color;
}

void fillBuffer(Color *buf, Color color) {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      buf[x + WIDTH * y] = color;
    }
  }
}

Uint32 packColor(Color color) {
  Uint32 packet = color.r << 24 | color.g << 16 | color.b << 8 | color.a;
  return packet;
}

void updateBuffer(Color *buf, SDL_Surface *surface) {
  SDL_LockSurface(surface);
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      Uint32 color = packColor(buf[x + WIDTH * y]);
      Uint8 *pixel = (Uint8 *)surface->pixels;
      pixel += (y * surface->pitch) + (x * sizeof(Uint32));
      *((Uint32 *)pixel) = color;
    }
  }
    SDL_UnlockSurface(surface);
}

void setPixel(Color *buf, Color color, uint32_t x, uint32_t y) {
  if (x > WIDTH || y > HEIGHT) {
    return;
  }
  buf[x + WIDTH * y] = color;
}

Color red;

int main(int, char **) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Raycayster", 0, 0, WIDTH, HEIGHT,SDL_WINDOW_SHOWN);
  SDL_Surface *surface = SDL_GetWindowSurface(window);
  Color *buffer = malloc(sizeof(Color) * WIDTH * HEIGHT);
  red = create_color(255, 0, 0, 0);
  fillBuffer(buffer, red);

  int shouldQuit = false;
  SDL_Event e;
  while (!shouldQuit) {
    SDL_PollEvent(&e);
    if (e.type == SDL_QUIT) {
      shouldQuit = true;
    }
    updateBuffer(buffer, surface);
  }

  SDL_Quit();
  free(buffer);
}
