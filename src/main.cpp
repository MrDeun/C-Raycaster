#include <cmath>
#include "raylib.h"

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
  const float newY = p->x + sinf(p->dir) * p->Move_Speed * wish_dir.x;
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
    const float cameraX = 2.0f * (float)x / WIDTH - 1.0f;
    const float rayDirX = cosf(p->dir) + cosf(p->dir - p->FOV / 2.0 + p->FOV * (float)x / WIDTH);
    const float rayDirY = sinf(p->dir) + sinf(p->dir - p->FOV / 2.0 + p->FOV * (float)x / WIDTH);

    int mapX = (int)p->x;
    int mapY = (int)p->y;


  }
}

int main() {

  const Color CEILING_COLOR = {0x55,0x55,0x55,0xff};
  const Color FLOOR_COLOR = {0x33,0x33,0x33,0xff};
  Color *baseColors = createBaseColors();
  Player p = {5.0f, 5.0f, PI / 3.0f, PI / 3.0f, 0.05f, PI / 30.0f};
  SetTargetFPS(60);

  InitWindow(WIDTH, HEIGHT, "Hello world!");

  while (!WindowShouldClose()) {
    GameLogic(&p);
    BeginDrawing();
    ClearBackground(WHITE);
    CastRays(&p, CEILING_COLOR, FLOOR_COLOR, baseColors);

    EndDrawing();
  }

  CloseWindow();
  delete[] baseColors; 
  return 0;
}