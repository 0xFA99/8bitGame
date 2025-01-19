#include "player.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "sprite.h"
#include "raymath.h"

struct Player InitPlayer()
{
    struct Player player;

    player.entity = (struct SpriteEntity *)malloc(sizeof(struct SpriteEntity));
    if (player.entity == NULL) {
        fprintf(stderr, "[ERROR] Failed to allocate memory for init Player Entity.\n");
        return player;
    }

    player.animation = NULL;
    player.movement = (struct PlayerMovement){
        .position = (Vector2){ 0.0f, 255.0f },
        .velocity = (Vector2){ 0.0f, 0.0f },
        .acceleration = -200.0f,
        .speed = 100.0f
    };

    player.status = (struct PlayerStatus){
        .state = STATE_IDLE,
        .direction = DIRECTION_RIGHT
    };

    return player;
}

bool PlayerSetSpriteSheetData(struct Player *player, const char *file, int rows, int columns)
{
    
    struct SpriteEntity *entity = player->entity;
    struct SpriteSheetData spriteSheet = LoadSpriteSheetData(file, rows, columns);

    AddFlipSpritesheetData(&spriteSheet, true, false);

    entity->sheetData = spriteSheet;

    return true;
}

void SetPlayerAnimation(struct Player *player, enum State state, enum Direction direction)
{
    for (int i = 0; i < player->entity->animationStateCount; i++) {

        struct AnimationState animationState = player->entity->animationStates[i];

        if (animationState.state == state && animationState.direction == direction) {
            player->animation = &player->entity->animationStates[i];
            player->currentFrame = player->entity->animationStates[i].animationSequence.startFrameIndex;
            player->status.state = state;
            player->status.direction = direction;
        }
    }
}

void UpdatePlayer(struct Player *player, float frameTime)
{

    {   // Check and Set new AnimationState
        struct PlayerStatus status = player->status;
        struct AnimationState *animation = player->animation;

        if (status.state != animation->state || status.direction != animation->direction)
            SetPlayerAnimation(player, status.state, status.direction);
    }

    player->movement.position = (Vector2) {
        player->movement.position.x + player->movement.velocity.x * frameTime,
        player->movement.position.y + player->movement.velocity.y * frameTime
    };

    float *frameDuration = &player->entity->frameDuration;
    float playerAnimFPS = player->animation->animationSequence.frameRate;

    *frameDuration += frameTime;

    if (*frameDuration >= 1.0f / playerAnimFPS) {
        *frameDuration = 0.0f;
        player->currentFrame++;

        if (player->currentFrame > player->animation->animationSequence.lastFrameIndex)
            player->currentFrame = player->animation->animationSequence.startFrameIndex;
    }
}

void DrawPlayer(struct Player *player)
{
    Rectangle frame = player->entity->sheetData.frameRects[player->currentFrame];

    Rectangle position = (Rectangle){
        player->movement.position.x, player->movement.position.y,
        frame.width, frame.height
    };

    Vector2 offset = (Vector2){ 0.0f, 0.0f };

    DrawTexturePro(player->entity->sheetData.spriteTexture, frame, position, offset, 0, WHITE);
}

void GetInputPlayer(struct Player *player, float gravity, float frameTime)
{
    int runDir = 0;
    bool isJump = IsKeyDown(KEY_UP);

    if (IsKeyDown(KEY_RIGHT)) runDir = 1;
    if (IsKeyDown(KEY_LEFT)) runDir = -1;

    switch (player->status.state) {
        case STATE_IDLE:
        {
            if (isJump) {
                player->status.state = STATE_JUMP;
                player->movement.velocity.y = player->movement.acceleration;
            } else if (runDir != 0) {
                player->status.state = STATE_RUN;
                player->status.direction = runDir > 0;
                player->movement.velocity.x = runDir * player->movement.speed;
            } else {
                player->movement.velocity.x = 0.0f;
                player->movement.velocity.y = 0.0f;
            }
        }
        break;

        case STATE_RUN:
        {
            if (isJump) {
                player->status.state = STATE_JUMP;
                player->movement.velocity.y = player->movement.acceleration;
            } else if (runDir == 0) {
                player->status.state = STATE_IDLE;
                player->movement.velocity.x = 0.0f;
            } else {
                player->status.direction = runDir > 0;
                player->movement.velocity.x = runDir * player->movement.speed;
            }
        }
        break;

        case STATE_JUMP:
        case STATE_FALL:
        {
            if (runDir != 0) {
                player->status.direction = runDir > 0;
                player->movement.velocity.x = runDir * player->movement.speed;
            } else {
                player->movement.velocity.x = runDir * player->movement.speed * frameTime;
            }

            if (player->movement.velocity.y >= 0 && player->status.state == STATE_JUMP) {
                player->status.state = STATE_FALL;
            }
        }
        break;
    }

    player->movement.velocity.y += gravity * frameTime;

    Vector2 newPos = Vector2Add(
        player->movement.position,
        Vector2Scale(player->movement.velocity, frameTime)
    );

    if (newPos.y > 255.0f) {
        newPos.y = 255.0f;
        player->movement.velocity.y = 0.0f;
        if (player->status.state == STATE_FALL || player->status.state == STATE_JUMP) {
            player->status.state = STATE_IDLE;
        }
    }

    player->movement.position = newPos;
}

void FreePlayer(struct Player *player)
{
    if (player == NULL) return;

    free(player->entity->sheetData.frameRects);
    free(player->entity->animationStates);
    free(player->entity);

    player->entity = NULL;
    player->animation = NULL;
}
