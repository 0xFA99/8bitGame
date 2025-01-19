#ifndef SPRITE_H
#define SPRITE_H

#include "raylib.h"
#include "state.h"

#define ANIMATION_CAP 102

struct SpriteSheetData
{
    Texture2D spriteTexture;
    Rectangle *frameRects;
    int frameCount;
};

struct AnimationSequence
{
    int startFrameIndex;
    int lastFrameIndex;
    float frameRate;
};

struct AnimationState
{
    struct AnimationSequence animationSequence;
    enum State state;
    enum Direction direction;
};

struct SpriteEntity
{
    struct SpriteSheetData sheetData;
    struct AnimationState *animationStates;
    int animationStateCount;
    float frameDuration;
};

struct SpriteSheetData LoadSpriteSheetData(const char *file, int rows, int columns);
void AddFlipSpritesheetData(struct SpriteSheetData *spritesheet, bool horizontal, bool vertical);
void AddAnimationState(struct SpriteEntity *entity, enum State state, enum Direction direction, int start, int end, int FPS);
void FreeSpriteEntity(struct SpriteEntity *entity);

#endif
