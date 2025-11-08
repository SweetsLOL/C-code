#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

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
    OBSTACLE_COLORFUL
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
    float speed;
    bool active;
    ObstacleType type;
} Obstacle;

typedef struct {
    float x, y;
    float radius;
    float speed;
    bool active;
    PowerType type;
} PowerUp;


// Check if a point is inside a rectangle
bool PointInRect(Vector2 point, Rectangle rect) {
    return (point.x >= rect.x && point.x <= rect.x + rect.width &&
            point.y >= rect.y && point.y <= rect.y + rect.height);
}

// Check if two line segments intersect
bool SegmentsIntersect(Vector2 startA, Vector2 endA, Vector2 startB, Vector2 endB)
{
    float denominator =
        (endB.y - startB.y) * (endA.x - startA.x) -
        (endB.x - startB.x) * (endA.y - startA.y);

    if (denominator == 0) return false;

    float u =
        ((startB.x - startA.x) * (endA.y - startA.y) -
         (startB.y - startA.y) * (endA.x - startA.x)) / denominator;

    float v =
        ((startB.x - startA.x) * (endB.y - startB.y) -
         (startB.y - startA.y) * (endB.x - startB.x)) / denominator;

    return (u >= 0 && u <= 1 && v >= 0 && v <= 1);
}

// Check if a line segment intersects any edge of a rectangle
bool SegmentVsRect(Vector2 start, Vector2 end, Rectangle rect)
{
    Vector2 rectTopLeft     = {rect.x, rect.y};
    Vector2 rectTopRight    = {rect.x + rect.width, rect.y};
    Vector2 rectBottomRight = {rect.x + rect.width, rect.y + rect.height};
    Vector2 rectBottomLeft  = {rect.x, rect.y + rect.height};

    if (SegmentsIntersect(start, end, rectTopLeft, rectTopRight)) return true;
    if (SegmentsIntersect(start, end, rectTopRight, rectBottomRight)) return true;
    if (SegmentsIntersect(start, end, rectBottomRight, rectBottomLeft)) return true; 
    if (SegmentsIntersect(start, end, rectBottomLeft, rectTopLeft)) return true;

    return false;
}

// Load high score from file to display
float LoadHighScore(char *filename) {
    FILE *ftpr = fopen(filename, "r");
    float score = 0.0f;
    if (ftpr) {
        fscanf(ftpr, "%f", &score);
        fclose(ftpr);
    }
    return score;
}

