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
        void createSprite(const Screen *screen);
        void turnRight(const float turnWeight);
        void turnLeft(const float turnWeight);
        void steerStraight(void);
        float x;
        float speed;
        sf::Sprite sprite;
        Player(void);
        void playIdleSound(void);
        void playAccelerateSound(void);
        void playBrakeSound(void);
    private:
        sf::Music brakeSound;
        sf::Music accelerateSound;
        sf::Music idleSound;
};

#endif /* Player_hpp */
