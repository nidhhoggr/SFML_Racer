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

void Player::createSprite(const Screen *screen) {
    
    sf::Texture playerTexture;
    std::string filePath = resourcePath() + "mercedesspritesheets.png";
    printf("Player:: %s\n",filePath.c_str());
    playerTexture.loadFromFile(filePath);
    playerTexture.setSmooth(true);
    
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);

    playerSprite.setTextureRect(sf::IntRect(407, 0, 40, 43));
    playerSprite.setPosition(screen->width/2 - 20, 500);
    playerSprite.setScale(3, 3);
    
    this->sprite = playerSprite;
}

void Player::turnRight(const float turnWeight) {
    x += turnWeight;
    sprite.setTextureRect(sf::IntRect(407 + 44, 0, 40, 43));
}

void Player::turnLeft(const float turnWeight) {
    x -= turnWeight;
    sprite.setTextureRect(sf::IntRect(407 - 44, 0, 40, 43));
}

void Player::steerStraight(void) {
    
    sprite.setTextureRect(sf::IntRect(407, 0, 40, 43));
}
