#include <stdio.h>
#include <stdlib.h>

#include "sprite.h"

struct SpriteSheetData LoadSpriteSheetData(const char *file, int rows, int columns)
{
    struct SpriteSheetData sheet = { 0 };
    sheet.spriteTexture = LoadTexture(file);

    if (sheet.spriteTexture.id)
    {
        sheet.frameCount = rows * columns;

        int width  = sheet.spriteTexture.width / columns;
        int height = sheet.spriteTexture.height / rows;

        sheet.frameRects = (Rectangle *)malloc(sheet.frameCount * sizeof(Rectangle));
        if (sheet.frameRects == NULL) {
            fprintf(stderr, "[ERROR] Failed to allocation memory for Spritesheet frames.\n");
            return sheet;
        }

        int count = 0;

        for (int row = 0; row < rows; row++)
        {
            for (int column = 0; column < columns; column++)
            {
                sheet.frameRects[count].x = column * width;
                sheet.frameRects[count].y = row * height;
                sheet.frameRects[count].width  = width;
                sheet.frameRects[count].height = height;

                count++;
            }
        }

    } else {
        fprintf(stderr, "[ERROR] Failed to load texture: %s\n", file);
        return sheet;
    }

    return sheet;
}

void AddFlipSpritesheetData(struct SpriteSheetData *spritesheet, bool horizontal, bool vertical)
{
    if (spritesheet->frameCount == 0) {
        fprintf(stderr, "[WARNING] Spritesheet is empty!\n");
        return;
    }

    int originalFrames = spritesheet->frameCount;
    int newTotalFrames = originalFrames * 2;

    spritesheet->frameRects = (Rectangle *)realloc(
            spritesheet->frameRects, 
            newTotalFrames * sizeof(Rectangle)
    );

    if (spritesheet->frameRects == NULL) {
        fprintf(stderr, "[ERROR] Failed to allocate memory for flipped frames!\n");
        return;
    }

    for (int i = 0; i < originalFrames; i++) {
        Rectangle rect = spritesheet->frameRects[i];

        spritesheet->frameRects[originalFrames + i].x = rect.x;
        spritesheet->frameRects[originalFrames + i].y = rect.y;

        if (horizontal) spritesheet->frameRects[originalFrames + i].width = -rect.width;
        else spritesheet->frameRects[originalFrames + i].width = rect.width;

        if (vertical) spritesheet->frameRects[originalFrames + i].height = -rect.height;
        else spritesheet->frameRects[originalFrames + i].height = rect.height;
    }

    spritesheet->frameCount = newTotalFrames;
}

void AddAnimationState(struct SpriteEntity *entity, enum State state, enum Direction direction,
                       int start, int end, int FPS)
{
    if (entity->sheetData.frameCount == 0) {
        fprintf(stderr, "[ERROR] SpriteSheet Data not found!\n");
        return;
    }

    if (entity->animationStateCount == 0) {
        entity->animationStates = (struct AnimationState *)malloc(
            sizeof(struct AnimationState)
        );
    } else {
        struct AnimationState *newStates = (struct AnimationState *)realloc(
            entity->animationStates,
            (entity->animationStateCount + 1) * sizeof(struct AnimationState)
        );

        if (newStates == NULL) {
            fprintf(stderr, "[ERROR] Memory reallocation failed!\n");
            return;
        }

        entity->animationStates = newStates;
    }

    struct AnimationState *newAnimation = &entity->animationStates[entity->animationStateCount];
    newAnimation->animationSequence = (struct AnimationSequence){
        .startFrameIndex = start,
        .lastFrameIndex = end,
        .frameRate = FPS
    };
    newAnimation->state = state;
    newAnimation->direction = direction;

    entity->animationStateCount++;
}

