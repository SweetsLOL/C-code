#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PLAYER_SIZE 20
#define MAX_OBSTACLES 150
#define MAX_POWERUPS 10
#define TARGET_FPS 60

typedef enum {
    OBSTACLE_NORMAL,
    OBSTACLE_FAST,
    OBSTACLE_WIDE,
    OBSTACLE_SMALL,
    OBSTACLE_DIAGONAL,
} ObstacleType;

typedef enum {
    STATE_TITLE,
    STATE_STAGE_SELECT,
    STATE_GAMEPLAY,
    STATE_GAME_OVER
} GameState;

typedef enum {
    STAGE_NONE,
    STAGE_EASY,
    STAGE_MEDIUM,
    STAGE_HARD
} Stage;

typedef enum {
    POWER_SHIELD,
    POWER_INVERT
} PowerType;

typedef struct {
    float x, y;
    float width, height;
    float speed_y;
    bool active;
    float speed_x;
    ObstacleType type;
} Obstacle;

typedef struct {
    float x, y;
    float radius;
    float speed;
    bool active;
    PowerType type;
} PowerUp;

// Load high score from file
float LoadHighScore(char *filename) {
    FILE *ftpr = fopen(filename, "r");
    float score = 0.0f;
    if (ftpr) {
        fscanf(ftpr, "%f", &score);
        fclose(ftpr);
    }
    return score;
}

