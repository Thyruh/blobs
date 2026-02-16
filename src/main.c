#include "../include/raylib.h"
#include "../include/helper.h"
#include <stdbool.h>
#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef bool b32;

typedef float f32;
typedef double f64;

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 720
#define PLAYER_SIZE 25
#define PLAYER_SPEED 1
#define FOOD_SIZE 10

f64 growth_factor;

static inline Vector2 EntityInitialize(void) {
   return (Vector2) {.x = (i32)(GetScreenWidth()/2), .y = (i32)(GetScreenHeight()/2)};
}

static inline Vector2 EntityRandomInitialize(void) {
   return (Vector2) {.x = GetRandomValue(FOOD_SIZE, GetScreenWidth()-FOOD_SIZE), .y = GetRandomValue(FOOD_SIZE, GetScreenHeight()-FOOD_SIZE)};
}

static inline void ClampX(Vector2* current_pos) {
   if (current_pos->x+PLAYER_SIZE*growth_factor > GetScreenWidth()) { // -PLAYER_SIZE for a funny dangling effect
      current_pos->x = GetScreenWidth()-PLAYER_SIZE*growth_factor;
   }
   else if (current_pos->x-PLAYER_SIZE*growth_factor < 0) current_pos->x = 0+PLAYER_SIZE*growth_factor; // +50
}

static inline void ClampY(Vector2* current_pos) {
   if (current_pos->y+PLAYER_SIZE*growth_factor > GetScreenHeight()) {
      current_pos->y = GetScreenHeight()-PLAYER_SIZE*growth_factor;
   }
   else if (current_pos->y-PLAYER_SIZE*growth_factor < 0) current_pos->y = 0+PLAYER_SIZE*growth_factor;
}

static inline void UpdatePositionX(Vector2* current_pos, f64 step) {
   current_pos->x = current_pos->x+step;
}

static inline void UpdatePositionY(Vector2* current_pos, f64 step) {
   current_pos->y = current_pos->y+step;
}

int main(void) {
   char food_counter_string[2]; // 999


   growth_factor = 1;

   InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MyWindow");


   Vector2 player_pos = EntityInitialize();
   Vector2 food_pos = EntityRandomInitialize();

   u8 food_counter = 0;

   while(!WindowShouldClose()) {
      DrawFPS(0, 0);

      BeginDrawing();
         DrawText(food_counter_string, SCREEN_WIDTH-30, 0, 20, RAYWHITE);
         if (!CheckCollisionCircles(player_pos, PLAYER_SIZE*growth_factor-10, food_pos, FOOD_SIZE)) { // offsets for better visuals
            DrawCircleV(food_pos, FOOD_SIZE, GREEN);
         }
         else {
            food_pos = EntityRandomInitialize();
            food_counter++;
            growth_factor = growth_factor + growth_factor*0.015;
            int_to_string(food_counter, food_counter_string);
         }

         ClearBackground(BLACK);
         DrawCircleV(player_pos, PLAYER_SIZE*growth_factor, WHITE);
         DrawCircleV(player_pos, PLAYER_SIZE, BLUE);
      EndDrawing();

      if(IsKeyDown('A')) {
         UpdatePositionX(&player_pos, -PLAYER_SPEED);
         ClampX(&player_pos);
      }
      else if(IsKeyDown('D')) {
         UpdatePositionX(&player_pos, PLAYER_SPEED);
         ClampX(&player_pos);
      }
      if(IsKeyDown('S')) {
         UpdatePositionY(&player_pos, PLAYER_SPEED);
         ClampY(&player_pos);
      }
      else if(IsKeyDown('W')) {
         UpdatePositionY(&player_pos, -PLAYER_SPEED);
         ClampY(&player_pos);
      }

      if(IsKeyPressed('F')) {
         ToggleFullscreen();
         ClampX(&food_pos);
         ClampY(&food_pos);
      }

      if(IsKeyDown('Q')) {
         CloseWindow();
         return 0;
      }

   }

   CloseWindow();
   return 0;
}
