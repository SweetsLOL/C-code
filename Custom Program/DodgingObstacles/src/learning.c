#include "raylib.h"

int main(){
    InitWindow(800, 600, "Triangle Test");
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTriangle(
            (Vector2){400, 300 - 20};
        )
    }
}