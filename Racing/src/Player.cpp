//
//  Player.cpp
//  Racing
//
//  Created by Joseph Persie on 1/22/17.
//  Copyright © 2017 StartupMedia. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <math.h>
#include "ResourcePath.hpp"
#include <iostream>
#include "Player.hpp"
#include "Screen.hpp"
#include <SFML/Audio.hpp>

Player::Player() {
    
    if (!brakeSound.openFromFile(resourcePath() + "car1-braking.ogg") ||
        !accelerateSound.openFromFile(resourcePath() + "car1-accelerating.ogg") ||
        !idleSound.openFromFile(resourcePath() + "car1-idle.ogg")) {
      //@TODO fatal error here
    }
    
    speed = 0.0f;
    height = 43;
    width = 40;
    maxOffroadSpeed = 150.0f;
    maxForwardSpeed = 300.0f;
    maxBackwardSpeed = -200.0f;
    
    y = 1500;
    z = 0.0f;
    x = 0.0f;
    
    speedIdlingResistance = -.8f;
    speedOffroadResistance = -1.0f;
    speedAcceleration = .4f;
    speedBraking = -2.0f;
}

void Player::createSprite(const Screen *screen) {
    
    sf::Texture playerTexture;
    std::string filePath = resourcePath() + "mercedesspritesheets.png";
    playerTexture.loadFromFile(filePath);
    playerTexture.setSmooth(true);
    
    sprite.setTexture(playerTexture);

    sprite.setTextureRect(sf::IntRect(407, 0, width, height));
    sprite.setPosition(screen->width/2 - (width/2), 500);
    sprite.setScale(3, 3);
}

void Player::turnRight(const float turnWeight) {
    x += turnWeight;

    sprite.setTextureRect(sf::IntRect(407 + height, 0, width, height));
}

void Player::turnLeft(const float turnWeight) {
    x -= turnWeight;

    sprite.setTextureRect(sf::IntRect(407 - height, 0, width, height));
}

void Player::steerStraight(void) {
    sprite.setTextureRect(sf::IntRect(407, 0, width, height));
}

void Player::playIdleSound(void) {

    
    if(idleSound.getStatus() == idleSound.Playing) return;
    
    if(brakeSound.getStatus() == brakeSound.Playing) {
        brakeSound.stop();
    }
    else if(accelerateSound.getStatus() == accelerateSound.Playing) {
        accelerateSound.stop();
    }
    
    idleSound.setLoop(true);

    if(speed > 200.0f) {
        idleSound.setPitch(1.4f);
    }
    else {
        idleSound.setPitch(1.0f);
    }
    
    idleSound.play();
}

void Player::playBrakeSound(void) {
    
    if(brakeSound.getStatus() == brakeSound.Playing) return;
    
    
    if(idleSound.getStatus() == idleSound.Playing) {
        idleSound.stop();
    }
    else if(accelerateSound.getStatus() == accelerateSound.Playing) {
        accelerateSound.stop();
    }
    brakeSound.play();
}

void Player::playAccelerateSound(void) {
    
    if(accelerateSound.getStatus() == accelerateSound.Playing) return;
    
    if(brakeSound.getStatus() == brakeSound.Playing) {
        brakeSound.stop();
    }
    else if(idleSound.getStatus() == idleSound.Playing) {
        idleSound.stop();
    }
    
    if(speed > 200.0f) {
       accelerateSound.setPitch(1.2f);
    }
    else {
       accelerateSound.setPitch(1.0f);
    }
    
    accelerateSound.play();
}


bool Player::isDrivingForward(void) {
    return speed > 0.0f;
}

bool Player::hasReachedMaxForwardSpeed(void) {
    return speed >= maxForwardSpeed;
}

bool Player::isDrivingBackward(void) {
    return speed < 0.0f;
}

bool Player::hasReachedMaxBackwardSpeed(void) {
    return speed <= maxBackwardSpeed;
}

bool Player::isStopped(void) {
    return !fabs(speed);
}

void Player::perLoopIdlingResistance() {

    if(isDrivingForward() && speed < speedAcceleration) {
        speed = 0.0f;
    }
    else if(isDrivingForward()) {
        speed += speedIdlingResistance;
    }
    else if(isDrivingBackward() && speed > -speedAcceleration) {
        speed  = 0.0f;
    }
    else if(isDrivingBackward()) {
        speed -= speedIdlingResistance;
    }
}


void Player::perLoopForwardBraking(void) {
    speed += speedBraking;
}

void Player::perLoopBackwardBraking(void) {
    speed -= speedBraking;
}

void Player::perLoopBackwardAcceleration(void) {
    speed -= speedAcceleration;
}

void Player::perLoopForwardAcceleration(void) {
    speed += speedAcceleration;
}


void Player::perLoopForwardAccelerationOffroad(void) {
    //player max speed offroad
    
    y = 1500;
    
    if(speed > maxOffroadSpeed) {
        speed += speedOffroadResistance;
    }
    else {
        perLoopForwardAcceleration();
    }
    //causes the ground to shake
    y += (1 + (rand() % 298));
    idleSound.setPitch(2.3);
}
