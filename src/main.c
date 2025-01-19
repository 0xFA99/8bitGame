#include <stdio.h>

#include "raylib.h"
#include "sprite.h"
#include "player.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    const float gravity = 500.0f;

    InitWindow(screenWidth, screenHeight, "RPG");

    struct Player player = InitPlayer();

    PlayerSetSpriteSheetData(&player, "assets/warrior.png", 17, 6);

    AddAnimationState(player.entity, STATE_IDLE, DIRECTION_RIGHT, 0, 5, 10);
    AddAnimationState(player.entity, STATE_IDLE, DIRECTION_LEFT, ANIMATION_CAP + 0, ANIMATION_CAP + 5, 10);
    AddAnimationState(player.entity, STATE_RUN, DIRECTION_RIGHT, 6, 13, 10);
    AddAnimationState(player.entity, STATE_RUN, DIRECTION_LEFT, ANIMATION_CAP + 6, ANIMATION_CAP + 11, 10);
    AddAnimationState(player.entity, STATE_JUMP, DIRECTION_RIGHT, 41, 43, 10);
    AddAnimationState(player.entity, STATE_JUMP, DIRECTION_LEFT, ANIMATION_CAP + 41, ANIMATION_CAP + 43, 10);
    AddAnimationState(player.entity, STATE_FALL, DIRECTION_RIGHT, 46, 48, 10);
    AddAnimationState(player.entity, STATE_FALL, DIRECTION_LEFT, ANIMATION_CAP + 46, ANIMATION_CAP + 48, 10);

    SetPlayerAnimation(&player, STATE_IDLE, DIRECTION_RIGHT);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        float frameTime = GetFrameTime();

        GetInputPlayer(&player, gravity, frameTime);

        UpdatePlayer(&player, GetFrameTime());

        BeginDrawing();
            ClearBackground((Color){ 18, 18, 18, 255 });
            DrawPlayer(&player);
        EndDrawing();
    }

    FreePlayer(&player);
    CloseWindow();

    return 0;
}
