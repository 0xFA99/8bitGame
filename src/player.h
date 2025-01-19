#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "sprite.h"
#include "state.h"

struct PlayerMovement {
    Vector2 position;
    Vector2 velocity;
    float acceleration;
    float speed;
};

struct PlayerStatus {
    enum State state;
    enum Direction direction;
};

struct Player {
    struct SpriteEntity *entity;
    struct AnimationState *animation;
    struct PlayerMovement movement;
    struct PlayerStatus status;
    int currentFrame;
};

struct Player InitPlayer(void);
bool PlayerSetSpriteSheetData(struct Player *player, const char *file, int rows, int columns);

void UpdatePlayer(struct Player *player, float frameTime);
void GetInputPlayer(struct Player *player, float gravity, float frameTime);
void DrawPlayer(struct Player *player);
void SetPlayerAnimation(struct Player *player, enum State state, enum Direction direction);
void FreePlayer(struct Player *player);

#endif
