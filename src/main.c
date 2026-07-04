// TODO: Add camera following
// TODO: Add enemy entities.
// TODO: Split the player into two circles
// TODO: Some shooting mechanic
// TODO: Add size to food and grow the player accordingly

#include "../include/raylib.h"
#include <stdbool.h>
#include <stdint.h>
#include "common.h"
#include "colours.h"
#include "darray.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 720
#define INIT_PLAYER_SIZE 25
#define PLAYER_GROWTH_FACTOR 1.025
#define PLAYER_SPEED 10
#define FOOD_SIZE 10
#define FOOD_MAX 10000000
#define FOOD_MAX_DISTANCE 6000

u16 width  = SCREEN_WIDTH;
u16 height = SCREEN_HEIGHT;

// TODO: Refactor this into player/food.h
typedef struct {
	Vector2 pos;
    f32 size;
	Color colour;
	/* Vector2 velocity; */
	/* Vector2 target_velocity; */
	/* Vector2 direction; */
} Food;

typedef struct {
	Vector2 pos;
	Vector2 velocity;
	Vector2 direction;
	Vector2 target_velocity;
    f32     target_size;
	f32     size;
} Player;

DARRAY_INIT(Food)

// TODO: Give food: velocity, acceleration, mass.

static inline Player PlayerInitialize(void) {
	Player p;
	p.pos = (Vector2) {.x = GetScreenWidth()/2, .y = GetScreenHeight()/2};
	p.velocity = (Vector2) { 0 };
	p.target_velocity = (Vector2) { 0 };
	p.direction = (Vector2) { 0 };
	p.target_size = INIT_PLAYER_SIZE;
	p.size = INIT_PLAYER_SIZE;
	return p;
}

static inline Food FoodRandomInitialize(Player* player) {
	Vector2 direction = (Vector2) {.x = GetRandomValue(0, 1), .y = GetRandomValue(0, 1)};
	Vector2 distance = (Vector2) {.x = GetRandomValue(player->size, FOOD_MAX_DISTANCE+player->size), .y = GetRandomValue(player->size, player->size+FOOD_MAX_DISTANCE)};
	direction.x = direction.x ? 1 : -1;
	direction.y = direction.y ? 1 : -1;
	Vector2 position = (Vector2) {.x = player->pos.x + distance.x * direction.x, .y = player->pos.y + distance.y * direction.y};
	return (Food) {.pos = position, .size = FOOD_SIZE, .colour = RANDOM_COLOUR()};
}

static inline void GarbageCollector(Darray_Food* food, Player player) {
	u32 x = player.pos.x;
	u32 y = player.pos.y;
	For (*food) {
		u32 current_x = current.pos.x;
		u32 current_y = current.pos.y;
		if (abs(current_x - x) - player.size > FOOD_MAX_DISTANCE || abs(current_y - y) - player.size > FOOD_MAX_DISTANCE) {
			Food_remove(food, ___);
		}
	}
}

static inline void UpdatePosition(Player* player) {
	player->direction = (Vector2) {0, 0};
	player->direction.y -= IsKeyDown('W');
	player->direction.x	-= IsKeyDown('A');
	player->direction.y += IsKeyDown('S');
	player->direction.x += IsKeyDown('D');

	player->target_velocity.x += player->direction.x * PLAYER_SPEED;
	player->target_velocity.y += player->direction.y * PLAYER_SPEED;

	if (player->direction.x == 0) player->target_velocity.x *= 0.99; // Less than 0.99 for a funny effect
	if (player->direction.y == 0) player->target_velocity.y *= 0.99w;

	player->velocity.x = (player->target_velocity.x - player->velocity.x) * 0.01f;
	player->velocity.y = (player->target_velocity.y - player->velocity.y) * 0.01f;

	player->pos.x = player->pos.x + player->velocity.x;
	player->pos.y = player->pos.y + player->velocity.y;
	/* Clamp(&player->pos, player->size); */
}

int main(void) {
	SetTargetFPS(120);
	Darray_Food food = Food_new();
	char score_string[4] = "0"; // 256
	char food_count_string[10] = "1000"; // 256
	i32 food_count = 1000;
	i32 score = 0;
	f32 time_prev = GetTime();
	f32 clean_timer = time_prev;

	Player player = PlayerInitialize();
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blobs");
	Food_reserve(&food, food_count);
	for (size_t i = 0; i <= food_count; i++)
		Food_push(&food, FoodRandomInitialize(&player));

	Camera2D camera = { 0 };
	camera.target = player.pos;
	camera.zoom = 1.0;
	camera.rotation = 0.0;
	camera.offset = (Vector2) {width/2, height/2};

	// TODO: Add gravity to food.
	while (!WindowShouldClose()) {

		camera.target = player.pos;

		width = GetScreenWidth();
		height = GetScreenHeight();

		BeginDrawing();
     		BeginMode2D(camera);
    		ClearBackground(EGGPLANT);
		    For (food) {
				if (CheckCollisionCircles(player.pos, player.target_size-10, current.pos, FOOD_SIZE)) { // offsets for better visuals
					Food_replace(&food, ___, FoodRandomInitialize(&player));
					player.target_size = player.target_size * PLAYER_GROWTH_FACTOR;
					score++;
					camera.zoom /= 1.01;
					int_to_string(score, score_string);
					continue;
				}
				DrawCircleV(current.pos, FOOD_SIZE, current.colour);
		    }

    		DrawCircleV(player.pos, player.size, WHITE);
    		DrawCircleV(player.pos, INIT_PLAYER_SIZE, YELLOW);
     		EndMode2D();
			DrawFPS(0, 0);
			DrawText(score_string, width-30, 0, 20, RAYWHITE);
			DrawText(food_count_string, 10, height-40, 20, RAYWHITE);
		EndDrawing();

		if (GetTime() - time_prev > 0.01 && food.size < FOOD_MAX) {
			time_prev = GetTime();
			Food_push(&food, FoodRandomInitialize(&player));
			food_count++;
			int_to_string(food_count, food_count_string);
		}

		if (GetTime() - clean_timer > 10) {
			clean_timer = GetTime();
			GarbageCollector(&food, player);
		}

		UpdatePosition(&player);
		player.size += (player.target_size - player.size) * 0.01f;

		if (IsKeyDown('G')) {
			player.target_size = player.target_size * 1.015;
		}

		if (IsKeyPressed('F')) {
			ToggleFullscreen();
		}

		if (IsKeyDown('Q')) {
			CloseWindow();
			return 0;
		}
	}

	Food_free(&food);
	CloseWindow();
	return 0;
}
