#ifndef PLAYER_H
#define PLAYER_H

#include "AnimData.h"

class Player
{
public:
    Player(AnimData *data, int window_height, Texture2D *texture2D);
    bool isOnGround();
    void updateAnimData(float deltaTime, int maxFrame);
    void updatePlayerPosition(int velocity, float deltaTime);
    int getJumpVel();
private:
    Texture2D *character;
    AnimData *animData;
    int windowHeight;
    int jumpVel = -600;
};

#endif