// Save high score to file
void SaveHighScore(char *filename, float newScore) {
    FILE *ftpr = fopen(filename, "w");
    if (ftpr) {
        fprintf(ftpr, "%.1f", newScore);
        fclose(ftpr);
    }
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dodge!");
    SetTargetFPS(TARGET_FPS);

    GameState gameState = STATE_TITLE;
    Stage currentStage = STAGE_NONE;

    float playerX = SCREEN_WIDTH / 2;
    float playerY = SCREEN_HEIGHT - 50;
    float playerSpeed = 400;

    Obstacle obstacles[MAX_OBSTACLES] = {0};
    float spawnTimer = 0;
    float spawnRate = 0.6f;

    bool playerAlive = true;
    float survivalTime = 0;
    float highScore = 0;
    char highscoreFile[64] = "highscore_easy.txt";

    float highScoreEasy = LoadHighScore("highscore_easy.txt");
    float highScoreMedium = LoadHighScore("highscore_medium.txt");
    float highScoreHard = LoadHighScore("highscore_hard.txt");


    PowerUp powerups[MAX_POWERUPS] = {0};
    float powerSpawnTimer = 0;
    float powerSpawnRate = 8;

    bool shieldActive = false;
    float shieldTimer = 0;

    bool invertedActive = false;
    float invertedTimer = 0;

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // ===== TITLE SCREEN =====
        if (gameState == STATE_TITLE)
        {
            if (IsKeyPressed(KEY_ENTER)) 
                gameState = STATE_STAGE_SELECT;
        }

        // ===== STAGE SELECT =====
        else if (gameState == STATE_STAGE_SELECT)
        {
            if (IsKeyPressed(KEY_ONE)) { 
                spawnRate = 0.8f; 
                sprintf(highscoreFile, "highscore_easy.txt");
                highScore = LoadHighScore(highscoreFile);
                gameState = STATE_GAMEPLAY;
            }
            if (IsKeyPressed(KEY_TWO)) { 
                spawnRate = 0.5f; 
                sprintf(highscoreFile, "highscore_medium.txt");
                highScore = LoadHighScore(highscoreFile);
                gameState = STATE_GAMEPLAY;
            }
            if (IsKeyPressed(KEY_THREE)) { 
                spawnRate = 0.3f; 
                sprintf(highscoreFile, "highscore_hard.txt");
                highScore = LoadHighScore(highscoreFile);
                gameState = STATE_GAMEPLAY;
            }

            if (gameState == STATE_GAMEPLAY)
            {
                playerAlive = true;
                survivalTime = 0;
                playerX = SCREEN_WIDTH / 2;
                shieldActive = false;
                invertedActive = false;
                spawnTimer = 0;
                powerSpawnTimer = 0;
                
                for (int i = 0; i < MAX_POWERUPS; i++) 
                    powerups[i].active = false;

                for (int i = 0; i < MAX_OBSTACLES; i++)
                    obstacles[i].active = false;
            }
        }

        // ===== GAMEPLAY =====
        else if (gameState == STATE_GAMEPLAY && playerAlive)
        {
            survivalTime += deltaTime;
            float difficultyMultiplier = 1.0f + (survivalTime / 30.0f);
            if (difficultyMultiplier > 3.0f) difficultyMultiplier = 3.0f;

            // Player movement (inverted controls if inverted debuff is active)
            float newPlayerX = playerX;
            if (!invertedActive) {
                if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  
                    newPlayerX -= playerSpeed * deltaTime;
                if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) 
                    newPlayerX += playerSpeed * deltaTime;
            } else {
                if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  
                    newPlayerX += playerSpeed * deltaTime;
                if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) 
                    newPlayerX -= playerSpeed * deltaTime;
            }
            
            // Prevent player from moving out of bounds
            if (newPlayerX < PLAYER_SIZE) 
                newPlayerX = PLAYER_SIZE;
            if (newPlayerX > SCREEN_WIDTH - PLAYER_SIZE) 
                newPlayerX = SCREEN_WIDTH - PLAYER_SIZE;

            // Triangle vertices
            Vector2 playerTip   = { playerX, playerY - PLAYER_SIZE };
            Vector2 playerLeft  = { playerX - PLAYER_SIZE, playerY + PLAYER_SIZE };
            Vector2 playerRight = { playerX + PLAYER_SIZE, playerY + PLAYER_SIZE };
            Vector2 playerCenter = { playerX, playerY }; // for power-up checks

            // ===== SPAWN OBSTACLES =====
            spawnTimer += deltaTime;
            float adjustedSpawnRate = spawnRate / (difficultyMultiplier * 1.33);
            
            if (spawnTimer >= adjustedSpawnRate)
            {
                spawnTimer = 0;
                for (int i = 0; i < MAX_OBSTACLES; i++)
                {
                    if (!obstacles[i].active)
                    {
                        obstacles[i].active = true; 
                        obstacles[i].x = GetRandomValue(0, SCREEN_WIDTH - 50);
                        obstacles[i].y = -50;
                        obstacles[i].type = GetRandomValue(0, 3);
                        obstacles[i].width = 65;
                        obstacles[i].height = 20;
                        obstacles[i].speed_y = 230 * difficultyMultiplier;

                        switch (obstacles[i].type) {
                            case OBSTACLE_FAST:  
                                obstacles[i].speed_y *= 1.5f; 
                                break;
                            case OBSTACLE_WIDE:  
                                obstacles[i].width *= 1.8f; 
                                break;
                            case OBSTACLE_SMALL: 
                                obstacles[i].width *= 0.6f; 
                                obstacles[i].height *= 0.6f; 
                                break;
                            default: 
                                break;
                        }
                        break;
                    }
                }
                // ===== SPAWN OBSTACLES ===== (for hard mode, adds diagonally moving blocks)
                if (spawnRate == 0.3f && GetRandomValue(0, 4) == 0) { //20% chance for diagonally moving block to spawn
                    for (int j = 0; j < MAX_OBSTACLES; j++) {
                        if (!obstacles[j].active) {
                            obstacles[j].active = true;
                            obstacles[j].x = GetRandomValue(0, SCREEN_WIDTH - 50);
                            obstacles[j].y = -50;
                            obstacles[j].width = 50;
                            obstacles[j].height = 20;
                            obstacles[j].speed_y = 200 * difficultyMultiplier;
                            obstacles[j].speed_x = (GetRandomValue(0, 1) == 0) ? 150 : -150;
                            obstacles[j].type = OBSTACLE_DIAGONAL;
                            break;
                        }
                    }
                }
            }

            // ===== SPAWN POWERUPS =====
            powerSpawnTimer += deltaTime;
            if (powerSpawnTimer >= powerSpawnRate) {
                powerSpawnTimer = 0;
                powerSpawnRate = GetRandomValue(4, 8);
                for (int i = 0; i < MAX_POWERUPS; i++) {
                    if (!powerups[i].active) {
                        powerups[i].active = true;
                        powerups[i].x = GetRandomValue(50, SCREEN_WIDTH - 50);
                        powerups[i].y = -30;
                        powerups[i].radius = 10;
                        powerups[i].speed = 120;
                        powerups[i].type = GetRandomValue(0, 1);
                        break;
                    }
                }
            }

            // ===== MOVE OBJECTS =====
            for (int i = 0; i < MAX_OBSTACLES; i++) {
              if (obstacles[i].active) {
                    if (obstacles[i].type == OBSTACLE_DIAGONAL) {
                        obstacles[i].x += obstacles[i].speed_x * deltaTime;
                        obstacles[i].y += obstacles[i].speed_y * deltaTime;

                        // Bounce off screen edges
                        if (obstacles[i].x <= 0 || obstacles[i].x + obstacles[i].width >= SCREEN_WIDTH)
                            obstacles[i].speed_x *= -1;
                    } else {
                        obstacles[i].y += obstacles[i].speed_y * deltaTime;
                    }
                    
                    if (obstacles[i].y > SCREEN_HEIGHT)
                        obstacles[i].active = false;
                }
            }

            for (int i = 0; i < MAX_POWERUPS; i++) {
                if (powerups[i].active) {
                    powerups[i].y += powerups[i].speed * deltaTime;
                    if (powerups[i].y > SCREEN_HEIGHT)
                        powerups[i].active = false;
                }
            }

            // ===== COLLISION DETECTION =====
            bool playerBlocked = false;

            for (int i = 0; i < MAX_OBSTACLES; i++) {
                if (!obstacles[i].active) continue;

                Rectangle obstacleRect = {
                    obstacles[i].x,
                    obstacles[i].y,
                    obstacles[i].width,
                    obstacles[i].height
                };

                // === TIP COLLISION (lethal) ===
                if (CheckCollisionPointRec(playerTip, obstacleRect)) {
                    if (shieldActive) {
                        shieldActive = false;
                        obstacles[i].active = false; // destroy obstacle if shielded
                    } else {
                        playerAlive = false;
                        gameState = STATE_GAME_OVER;
                        if (survivalTime > highScore) {
                            highScore = survivalTime;
                            SaveHighScore(highscoreFile, highScore);

                            if (strcmp(highscoreFile, "highscore_easy.txt") == 0)
                                highScoreEasy = highScore;
                            else if (strcmp(highscoreFile, "highscore_medium.txt") == 0)
                                highScoreMedium = highScore;
                            else if (strcmp(highscoreFile, "highscore_hard.txt") == 0)
                                highScoreHard = highScore;
                        }
                    }
                    break;
                }

                // === SIDE COLLISION (blocking only, non-lethal) ===
                Vector2 collisionPoint = {0};
                Vector2 topLeft     = { obstacleRect.x, obstacleRect.y };
                Vector2 topRight    = { obstacleRect.x + obstacleRect.width, obstacleRect.y };
                Vector2 bottomLeft  = { obstacleRect.x, obstacleRect.y + obstacleRect.height };
                Vector2 bottomRight = { obstacleRect.x + obstacleRect.width, obstacleRect.y + obstacleRect.height };

                bool leftSideHit = 
                    CheckCollisionLines(playerTip, playerLeft, topLeft, topRight, &collisionPoint) ||
                    CheckCollisionLines(playerTip, playerLeft, topRight, bottomRight, &collisionPoint) ||
                    CheckCollisionLines(playerTip, playerLeft, bottomRight, bottomLeft, &collisionPoint) ||
                    CheckCollisionLines(playerTip, playerLeft, bottomLeft, topLeft, &collisionPoint);

                bool rightSideHit = 
                    CheckCollisionLines(playerTip, playerRight, topLeft, topRight, &collisionPoint) ||
                    CheckCollisionLines(playerTip, playerRight, topRight, bottomRight, &collisionPoint) ||
                    CheckCollisionLines(playerTip, playerRight, bottomRight, bottomLeft, &collisionPoint) ||
                    CheckCollisionLines(playerTip, playerRight, bottomLeft, topLeft, &collisionPoint);

                if (leftSideHit || rightSideHit) {
                    playerBlocked = true;
                    break;
                }
            }

            if (!playerBlocked)
            playerX = newPlayerX;

            // ===== POWER-UP COLLECTION =====
            for (int i = 0; i < MAX_POWERUPS; i++) {
                if (!powerups[i].active) continue;

                if (CheckCollisionCircles(playerCenter, PLAYER_SIZE, 
                                          (Vector2){ powerups[i].x, powerups[i].y }, 
                                          powerups[i].radius)) {
                    if (powerups[i].type == POWER_SHIELD) {
                        shieldActive = true;
                        shieldTimer = 8.0f;
                    } else if (powerups[i].type == POWER_INVERT) {
                        invertedActive = true;
                        invertedTimer = 5.0f;
                    }
                    powerups[i].active = false;
                }
            }

            // ===== POWER-UP TIMERS =====
            if (shieldActive) {
                shieldTimer -= deltaTime;
                if (shieldTimer <= 0) { shieldActive = false; shieldTimer = 0; }
            }
            if (invertedActive) {
                invertedTimer -= deltaTime;
                if (invertedTimer <= 0) { invertedActive = false; invertedTimer = 0; }
            }

        }

        // ===== GAME OVER =====
        else if (gameState == STATE_GAME_OVER)
        {
            if (IsKeyPressed(KEY_ENTER))
                gameState = STATE_STAGE_SELECT;
        }

        // ===== DRAW EVERYTHING =====
        BeginDrawing();
        ClearBackground(BLACK);

        if (gameState == STATE_TITLE)
        {
            DrawText("DODGE!", 310, 200, 40, WHITE);
            DrawText("Press ENTER", 310, 300, 20, YELLOW);
        }
        else if (gameState == STATE_STAGE_SELECT)
        {
            char selectEasy[64];
            char selectMedium[64];
            char selectHard[64];
            sprintf(selectEasy, "1 - Easy   (High Score: %.1fs)", highScoreEasy);
            sprintf(selectMedium, "2 - Medium   (High Score: %.1fs)", highScoreMedium);
            sprintf(selectHard, "3 - Hard   (High Score: %.1fs)", highScoreHard);

            DrawText("SELECT STAGE", 260, 120, 40, WHITE);
            DrawText(selectEasy,   220, 220, 30, GREEN);
            DrawText(selectMedium, 220, 270, 30, YELLOW);
            DrawText(selectHard,   220, 320, 30, RED);
        }
        else if (gameState == STATE_GAMEPLAY)
        {
            DrawTriangle(
                (Vector2){playerX, playerY - PLAYER_SIZE},
                (Vector2){playerX - PLAYER_SIZE, playerY + PLAYER_SIZE},
                (Vector2){playerX + PLAYER_SIZE, playerY + PLAYER_SIZE},
                GREEN
            );

            if (shieldActive)
                DrawCircleLines(playerX, playerY, PLAYER_SIZE + 10, SKYBLUE);

            if (invertedActive)
                DrawText("INVERTED!", SCREEN_WIDTH - 170, 10, 25, RED);

            if (shieldActive) {
                char shieldText[32];
                sprintf(shieldText, "Shield: %.1fs", shieldTimer);
                DrawText(shieldText, 10, 50, 25, SKYBLUE);
            }

            if (invertedActive) {
                char invertText[32];
                sprintf(invertText, "Invert: %.1fs", invertedTimer);
                DrawText(invertText, 10, 80, 25, RED);
            }

            for (int i = 0; i < MAX_OBSTACLES; i++)
                if (obstacles[i].active){
                    Color color = RED;
                    switch (obstacles[i].type)
                    {
                        case OBSTACLE_FAST:  color = ORANGE; break;
                        case OBSTACLE_WIDE:  color = BLUE;   break;
                        case OBSTACLE_SMALL: color = PURPLE; break;
                        case OBSTACLE_DIAGONAL: color = DARKGREEN; break;
                        default: color = RED; break;
                    }
                    DrawRectangle(obstacles[i].x, obstacles[i].y,
                                  obstacles[i].width, obstacles[i].height,
                                  color);
                }

            for (int i = 0; i < MAX_POWERUPS; i++) {
                if (!powerups[i].active) continue;
                Color color = (powerups[i].type == POWER_SHIELD) ? SKYBLUE : RED;
                DrawCircle(powerups[i].x, powerups[i].y, powerups[i].radius, color);
                DrawCircleLines(powerups[i].x, powerups[i].y, powerups[i].radius + 2, WHITE);
            }

            char timeText[32];
            sprintf(timeText, "%.1f", survivalTime);
            DrawText(timeText, 10, 10, 30, WHITE);
        }
        else if (gameState == STATE_GAME_OVER)
        {
            DrawText("GAME OVER!", 260, 200, 40, RED);
            char resultText[64];
            sprintf(resultText, "Survived: %.1f seconds", survivalTime);
            DrawText(resultText, 245, 260, 25, WHITE);

            char highScoreText[64];
            sprintf(highScoreText, "High Score: %.1f", highScore);
            DrawText(highScoreText, 260, 300, 25, ORANGE);

            DrawText("Press ENTER to return", 230, 330, 25, YELLOW);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
