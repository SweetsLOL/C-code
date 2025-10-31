#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PLAYER_SIZE 20
#define MAX_OBSTACLES 50

typedef struct {
    float x, y;
    float width, height;
    float speed;
    bool active;
} Obstacle;

typedef enum {
    STATE_TITLE,
    STATE_STAGE_SELECT,
    STATE_GAMEPLAY,
    STATE_GAME_OVER
} GameState;

bool PointInRect(Vector2 player, Rectangle rect) {
    return (player.x >= rect.x && player.x <= rect.x + rect.width &&
            player.y >= rect.y && player.y <= rect.y + rect.height);
}

// Intersection test: segment vs segment
bool SegmentsIntersect(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4)
{
    float d =
        (p4.y - p3.y) * (p2.x - p1.x) -
        (p4.x - p3.x) * (p2.y - p1.y);

    if (d == 0) return false;

    float u =
        ((p3.x - p1.x) * (p2.y - p1.y) -
         (p3.y - p1.y) * (p2.x - p1.x)) / d;

    float v =
        ((p3.x - p1.x) * (p4.y - p3.y) -
         (p3.y - p1.y) * (p4.x - p3.x)) / d;

    return (u >= 0 && u <= 1 && v >= 0 && v <= 1);
}

bool SegmentVsRect(Vector2 a, Vector2 b, Rectangle r)
{
    Vector2 r1 = {r.x, r.y};
    Vector2 r2 = {r.x + r.width, r.y};
    Vector2 r3 = {r.x + r.width, r.y + r.height};
    Vector2 r4 = {r.x, r.y + r.height};

    if (SegmentsIntersect(a, b, r1, r2)) return true;
    if (SegmentsIntersect(a, b, r2, r3)) return true;
    if (SegmentsIntersect(a, b, r3, r4)) return true;
    if (SegmentsIntersect(a, b, r4, r1)) return true;

    return false;
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Triangle Dodge");
    SetTargetFPS(60);

    GameState state = STATE_TITLE;

    float playerX = SCREEN_WIDTH/2;
    float playerY = SCREEN_HEIGHT - 80;
    float playerSpeed = 400;

    Obstacle obstacles[MAX_OBSTACLES] = {0};
    float spawnTimer = 0;
    float spawnRate = 0.6f;

    bool alive = true;
    float timeAlive = 0;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // ===== TITLE =====
        if (state == STATE_TITLE)
        {
            if (IsKeyPressed(KEY_ENTER))
                state = STATE_STAGE_SELECT;
        }

        // ===== STAGE SELECT =====
        else if (state == STATE_STAGE_SELECT)
        {
            if (IsKeyPressed(KEY_ONE))  spawnRate = 0.6f, state = STATE_GAMEPLAY;
            if (IsKeyPressed(KEY_TWO))  spawnRate = 0.4f, state = STATE_GAMEPLAY;
            if (IsKeyPressed(KEY_THREE))spawnRate = 0.25f,state = STATE_GAMEPLAY;

            if (state == STATE_GAMEPLAY)
            {
                alive = true;
                timeAlive = 0;
                playerX = SCREEN_WIDTH/2;

                for (int i = 0; i < MAX_OBSTACLES; i++)
                    obstacles[i].active = false;
            }
        }

        // ===== GAMEPLAY =====
        else if (state == STATE_GAMEPLAY && alive)
        {
            timeAlive += dt;

            // Movement prediction
            float newX = playerX;
            if (IsKeyDown(KEY_LEFT))  newX -= playerSpeed * dt;
            if (IsKeyDown(KEY_RIGHT)) newX += playerSpeed * dt;
            if (newX < PLAYER_SIZE) newX = PLAYER_SIZE;
            if (newX > SCREEN_WIDTH - PLAYER_SIZE) newX = SCREEN_WIDTH - PLAYER_SIZE;

            // Triangle current vertices
            Vector2 tip    = { playerX,               playerY - PLAYER_SIZE };
            Vector2 left   = { playerX - PLAYER_SIZE, playerY + PLAYER_SIZE };
            Vector2 right  = { playerX + PLAYER_SIZE, playerY + PLAYER_SIZE };

            // Triangle future vertices
            Vector2 fTip    = { newX,               playerY - PLAYER_SIZE };
            Vector2 fLeft   = { newX - PLAYER_SIZE, playerY + PLAYER_SIZE };
            Vector2 fRight  = { newX + PLAYER_SIZE, playerY + PLAYER_SIZE };

            // Spawn
            spawnTimer += dt;
            if (spawnTimer >= spawnRate)
            {
                spawnTimer = 0;
                for (int i = 0; i < MAX_OBSTACLES; i++)
                {
                    if (!obstacles[i].active)
                    {
                        obstacles[i].active = true;
                        obstacles[i].x = GetRandomValue(0, SCREEN_WIDTH - 50);
                        obstacles[i].y = -50;
                        obstacles[i].width = GetRandomValue(40, 100);
                        obstacles[i].height = 20;
                        obstacles[i].speed = GetRandomValue(150, 250);
                        break;
                    }
                }
            }

            // Move obstacles
            for (int i = 0; i < MAX_OBSTACLES; i++)
            {
                if (!obstacles[i].active) continue;

                obstacles[i].y += obstacles[i].speed * dt;
                if (obstacles[i].y > SCREEN_HEIGHT)
                    obstacles[i].active = false;
            }

            // Collision detection
            bool block = false;

            for (int i = 0; i < MAX_OBSTACLES; i++)
            {
                if (!obstacles[i].active) continue;
                Rectangle r = { obstacles[i].x, obstacles[i].y,
                                obstacles[i].width, obstacles[i].height };

                // Tip death
                if (PointInRect(tip, r)) {
                    alive = false;
                    state = STATE_GAME_OVER;
                    break;
                }

                // Side / bottom blocking
                if (SegmentVsRect(fTip, fLeft, r) ||
                    SegmentVsRect(fTip, fRight, r) ||
                    SegmentVsRect(fLeft, fRight, r))
                {
                    block = true;
                    break;
                }
            }

            if (!block)
                playerX = newX;
        }

        // ===== GAME OVER =====
        else if (state == STATE_GAME_OVER)
        {
            if (IsKeyPressed(KEY_ENTER))
                state = STATE_STAGE_SELECT;
        }

        // ===== DRAW =====
        BeginDrawing();
        ClearBackground(BLACK);

        if (state == STATE_TITLE)
        {
            DrawText("TRIANGLE DODGE", 240, 200, 40, WHITE);
            DrawText("Press ENTER", 310, 300, 20, YELLOW);
        }
        else if (state == STATE_STAGE_SELECT)
        {
            DrawText("SELECT STAGE", 260, 120, 40, WHITE);
            DrawText("1 - Easy",   330, 220, 30, GREEN);
            DrawText("2 - Medium", 330, 270, 30, YELLOW);
            DrawText("3 - Hard",   330, 320, 30, RED);
        }
        else if (state == STATE_GAMEPLAY)
        {
            DrawTriangle(
                (Vector2){playerX, playerY - PLAYER_SIZE},
                (Vector2){playerX - PLAYER_SIZE, playerY + PLAYER_SIZE},
                (Vector2){playerX + PLAYER_SIZE, playerY + PLAYER_SIZE},
                GREEN
            );

            for (int i = 0; i < MAX_OBSTACLES; i++)
                if (obstacles[i].active)
                    DrawRectangle(obstacles[i].x, obstacles[i].y,
                                  obstacles[i].width, obstacles[i].height,
                                  RED);

            char buf[32];
            sprintf(buf, "%.1f", timeAlive);
            DrawText(buf, 10, 10, 30, WHITE);
        }
        
        else if (state == STATE_GAME_OVER)
        {
            DrawText("GAME OVER!", 260, 200, 40, RED);

            char buffer[64];
            sprintf(buffer, "Survived: %.1f seconds", timeAlive);
            DrawText(buffer, 245, 260, 25, WHITE);

            DrawText("Press ENTER to return", 230, 330, 25, YELLOW);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
