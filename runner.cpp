/*
**  Game Title: Ninja Jump
**
**  A classic tap tap game created by Qikun Xu
**
**  Rendered by raylib library
*/

#include "raylib.h"
#include "stdio.h"
#include "AnimData.h"
#include "player.h"

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 600

void updateAnimData(AnimData *data, float deltaTime, int maxFrame)
{
    // update running time
    data->runningTime += deltaTime;
    if (data->runningTime >= data->updateTime)
    {
        data->runningTime = 0.0;
        // update animation frame
        data->rec.x = data->frame * data->rec.width;
        data->frame++;
        if (data->frame > maxFrame)
        {
            data->frame = 0;
        }
    }
}

int main()
{
    // initialize the window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tap Dasher!");

    // acceleration due to gravity (pixels/s)/s
    const int gravity{1'000};

    // obstacle_texture variables
    Texture2D obstacle_texture = LoadTexture("textures/12_nebula_spritesheet.png");
    // initialize obstacles
    const int sizeOfObstacle{50};
    AnimData obstacle_data[sizeOfObstacle];
    for (int i = 0; i < sizeOfObstacle; i++)
    {
        obstacle_data[i].rec.x = 0.0;
        obstacle_data[i].rec.y = 0.0;
        obstacle_data[i].rec.width = obstacle_texture.width/8;
        obstacle_data[i].rec.height = obstacle_texture.height/8;
        obstacle_data[i].pos.y = WINDOW_HEIGHT - obstacle_texture.height/8;
        obstacle_data[i].frame = 0;
        obstacle_data[i].runningTime = 0.0;
        obstacle_data[i].updateTime = 0.0;
        obstacle_data[i].pos.x =  WINDOW_WIDTH + i * 300;
    }
    // initialize finish line
    float finishLine{ obstacle_data[sizeOfObstacle - 1].pos.x };

    // obstacle_texture X velocity (pixels/second)
    int obstacleVelocity{-200};

    // scarfy variables
    Texture2D player_texture = LoadTexture("textures/scarfy.png");
    AnimData playerData = {
        {0, 0, player_texture.width/6, player_texture.height},
        {WINDOW_WIDTH/2 - playerData.rec.width/2, WINDOW_HEIGHT - playerData.rec.height},
        0,
        1.0/12.0,
        0.0
    };
    Player player(&playerData, WINDOW_HEIGHT, &player_texture);

    // check play status
    bool isInAir{};
    int vertical_velocity{0};
    bool isDead{};

    Texture2D background = LoadTexture("art_contents/PNG/game_background_1/game_background_1.png");
    float bgX{};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // delta time (time since last frame)
        const float dT{GetFrameTime()};

        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // Scroll background
        bgX -= 80 * dT;
        if (bgX <= -background.width*0.3)
        {
            bgX = 0.0;
        }

        // draw the background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 0.3, WHITE);
        Vector2 bg2Pos{bgX + background.width * 0.3, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 0.3, WHITE);

        // perform ground check
        if (player.isOnGround())
        {
            // rectangle is on the ground
            vertical_velocity = 0;
            isInAir = false;
        }
        else
        {
            // rectangle is in the air
            vertical_velocity += gravity * dT;
            isInAir = true;
        }

        // jump check
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            vertical_velocity += player.getJumpVel();
        }

        for (int i = 0; i < sizeOfObstacle; i++)
        {
            // update the position of each obstacle_texture
            obstacle_data[i].pos.x += obstacleVelocity * dT;
        }

        // update finishLine
        finishLine += obstacleVelocity * dT;

        // update player_texture position
        player.updatePlayerPosition(vertical_velocity, dT);

        // update player_texture's animation frame
        if (!isInAir)
        {
            player.updateAnimData(dT, 5);
        }

        for (int i = 0; i < sizeOfObstacle; i++)
        {
            updateAnimData(&obstacle_data[i], dT, 7);
        }

        // check isDead
        for (AnimData nebula : obstacle_data)
        {
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };
            Rectangle scarfyRec{
                playerData.pos.x,
                playerData.pos.y,
                playerData.rec.width,
                playerData.rec.height
            };
            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                isDead = true;
            }
        }

        if (isDead)
        {
            // lose the game
            DrawText("Game Over!", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 40, RED);
        }
        else if (playerData.pos.x >= finishLine)
        {
            // win the game
            DrawText("You Win!", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 40, RED);
        }
        else
        {
            for (int i = 0; i < sizeOfObstacle; i++)
            {
                // draw obstacle_texture
                DrawTextureRec(obstacle_texture, obstacle_data[i].rec, obstacle_data[i].pos, WHITE);
            }

            // draw player_texture
            DrawTextureRec(player_texture, playerData.rec, playerData.pos, WHITE);
        }

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(player_texture);
    UnloadTexture(obstacle_texture);
    UnloadTexture(background);
    CloseWindow();
}