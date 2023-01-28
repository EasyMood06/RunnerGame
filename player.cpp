#include "player.h"

Player::Player(AnimData *data, int window_height, Texture2D *texture2D)
{
    animData = data;
    windowHeight = window_height;
    character = texture2D;
}

bool Player::isOnGround()
{
    return animData->pos.y >= windowHeight - animData->rec.height;
}

void Player::updateAnimData(float deltaTime, int maxFrame)
{
    animData->runningTime += deltaTime;
    if(animData->runningTime >= animData->updateTime)
    {
        animData->runningTime = 0.0f;
        animData->rec.x = animData->frame * animData->rec.width;
        animData->frame++;
        if(animData->frame > maxFrame)
        {
            animData->frame = 0;
        }
    }
}

void Player::updatePlayerPosition(int velocity, float deltaTime)
{
    animData->pos.y += velocity * deltaTime;
}