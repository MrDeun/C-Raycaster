#include "fmt/core.h"
#include "fmt/format.h"
#include "raylib.h"
#include <cmath>
#include <iostream>

const int WIDTH = 1600;
const int HEIGHT = 900;

typedef struct Player {
  float x;
  float y;
  float dir;
  const float FOV;
  const float Move_Speed;
  const float Rot_Speed;
} Player;

const int map[10][10] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

float fltBool(bool condition) { return condition ? 1.0f : 0.0f; }

Color *createBaseColors() {
  Color *base = new Color[3];
  base[0] = GetColor(0xff000000);
  base[1] = GetColor(0x00ff0000);
  base[2] = GetColor(0x0000ff00);
  return base;
}

Vector2 HandleInput() {
  float right_dir =
      fltBool(IsKeyDown(KEY_RIGHT)) - fltBool(IsKeyDown(KEY_LEFT));
  float forward_dir = fltBool(IsKeyDown(KEY_UP)) - fltBool(IsKeyDown(KEY_DOWN));
  Vector2 wish_dir = {forward_dir, right_dir};
  return wish_dir;
}

void GameLogic(Player *p) {
  Vector2 wish_dir = HandleInput();
  const float newX = p->x + cosf(p->dir) * p->Move_Speed * wish_dir.x;
  const float newY = p->y + sinf(p->dir) * p->Move_Speed * wish_dir.x;
  if (map[(int)newY][(int)newX] == 0) {
    p->x = newX;
    p->y = newY;
  }
  p->dir += p->Rot_Speed * wish_dir.y;
}

void CastRays(const Player *p, const Color &CEILING_COLOR,
              const Color &FLOOR_COLOR, const Color *baseColors) {
  DrawRectangle(0, 0, WIDTH, HEIGHT / 2, CEILING_COLOR);
  DrawRectangle(0, HEIGHT / 2, WIDTH, HEIGHT / 2, FLOOR_COLOR);
  for (int x = 0; x < WIDTH; x++) {
    // Calculate ray position and direction
    float cameraX = 2.0f * x / WIDTH - 1.0f; // x-coordinate in camera space
    float rayDirX = std::cos(p->dir) + std::cos(p->dir - p->FOV / 2 + p->FOV * x / WIDTH);
    float rayDirY = std::sin(p->dir) + std::sin(p->dir - p->FOV / 2 + p->FOV * x / WIDTH);
    
    // Which box of the map we're in
    int mapX = static_cast<int>(p->x);
    int mapY = static_cast<int>(p->y);
    
    // Length of ray from current position to next x or y-side
    float sideDistX, sideDistY;
    
    // Length of ray from one x or y-side to next x or y-side
    float deltaDistX = std::abs(1.0f / rayDirX);
    float deltaDistY = std::abs(1.0f / rayDirY);
    
    // Direction to step in x or y direction (either +1 or -1)
    int stepX, stepY;
    
    // Was a wall hit?
    int hit = 0;
    // Was a NS or a EW wall hit?
    int side;
    
    // Calculate step and initial sideDist
    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (p->x - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0f - p->x) * deltaDistX;
    }
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (p->y - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0f - p->y) * deltaDistY;
    }
    
    // Perform DDA (Digital Differential Analysis)
    while (hit == 0) {
        // Jump to next map square, either in x-direction, or in y-direction
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }
        // Check if ray has hit a wall
        if (mapY >= 0 && mapY < 10 && mapX >= 0 && mapX < 10 && map[mapY][mapX] > 0) {
            hit = 1;
        }
    }
    
    // Calculate distance projected on camera direction
    float perpWallDist;
    if (side == 0) {
        perpWallDist = (mapX - p->x + (1 - stepX) / 2) / rayDirX;
    } else {
        perpWallDist = (mapY - p->y + (1 - stepY) / 2) / rayDirY;
    }
    
    // Calculate height of line to draw on screen
    int lineHeight = static_cast<int>(HEIGHT / perpWallDist);
    
    // Calculate lowest and highest pixel to fill in current stripe
    int drawStart = -lineHeight / 2 + HEIGHT / 2;
    if (drawStart < 0) drawStart = 0;
    int drawEnd = lineHeight / 2 + HEIGHT / 2;
    if (drawEnd >= HEIGHT) drawEnd = HEIGHT - 1;
    
    // Choose wall color based on map value
    // Make y-side walls darker
    
    // Draw the vertical line
    DrawLine(x, drawStart, x, drawEnd, side ? DARKBLUE : BLUE);
}
}

int main() {

  const Color CEILING_COLOR = {0x55, 0x55, 0x55, 0xff};
  const Color FLOOR_COLOR = {0x33, 0x33, 0x33, 0xff};
  Color *baseColors = createBaseColors();
  Player p = {5.0f, 5.0f, PI / 3.0f, PI / 3.0f, 0.05f, PI / 360.0f};
  SetTargetFPS(60);

  InitWindow(WIDTH, HEIGHT, "Hello world!");

  while (!WindowShouldClose()) {
    GameLogic(&p);
    BeginDrawing();
    ClearBackground(WHITE);
    CastRays(&p, CEILING_COLOR, FLOOR_COLOR, baseColors);
    DrawFPS(10, 10);
    EndDrawing();
  }

  CloseWindow();
  delete[] baseColors;
  return 0;
}