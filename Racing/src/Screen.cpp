//
//  Screen.cpp
//  Racing
//
//  Created by Joseph Persie on 1/22/17.
//  Copyright © 2017 StartupMedia. All rights reserved.
//

#include "Screen.hpp"
#include "Player.hpp"
#include "ResourcePath.hpp"
#include <SFML/Graphics.hpp>
#include <math.h>


Screen::Screen(void) {
    // Create a graphical text to display
    
    if (!font.loadFromFile(resourcePath() + "Road_Rage.otf")) {
        return EXIT_FAILURE;
    }

    width = 1024;
    height = 768;
}
