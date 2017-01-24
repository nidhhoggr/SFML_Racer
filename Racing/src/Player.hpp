//
//  Player.hpp
//  Racing
//
//  Created by Joseph Persie on 1/22/17.
//  Copyright © 2017 StartupMedia. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Screen.hpp"

class Player {
public:
    Player(void);
    void createSprite(const Screen *screen);
    void turnRight(const float turnWeight);
    void turnLeft(const float turnWeight);
    void steerStraight(void);
    bool isDrivingForward(void);
    bool hasReachedMaxForwardSpeed(void);
    bool isDrivingBackward(void);
    bool isStopped(void);
    bool hasReachedMaxBackwardSpeed(void);
    void playIdleSound(void);
    void playAccelerateSound(void);
    void playBrakeSound(void);
    
    void perLoopIdlingResistance(void);
    void perLoopForwardBraking(void);
    void perLoopBackwardBraking(void);
    void perLoopBackwardAcceleration(void);
    void perLoopForwardAcceleration(void);
    void perLoopForwardAccelerationOffroad(void);
    
    sf::Sprite sprite;
    int height;
    int width;
    float x;
    float y;
    float z;
    float speed;
    float maxForwardSpeed;
    float maxBackwardSpeed;
    float maxOffroadSpeed;
private:
    sf::Music brakeSound;
    sf::Music accelerateSound;
    sf::Music idleSound;
    float speedIdlingResistance;
    float speedOffroadResistance;
    float speedAcceleration;
    float speedBraking;
};

#endif /* Player_hpp */