// Save new high score to file
void SaveHighScore(char *filename, float newScore) {
    FILE *ftpr = fopen(filename, "w");
    if (ftpr) {
        fprintf(ftpr, "%.1f", newScore);
        fclose(ftpr);
    }
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Triangle Dodge");
    SetTargetFPS(TARGET_FPS);

    GameState gameState = STATE_TITLE;
    Stage currentStage = STAGE_NONE;

    float playerX = SCREEN_WIDTH / 2;
    float playerY = SCREEN_HEIGHT - 80;
    float playerSpeed = 400;

    Obstacle obstacles[MAX_OBSTACLES] = {0};
    float spawnTimer = 0;
    float spawnRate = 0.6f;

    bool playerAlive = true;
    float survivalTime = 0;
    float highScore = 0;
    char highscoreFile[64] = "highscore_easy.txt";
    
    PowerUp powerups[MAX_POWERUPS] = {0};
    float powerSpawnTimer = 0;
    float powerSpawnRate = 8.0f;

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
                gameState = STATE_GAMEPLAY; 
                sprintf(highscoreFile, "highscore_easy.txt");
                highScore = LoadHighScore(highscoreFile);
                gameState = STATE_GAMEPLAY;
            }
            if (IsKeyPressed(KEY_TWO)) { 
                spawnRate = 0.5f; 
                gameState = STATE_GAMEPLAY; 
                sprintf(highscoreFile, "highscore_medium.txt");
                highScore = LoadHighScore(highscoreFile);
                gameState = STATE_GAMEPLAY;
            }
            if (IsKeyPressed(KEY_THREE)) { 
                spawnRate = 0.3f; 
                gameState = STATE_GAMEPLAY;
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

                // Reset spawn timers for powerups and obstacles
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
            if (difficultyMultiplier > 3.0f) 
                difficultyMultiplier = 3.0f; // cap max difficulty

            // Predict next player X position               
            float newPlayerX = playerX;
            if (invertedActive != true){
                if (IsKeyDown(KEY_LEFT))  newPlayerX -= playerSpeed * deltaTime;
                if (IsKeyDown(KEY_RIGHT)) newPlayerX += playerSpeed * deltaTime;
            }
            else {
                if (IsKeyDown(KEY_LEFT))  newPlayerX += playerSpeed * deltaTime;
                if (IsKeyDown(KEY_RIGHT)) newPlayerX -= playerSpeed * deltaTime;
            }
            
            if (newPlayerX < PLAYER_SIZE) newPlayerX = PLAYER_SIZE;
            if (newPlayerX > SCREEN_WIDTH - PLAYER_SIZE) newPlayerX = SCREEN_WIDTH - PLAYER_SIZE;

            // Current triangle vertices
            Vector2 playerTip   = { playerX, playerY - PLAYER_SIZE };
            Vector2 playerLeft  = { playerX - PLAYER_SIZE, playerY + PLAYER_SIZE };
            Vector2 playerRight = { playerX + PLAYER_SIZE, playerY + PLAYER_SIZE };

            // Predicted next-frame triangle vertices
            Vector2 nextTip   = { newPlayerX, playerY - PLAYER_SIZE };
            Vector2 nextLeft  = { newPlayerX - PLAYER_SIZE, playerY + PLAYER_SIZE };
            Vector2 nextRight = { newPlayerX + PLAYER_SIZE, playerY + PLAYER_SIZE };

            // Spawn new obstacles
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

                        obstacles[i].type = GetRandomValue(0, 4);

                        obstacles[i].width = 65;
                        obstacles[i].height = 20;
                        obstacles[i].speed = 230 * difficultyMultiplier;

                        switch (obstacles[i].type) {
                            case OBSTACLE_FAST:
                                obstacles[i].speed *= 1.5f;
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
            }

            powerSpawnTimer += deltaTime;
            if (powerSpawnTimer >= powerSpawnRate) {
                powerSpawnTimer = 0;
                for (int i = 0; i < MAX_POWERUPS; i++) {
                    if (!powerups[i].active) {
                        powerups[i].active = true;
                        powerups[i].x = GetRandomValue(50, SCREEN_WIDTH - 50);
                        powerups[i].y = -30;
                        powerups[i].radius = 10;
                        powerups[i].speed = 120;
                        powerups[i].type = GetRandomValue(0, 1); // 0=shield,1=invert
                        break;
                    }
                }
            }

            // Move active obstacles
            for (int i = 0; i < MAX_OBSTACLES; i++)
            {
                if (!obstacles[i].active) 
                    continue;

                obstacles[i].y += obstacles[i].speed * deltaTime;
                if (obstacles[i].y > SCREEN_HEIGHT)
                    obstacles[i].active = false;
            }

            // Move active powerups
            for (int i = 0; i < MAX_POWERUPS; i++) {
                if (!powerups[i].active) continue;
                powerups[i].y += powerups[i].speed * deltaTime;
                if (powerups[i].y > SCREEN_HEIGHT)
                    powerups[i].active = false;
            }

            // Collision detection
            bool playerBlocked = false;

            for (int i = 0; i < MAX_OBSTACLES; i++)
            {
                if (!obstacles[i].active) 
                    continue;

                Rectangle obstacleRect = {
                    obstacles[i].x,
                    obstacles[i].y,
                    obstacles[i].width,
                    obstacles[i].height
                };

                // Tip collision = death
                if (PointInRect(playerTip, obstacleRect)) {
                    if (shieldActive) {
                        shieldActive = false; // use up shield
                        obstacles[i].active = false; // remove the obstacle hit
                        continue;
                    } else {
                        playerAlive = false;
                        gameState = STATE_GAME_OVER;
                    
                        if (survivalTime > highScore) {
                            highScore = survivalTime;
                            SaveHighScore(highscoreFile, highScore);
                        }
                    }
                    break;
                }

                // Side or bottom collision = block
                if (SegmentVsRect(nextTip, nextLeft, obstacleRect) ||
                    SegmentVsRect(nextTip, nextRight, obstacleRect) ||
                    SegmentVsRect(nextLeft, nextRight, obstacleRect))
                {
                    playerBlocked = true;
                    break;
                }
            }
            
            // Power up collision
            for (int i = 0; i < MAX_POWERUPS; i++) {
                if (!powerups[i].active) continue;
                float dx = playerTip.x - powerups[i].x;
                float dy = playerTip.y - powerups[i].y;
                if (dx * dx + dy * dy <= (powerups[i].radius + 5) * (powerups[i].radius + 5)) {
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

            // Power up timers
            if (shieldActive) {
                shieldTimer -= deltaTime;
                if (shieldTimer <= 0){
                    shieldTimer = 0; 
                    shieldActive = false;
                }
            }
            if (invertedActive) {
                invertedTimer -= deltaTime;
                if (invertedTimer <= 0){
                    invertedActive = 0;
                    invertedActive = false;
                }
            }

            // Move only if not blocked
            if (!playerBlocked)
                playerX = newPlayerX;

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
            DrawText("TRIANGLE DODGE", 240, 200, 40, WHITE);
            DrawText("Press ENTER", 310, 300, 20, YELLOW);
        }
        else if (gameState == STATE_STAGE_SELECT)
        {
            DrawText("SELECT STAGE", 260, 120, 40, WHITE);
            DrawText("1 - Easy",   330, 220, 30, GREEN);
            DrawText("2 - Medium", 330, 270, 30, YELLOW);
            DrawText("3 - Hard",   330, 320, 30, RED);
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

            //Power-up timer display
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
                        case OBSTACLE_FAST:      
                            color = ORANGE; break;
                        case OBSTACLE_WIDE:      
                            color = BLUE;   break;
                        case OBSTACLE_SMALL:     
                            color = PURPLE; break;
                        default: 
                            color = RED; 
                            break;
                    }

                    DrawRectangle(obstacles[i].x, obstacles[i].y,
                                  obstacles[i].width, obstacles[i].height,
                                  color);

                }

            for (int i = 0; i < MAX_POWERUPS; i++) {
                if (!powerups[i].active) 
                    continue;
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
