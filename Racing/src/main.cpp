
#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "ResourcePath.hpp"
#include "Screen.hpp"
#include "Player.hpp"

using namespace sf;


int roadW = 2000;
int segL = 200; //segment length
float camD = .89f; //camera depth

void drawQuad(RenderWindow &w, Color c, int x1,int y1,int w1,int x2,int y2,int w2)
{
    ConvexShape shape(4);
    shape.setFillColor(c);
    shape.setPoint(0, Vector2f(x1-w1,y1));
    shape.setPoint(1, Vector2f(x2-w2,y2));
    shape.setPoint(2, Vector2f(x2+w2,y2));
    shape.setPoint(3, Vector2f(x1+w1,y1));
    w.draw(shape);
}

struct Line
{
    float x,y,z; //3d center of line
    float X,Y,W; //screen coord
    float curve,spriteX,clip,scale;
    Sprite sprite;
    
    Line()
    {spriteX=curve=x=y=z=0;}
    
    void project(int camX,int camY,int camZ, const Screen *screen)
    {
        scale = camD/(z-camZ);
        X = (1 + scale*(x - camX)) * screen->width/2;
        Y = (1 - scale*(y - camY)) * screen->height/2;
        W = scale * roadW  * screen->width/2;
    }
    
    void drawSprite(RenderWindow &app, const Screen *screen)
    {
        Sprite s = sprite;
        int w = s.getTextureRect().width;
        int h = s.getTextureRect().height;
        
        float destX = X + scale * spriteX * screen->width/2;
        float destY = Y + 4;
        float destW  = w * W / 266;
        float destH  = h * W / 266;
        
        destX += destW * spriteX; //offsetX
        destY += destH * (-1);    //offsetY
        
        float clipH = destY+destH-clip;
        if (clipH<0) clipH=0;
        
        if (clipH>=destH) return;
        s.setTextureRect(IntRect(0,0,w,h-h*clipH/destH));
        s.setScale(destW/w,destH/h);
        s.setPosition(destX, destY);
        app.draw(s);
    }
};



int main()
{
    Screen *screen = new Screen();
    
    RenderWindow app(VideoMode(screen->width, screen->height), "");
    app.setFramerateLimit(60);
    
    Texture t[50];
    Sprite object[50];
    for(int i=1;i<=7;i++)
    {
        t[i].loadFromFile(resourcePath() + std::to_string(i) + ".png");
        t[i].setSmooth(true);
        object[i].setTexture(t[i]);
    }
    
    Texture bg;
    bg.loadFromFile(resourcePath() + "bg.png");
    bg.setRepeated(true);
    Sprite sBackground(bg);
    sBackground.setTextureRect(IntRect(0,0,5000,411));
    sBackground.setPosition(-2000,0);
    
    
    std::vector<Line> lines;
    
    for(int i=0;i<1600;i++)
    {
        Line line;
        line.z = i*segL;
        
        if (i>300 && i<700) line.curve=0.5;
        if (i>1100) line.curve=-0.7;
        
        if (i<300 && i%20==0) {line.spriteX=-2.5; line.sprite=object[5];}
        if (i%17==0)          {line.spriteX=2.0; line.sprite=object[6];}
        if (i>300 && i%20==0) {line.spriteX=-0.7; line.sprite=object[4];}
        if (i>800 && i%20==0) {line.spriteX=-1.2; line.sprite=object[1];}
        if (i==400)           {line.spriteX=-1.2; line.sprite=object[7];}
        
        if (i>750) line.y = sin(i/30.0)*1500;
        
        lines.push_back(line);
    }
    
    int N = lines.size();
    float pos = 0.0f;
    int H = 1500;
    float turnWeight = 0;
    char speedIndicator[8];
    
    
    sf::Text speedText("0", screen->font, 50);
    speedText.setPosition(screen->width - 200, screen->height - 80);
    
    Player *player = new Player();
    player->playIdleSound();
    
    
    player->createSprite(screen);
    sf::Texture playerTexture;
    playerTexture.loadFromFile(resourcePath() + "mercedesspritesheets.png");
    player->sprite.setTexture(playerTexture);
    
    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }
        
        
        
        //detect if the racer is moving
        if(Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Down)) {
            
            //if they are currently driving forward
            if (Keyboard::isKeyPressed(Keyboard::Up) && player->speed < 300.0f && player->speed >= 0.0f) {
                player->speed += .4f;
                player->playAccelerateSound();
            }
            //player reaches max speed
            else if (Keyboard::isKeyPressed(Keyboard::Up) && player->speed >= 300.0f) {
                player->playIdleSound();
            }
            //if they are reversing and APPLY BRAKES
            else if (Keyboard::isKeyPressed(Keyboard::Up) && player->speed < 0.0f) {
                player->playBrakeSound();
                player->speed += 2.0f;
            }
            
            //if they are currently driving in reverse
            if (Keyboard::isKeyPressed(Keyboard::Down) && player->speed > -300.0f && player->speed <= 0.0f) {
                player->playIdleSound();
                player->speed -= .4f;
            }
            //if they are currently driving forward  and APPLY BRAKES
            else if (Keyboard::isKeyPressed(Keyboard::Down) && player->speed > 0.0f) {
                player->playBrakeSound();
                player->speed -= 2.0f;
            }
            
            if (Keyboard::isKeyPressed(Keyboard::W)) H+=100;
            if (Keyboard::isKeyPressed(Keyboard::S)) H-=100;
        
        }
        else {
            
            player->playIdleSound();
            
            if(player->speed > 0.0f && player->speed < .4f) {
                player->speed = 0.0f;
            }
            else if(player->speed > 0.0f) {
                player->speed -= .8f;
            }
            else if(player->speed < 0.0f && player->speed > -.4f) {
                player->speed  = 0.0f;
            }
            else if(player->speed < 0.0f) {
                player->speed += .8;
            }
        }
        
        if(fabs(player->speed)){
            turnWeight = fmin(player->speed * .0005f, 0.04f);
           
            printf("Current Speed: %f\n", player->speed);
            //printf("Current Turn Wieght: %f\n", turnWeight);
        
            
            if ((Keyboard::isKeyPressed(Keyboard::Right) && (player->speed > 0.0f)) ||
                (Keyboard::isKeyPressed(Keyboard::Left) && (player->speed < 0.0f))) {
                player->turnRight(turnWeight);
            }
            else if ((Keyboard::isKeyPressed(Keyboard::Left) && (player->speed > 0.0f)) ||
                (Keyboard::isKeyPressed(Keyboard::Right) && (player->speed < 0.0f))) {
                player->turnLeft(turnWeight);
            }
            else {
                player->steerStraight();
            }
        }
        
    
        
        
        pos+=player->speed;
        
    
        printf("POS BEFORE: %f, %d, %f\n", pos, (N*segL), player->x);
        
        while (pos >= N*segL) pos-=N*segL;
        while (pos < 0.0f) pos += N*segL;
        
        printf("POS AFTER: %f, %d, %f\n", pos, (N*segL), player->x);
        
        app.clear(Color(105,205,4));
        app.draw(sBackground);
        int startPos = pos/segL;
        int camH = lines[startPos].y + H;
        
        
        if(fabsf(player->speed)) {
            
            sBackground.move( lines[startPos].curve * (player->speed/100.0f),0);
            
            
            //part that forces the car to stray from track when the road cruves
            if(player->speed > 0.0f) {
                player->x -= (lines[startPos].curve * (player->speed / 8000.0f));
            }
            else if(player->speed < 0.0f) {
                player->x += (lines[startPos].curve * (player->speed / 8000.0f));
            }
        }

        
        int maxy = screen->height;
        float x=0,dx=0;
    
        
        ///////draw road////////
        for(int n = startPos; n<startPos+300; n++)
        {
            Line &l = lines[n%N];
            l.project(player->x * roadW-x, camH, startPos*segL - (n>=N?N*segL:0), screen);
            x+=dx;
            dx+=l.curve;
            
            l.clip=maxy;
            if (l.Y>=maxy) continue;
            maxy = l.Y;
            
            Color grass  = (n/3)%2?Color(16,200,16):Color(0,154,0);
            Color rumble = (n/3)%2?Color(255,255,255):Color(0,0,0);
            Color road   = (n/3)%2?Color(107,107,107):Color(105,105,105);
            
            Line p = lines[(n-1)%N]; //previous line
            
            drawQuad(app, grass, 0, p.Y, screen->width, 0, l.Y, screen->width);
            drawQuad(app, rumble, p.X, p.Y, p.W*1.2, l.X, l.Y, l.W*1.2);
            drawQuad(app, road,  p.X, p.Y, p.W, l.X, l.Y, l.W);
            //printf("ROAD COORDS %f %f %f %f %f %f", p.X, p.Y, p.W, l.X, l.Y, l.W);
        }
        
        printf("%f\n", lines[startPos].curve);
        
        ////////draw objects////////
        for(int n=startPos+300; n>startPos; n--)
            lines[n%N].drawSprite(app, screen);
        
        app.draw(player->sprite);
        
        sprintf(speedIndicator, "%.0f kmh", ceilf(player->speed));
        speedText.setString(speedIndicator);
        app.draw(speedText);
        
        app.display();
    }
    
    return 0;
}
