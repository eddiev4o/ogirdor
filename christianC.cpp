// Name: Christian Capusi
// Date: June 21, 2017
// Recent: July 22, 2017
// Purpose: Create a sidescroller with a team

//===============================================
// Responsibilities - Working Code
//===============================================
// Level Collision Physics
// Main Character Jump/Falling Physics
// Main Character/Sprite Movements
// Main Character Shooting
// Particle Creation
// Particle Physics
// Particle Enemy Collision Logic
// Enemy Death Logic
// Power Up Pick Up and Logic
// Main Character State Animations
//===============================================

//===============================================
// Cool features
//===============================================
// Movement is created through an illusion by
//     moving everything but the main character
// Collision works with the dynamic text-based
//     tile system
// Jump height is dynamically calculated based on
//     current character position before jumping
// Particle movement is affected by
//    character's movement!
// Picking up power ups increases the sprite's
//    stats
// Attributes (health, movementspeed, etc.) are
//    stored in a class "Sprite" this makes
//    it easy to manipulate. Good for power ups!
//===============================================

//===============================================
// Week 4 Progress
//===============================================
// Displayed Main Character Sprite on Screen
// with Movement and Shooting animations
//
// No actual shooting yet
//===============================================

//===============================================
// Week 5 Progress
//===============================================
// Created Sprite class with variables such as
// x coordinate and y coordinate which is useful
// for placing sprites on the map.
//
// With these variables, I was able to create
// the illusion of movement by moving every
// Sprite but the main character in the middle
//===============================================

//===============================================
// Week 6 Progress
//===============================================
// Added Jump animations
//
// Created dynamic jump physics which calculates
// jump height based on the y coordinate before
// jumping
//===============================================

//===============================================
// Week 7 Progress
//===============================================
// Added Power ups
//
// Collision detection for power ups. Main
// character picks up the power ups by walking 
// on it
//
// Power ups disappear once picked up by
// stopping rendering and teleporting it
// to far far away
//===============================================

//===============================================
// Week 8 Progress
//===============================================
// Created enemy particle collision + death logic
// Created dynamic level collision Physics
//    Change the level however you want using level.txt
//    and the physics will still work :^)
//
// This took the longest to implement of all
//===============================================

//===============================================
// Week 9 Progress
//===============================================
// Added 2 animated moving enemies to the game
//     with death and particle collision
//===============================================

//===============================================
// Week 10 Progress
//===============================================
//===============================================

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include "fonts.h"
#include "game.h"

#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <vector>

#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

// Everything about tile collision and
// Movement
void tileCollision(Vec*);
void checkUnder(Vec*);
void moveSpriteRight(Sprite*);
void moveSpriteLeft(Sprite*);
void moveLevelRight();
void moveLevelLeft();
void mainMoveSpriteRight();
void mainMoveSpriteLeft();
void spriteDisappear(Sprite*);

// Everything about particles and combat
void makeParticle();
void shootParticle();
void particlePhysics(int);
void particleMove();
void enemyParticleCollision(Particle*, Sprite*);

// Everything about main character animations
void renderMainCharacter(int);
void conditionalRenders(Flt, Flt, Flt, Flt, Flt, Flt);
void shootWalkRight(Flt , Flt , Flt , Flt , Flt , Flt );
void shootWalkLeft(Flt , Flt , Flt , Flt , Flt , Flt );
void shootStandRight(Flt , Flt , Flt , Flt , Flt , Flt );
void shootStandLeft(Flt , Flt , Flt , Flt , Flt , Flt );
void standRight(Flt , Flt , Flt , Flt , Flt , Flt );
void standLeft(Flt , Flt , Flt , Flt , Flt , Flt );
void jumpLeft(Flt, Flt, Flt, Flt, Flt, Flt);

// Everything about jump physics
void jump();
void checkJump();

// Everything about rendering other sprites
void renderSpeedboost1();
void renderShield1();
void renderHeart1();
void renderHeart2();
void renderHeart3();
void renderHeart4();
void christianInit();

// Everything about sound

//the "main function of my file"
void renderChristianSprites(int);

void clearScreen();
using namespace std;

vector<Sprite*> allSprites;
vector<Sprite*> shootableSprites;

void addToAllSprites()
{
    //add every sprite here that reacts to player movement
    //this is so all can be accessed through a for loop
    //when checking for movement 
    if (allSprites.size() < 2) {
        allSprites.push_back(&turt1);
        allSprites.push_back(&turt2);
        allSprites.push_back(&mariEnemy);
        allSprites.push_back(&female);
        allSprites.push_back(&obama);
        allSprites.push_back(&sun);
        allSprites.push_back(&heart1);
        allSprites.push_back(&heart2);
        allSprites.push_back(&heart3);
        allSprites.push_back(&heart4);
        allSprites.push_back(&shield1);
        allSprites.push_back(&speedboost1);
        allSprites.push_back(&turret);
        allSprites.push_back(&turretbeam);
        allSprites.push_back(&enemy1);
        allSprites.push_back(&godzilla);
        allSprites.push_back(&godzillaball);
        allSprites.push_back(&pika);
        allSprites.push_back(&taco);
    }
}

void addToShootableSprites()
{
    //add every sprite here that reacts to player shooting
    //this is so all can be accessed through a for loop
    //when checking for collision
    if (shootableSprites.size() < 2) {
        shootableSprites.push_back(&turt1);
        shootableSprites.push_back(&turt2);
        shootableSprites.push_back(&mariEnemy);
        shootableSprites.push_back(&female);
        shootableSprites.push_back(&obama);
        shootableSprites.push_back(&turret);
        shootableSprites.push_back(&enemy1);
        shootableSprites.push_back(&godzilla);
    }
} 

// These functions move sprites based on character movement
// and collision.
// If main character presses right, sprites and level move left
// If main character presses left, sprites and level move right
// Each layer have different speeds (background and foreground)
void moveSpriteRight(Sprite *sprt)
{
    if (gl.camera[0] > 0 && gl.cantMove == false) {
        sprt->cx = sprt->cx + gl.movementSpeed;
    }
}

void moveSpriteLeft(Sprite *sprt)
{
    if (gl.cantMove == false) {
        sprt->cx = sprt->cx - gl.movementSpeed;
    }
}

void moveLevelRight()
{
    if (gl.camera[0] > 0.1 && gl.cantMove == false) {
        gl.camera[0] -= gl.movementSpeed;
    }
}

void moveLevelLeft()
{
    if (gl.cantMove == false) {
        gl.camera[0] += gl.movementSpeed;
    }
}

void mainMoveSpriteRight()
{
    for (unsigned int i = 0;  i < allSprites.size(); i++) {
        moveSpriteRight(allSprites[i]);
    }
}

void mainMoveSpriteLeft()
{
    for (unsigned int i = 0;  i < allSprites.size(); i++) {
        moveSpriteLeft(allSprites[i]);
    }
}

void tileCollision(Vec *tile)
{
    if ((((mainChar.cy) >= (tile->y))
    && ((mainChar.cy) <= (tile->y) + lev.tilesize[1] + 20))
    && (((mainChar.cx) >= (tile->x))
    && ((mainChar.cx) <= (tile->x) + lev.tilesize[0]))) {
        // if character is walking into tile, move sprites and level to counteract
	// default sprite movement bounded to XK_Left and XK_Right
        if (gl.directionFlag == 0) {
            gl.camera[0] -= gl.movementSpeed;
            gl.xc[0] -= 0.001;
            gl.xc[1] -= 0.001;
            for (unsigned int i = 0;  i < allSprites.size(); i++) {
                moveSpriteRight(allSprites[i]);
            }
            for (int i = 0; i < 100; i++) {
                moveSpriteRight(&gl.coins[i]);
            }
        }
        if (gl.directionFlag == 1) {
            gl.camera[0] += gl.movementSpeed;
            gl.xc[0] += 0.001;
            gl.xc[1] += 0.001;
            for (unsigned int i = 0;  i < allSprites.size(); i++) {
                moveSpriteLeft(allSprites[i]);
            }
            for (int i = 0; i < 100; i++) {
                moveSpriteLeft(&gl.coins[i]);
            }
        }
        gl.isJumpingFlag = false;
        mainChar.cy = mainChar.cy + 1;
    }
    // if your character jumps, the lowest Y coordinate it can go is 0. This utilizes my checkJump function
    if (!((((mainChar.cy - 3) >= (tile->y))
    && ((mainChar.cy - 3) <= (tile->y - 20) + lev.tilesize[1]))
    && (((mainChar.cx) >= (tile->x))
    && ((mainChar.cx) <= (tile->x + 20) + lev.tilesize[0])))) {
        gl.initialJumpCy = 0;
    }
}

void checkUnder(Vec *tile)
{
    // check if there is a tile under the mainCharacter. If there is no tile,
    // it means your character is mid air. Initiate 2nd phase of checkJump() (falling).
    if ((((mainChar.cy - 30) >= (tile->y))
    && ((mainChar.cy - 30) <= (tile->y) + lev.tilesize[1] + 20))
    && (((mainChar.cx) >= (tile->x))
    && ((mainChar.cx) <= (tile->x) + lev.tilesize[0]))) {
        printf("On Air!\n");
        if (gl.isJumpingFlag == false) {
            gl.isJumpingFlag = true;
            gl.jumpDirectionFlag = 0;
        }
    }
}

void spriteDisappear(Sprite* sprt)
{
    sprt->cx = -9999999;
}


void enemyParticleCollision(Particle* p, Sprite *sprt)
{
    if ( p->cx > (sprt->cx - sprt->height / 2)
    && p->cx < (sprt->cx + sprt->height / 2)) {
        if (p->cy > (sprt-> cy - sprt->height / 2)
        && p->cy < (sprt->cy + sprt->height / 2)) {
            //when particle hits enemy, teleport to all the way up
            //this doesnt mess up particle off screen detection
            p->cy = 1000;
            sprt->health = sprt->health - 1;
        }
        if (sprt->health == 0) {
            //when enemy reaches 0 health, they "die" by teleporting
            spriteDisappear(sprt);
        }
    }
}

void particleMove()
{
    //move particles based on character movement.
    //if character is moving same direction, slow down
    //if character is moving opposite direction, speed up
    for(int i = 0; i < gl.particleCount; i++) {
        if (gl.keys[XK_Left] && gl.particle[i].direction == 1) {
            gl.particle[i].cx = gl.particle[i].cx + gl.movementSpeed;
        }
        if (gl.keys[XK_Right] && gl.particle[i].direction == 0) {
            gl.particle[i].cx = gl.particle[i].cx - gl.movementSpeed;
        }
    }
}

void particlePhysics(int charSelect)
{
    // Depending on what they selected, bind that texture to the main
    // character's particles
    if (charSelect == 1)
        gl.tempTexture = gl.purpleballTexture;
    if (charSelect == 2)
        gl.tempTexture = gl.moltenballTexture;
    if (charSelect == 3)
        gl.tempTexture = gl.texthighlightTexture;
    if (charSelect == 4)
        gl.tempTexture = gl.americaballTexture;
    if (charSelect == 5)
        gl.tempTexture = gl.applecoreTexture;
    if (charSelect == 6)
        gl.tempTexture = gl.americaballTexture;
   
    for (int i = 0; i < gl.particleCount; i++) {
        glPushMatrix();
        glColor3f(1.0,1.0,1.0);
        glTranslatef(gl.particle[i].cx, gl.particle[i].cy, 0);
        float w = gl.particleWidth;
        float h = gl.particleHeight;
        glBindTexture(GL_TEXTURE_2D, gl.tempTexture);
        glEnable(GL_ALPHA_TEST);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0); glVertex2i(-w, -h);
        glTexCoord2f(0.0, 0.0); glVertex2i(-w,  h);
        glTexCoord2f(1.0, 0.0); glVertex2i(w,   h);
        glTexCoord2f(1.0, 1.0); glVertex2i(w,  -h);
        glEnd();
        glPopMatrix();
        glDisable(GL_ALPHA_TEST);
        if (gl.particle[i].direction == 1) {
            gl.particle[i].cx = gl.particle[i].cx - gl.particle[i].velocity;
        }
        if (gl.particle[i].direction == 0) {
            gl.particle[i].cx = gl.particle[i].cx + gl.particle[i].velocity;
        }
        if (gl.particle[i].cx < 0 || gl.particle[i].cx > gl.xres) {
            gl.particle[i] = gl.particle[gl.particleCount - 1];
            gl.particleCount--;
        }
        // projectilecollisions. Pass each particle and check if it collides
        // with the sprites
        for (int i =0; i < gl.particleCount; i++) {
            for (unsigned int j = 0; j < shootableSprites.size(); j++) {
                enemyParticleCollision(&gl.particle[i], shootableSprites[j]);
            }
        }
    }
}

void makeParticle()
{
    // Initialize position and velocity of each particle
    gl.particle[gl.particleCount].cx = mainChar.cx;
    gl.particle[gl.particleCount].cy = mainChar.cy;
    gl.particle[gl.particleCount].velocity = gl.particleVelocity;
    gl.particle[gl.particleCount].direction = gl.directionFlag;
    gl.particleCount++;
}

void shootParticle()
{
    // XK_space allows you to shoot. However, you can only shoot once
    // per press of the space key. This is done using a flag
    // that resets when pressing of space key is stopped.
    if (gl.oneOffShootFlag == true) {
        printf("Shoot\n");
        makeParticle(); 
        gl.oneOffShootFlag = false;
    }
}

// Conditional statements that offsets sprite sheet based on what
// The characters are doing. Function names should be self explanatory
void shootWalkRight(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx + .25, ty + .333); glVertex2i(cx + w, cy - h);
    glTexCoord2f(tx,       ty + .333); glVertex2i(cx - w, cy - h);
    glTexCoord2f(tx,              ty); glVertex2i(cx - w, cy + h);
    glTexCoord2f(tx + .25,        ty); glVertex2i(cx + w, cy + h);
}

void shootWalkLeft(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx + .25, ty + .333); glVertex2i(cx - w, cy - h);
    glTexCoord2f(tx + .25,        ty); glVertex2i(cx - w, cy + h);
    glTexCoord2f(tx,              ty); glVertex2i(cx + w, cy + h);
    glTexCoord2f(tx,       ty + .333); glVertex2i(cx + w, cy - h);
}    

void shootStandRight(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx + .25, ty + .666); glVertex2i(cx + w, cy - h);
    glTexCoord2f(tx,       ty + .666); glVertex2i(cx - w, cy - h);
    glTexCoord2f(tx,       ty + .333); glVertex2i(cx - w, cy + h);
    glTexCoord2f(tx+.25,   ty + .333); glVertex2i(cx + w, cy + h);
}

void shootStandLeft(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx,       ty + .666); glVertex2i(cx + w, cy - h);
    glTexCoord2f(tx + .25, ty + .666); glVertex2i(cx - w, cy - h);
    glTexCoord2f(tx + .25, ty + .333); glVertex2i(cx - w, cy + h);
    glTexCoord2f(tx,       ty + .333); glVertex2i(cx + w, cy + h);
}

void standRight(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx + .25, ty + .333); glVertex2i(cx + w, cy - h);
    glTexCoord2f(tx,       ty + .333); glVertex2i(cx - w, cy - h);
    glTexCoord2f(tx,              ty); glVertex2i(cx - w, cy + h);
    glTexCoord2f(tx + .25,        ty); glVertex2i(cx + w, cy + h);
}

void standLeft(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx + .25, ty + .333); glVertex2i(cx - w, cy - h);
    glTexCoord2f(tx + .25,        ty); glVertex2i(cx - w, cy + h);
    glTexCoord2f(tx,              ty); glVertex2i(cx + w, cy + h);
    glTexCoord2f(tx,       ty + .333); glVertex2i(cx + w, cy - h);
}

void jumpRight(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx,          ty + 1); glVertex2i(cx - w, cy - h);
    glTexCoord2f(tx,       ty + .666); glVertex2i(cx - w, cy + h);
    glTexCoord2f(tx +.25,  ty + .666); glVertex2i(cx + w, cy + h);
    glTexCoord2f(tx +.25,     ty + 1); glVertex2i(cx + w, cy - h);
}

void jumpLeft(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx + .25,    ty + 1); glVertex2i(cx - w, cy - h);
    glTexCoord2f(tx + .25, ty + .666); glVertex2i(cx - w, cy + h);
    glTexCoord2f(tx,       ty + .666); glVertex2i(cx + w, cy + h);
    glTexCoord2f(tx,          ty + 1); glVertex2i(cx + w, cy - h);
}

// this function initiates the jump process by initializing
// needed variables and making simple calculations
void jump()
{
    printf("Jump\n");
    if (gl.isJumpingFlag == 0) {
        //temporarily store current y coord
        gl.initialJumpCy = mainChar.cy;
        //temporarily store max jump height
        gl.finalJumpCy = gl.initialJumpCy + gl.jumpHeight;
        gl.isJumpingFlag = 1;
        gl.jumpDirectionFlag = 1;
    }
}

// Each cycle of the game, this function checks if the character
// is jumping using the gl.isJumpingFlag.
//
// gl.jumpDirectionFlag determines if the main character is going
// upwards or downwards.
//
// Once calculated height is reached from the jump() function,
// gl.jumpDirectionFlag changes.
//
// Once original height is reached after falling, gl.isJumpingFlag
// is returned to false.
void checkJump()
{
    // if character started jump, increment to max height
    if (gl.isJumpingFlag == 1 && gl.jumpDirectionFlag == 1) {
        if (mainChar.cy <= gl.finalJumpCy) {
            mainChar.cy = 1 + mainChar.cy + ((gl.finalJumpCy - mainChar.cy) * gl.jumpRate);
        }
        if (mainChar.cy >= gl.finalJumpCy) {
            gl.jumpDirectionFlag = 0;
        }
    }
    // When character reaches max height, decrement to highest tile on x coord
    if (gl.isJumpingFlag == 1 && gl.jumpDirectionFlag == 0) {
        if (mainChar.cy > gl.initialJumpCy) {
        //if (!)
            mainChar.cy = mainChar.cy - ((gl.finalJumpCy - mainChar.cy) * gl.jumpRate) - 1;
        }
        if (mainChar.cy <= gl.initialJumpCy) {
            mainChar.cy = gl.initialJumpCy;
            //gl.isJumpingFlag = 0;
        }
    }
}

// This function determines which frame of the sprite sheet is displayed
//    depending on what the user is doing.
// gl.keys[XK_Left]  detects when a specific key is pressed. in this case,
//    the left button.
// gl.keys[XK_Up] is used to jump
// gl.keys[XK_space] is used to shoot
// gl.isJumpingFlag  is from the checkJump() function which determines
//    if the character is currently jumping or not
// gl.directionFlag determines what direction the character is facing.
//    this flag changes based on which direction was last faced. Ex:
//    moving to the right will change direction to right until changed
//    again.
void conditionalRenders(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    checkJump();
    if (gl.keys[XK_Right] && gl.keys[XK_space] == 0 &&
    gl.isJumpingFlag == 0) {
        shootWalkRight(tx, ty, cx, w, cy, h);
        gl.directionFlag = 0;
    } else if (gl.keys[XK_Left] && gl.keys[XK_space] == 0 &&
    gl.isJumpingFlag == 0) {
        shootWalkLeft(tx, ty, cx, w, cy, h);
        gl.directionFlag = 1;
    } else if ((gl.keys[XK_space] && gl.directionFlag == 0 &&
    gl.isJumpingFlag == 0) || (gl.keys[XK_space] && gl.keys[XK_Right])) {
        shootStandRight(tx, ty, cx, w, cy, h);
        shootParticle();
    } else if ((gl.keys[XK_space] && gl.directionFlag == 1 &&
    gl.isJumpingFlag == 0) || (gl.keys[XK_space] && gl.keys[XK_Left])) {
        shootStandLeft(tx, ty, cx, w, cy, h);
        shootParticle();
    } else if (gl.keys[XK_Left] && gl.isJumpingFlag == 1 &&
    (gl.keys[XK_space] == 1 || gl.keys[XK_space] == 0)) {
        jumpLeft(tx, ty, cx, w, cy, h);
        gl.directionFlag = 1;
    } else if (gl.keys[XK_Right] && gl.isJumpingFlag == 1 &&
    (gl.keys[XK_space] == 1 || gl.keys[XK_space] == 0)) {
        jumpRight(tx, ty, cx, w, cy, h);
        gl.directionFlag = 0;
    }

    if (gl.keys[XK_Right] == 1 || gl.keys[XK_Left] == 1) {
        particleMove();
    }
    if (gl.keys[XK_space] == 0) {
        gl.oneOffShootFlag = true;
    }
    if (gl.keys[XK_Up] && gl.isJumpingFlag == 0) {
        jump();
    }
    if (gl.keys[XK_Left] == 0 && gl.keys[XK_Right] == 0 &&
    gl.directionFlag == 1 && gl.keys[XK_space] == 0 &&
    gl.isJumpingFlag == 0) {
        standLeft(tx, ty, cx, w, cy, h);
    }
    if (gl.keys[XK_Right] == 0 && gl.keys[XK_Right] == 0 &&
    gl.directionFlag == 0 && gl.keys[XK_space] == 0  &&
    gl.isJumpingFlag == 0) {
        standRight(tx, ty, cx, w, cy, h);
    }
    if (gl.keys[XK_Left] == 0 && gl.keys[XK_Right] == 0 &&
    gl.directionFlag == 1 && (gl.keys[XK_space] == 0 || 
    gl.keys[XK_space] == 1) && gl.isJumpingFlag == 1) {
        jumpLeft(tx, ty, cx, w, cy, h);
        if (gl.keys[XK_space] == 1) {
            shootParticle();
        }
    }
    if (gl.keys[XK_Right] == 0 && gl.keys[XK_Right] == 0 &&
    gl.directionFlag == 0 && (gl.keys[XK_space] == 0 ||
    gl.keys[XK_space] == 1) && gl.isJumpingFlag == 1) {
        jumpRight(tx, ty, cx, w, cy, h);
        if (gl.keys[XK_space] == 1) {
            shootParticle();
        }
    }
}

// This function clears the screen each cycle to create
// "frames" like how videos have frames per second.
void clearScreen()
{
    glClearColor(0.1,0.1,0.1,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

//---
void renderMainCharacter(int charSelect)
{
    // Texture of main character depends on what they select
    if (charSelect == 1)
        gl.tempMainCharacterTexture = gl.mchar1Texture;
    if (charSelect == 2)
        gl.tempMainCharacterTexture = gl.mchar2Texture;
    if (charSelect == 3)
        gl.tempMainCharacterTexture = gl.mchar3Texture;
    if (charSelect == 4)
        gl.tempMainCharacterTexture = gl.mchar4Texture;
    if (charSelect == 5)
        gl.tempMainCharacterTexture = gl.mchar5Texture;
    if (charSelect == 6)
        gl.tempMainCharacterTexture = gl.mchar4Texture;

    mainChar.cx = gl.xres/2.0;
    float h = 29.0;
    float w = h * .903;
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.tempMainCharacterTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ix = gl.mcharFrame % 4;
    int iy = 3;
    if (gl.mcharFrame >= 4 && gl.attackFlag == 0)
        iy = 0;
    float tx = (float)ix / 4.0;
    float ty = (float)iy / 3.0;
    glBegin(GL_QUADS);
    conditionalRenders(tx, ty, mainChar.cx, w, mainChar.cy, h);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

void renderSpeedboost1()
{
    if (gl.speedboost1Flag != false) {
        float h = 25;
        float w = 25;
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, gl.speedboostTexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        int ix = 1, iy = 1;
        float tx = (float)ix;
        float ty = (float)iy;
        glBegin(GL_QUADS);
        glTexCoord2f(tx + 1, ty + 1); glVertex2i(speedboost1.cx-w, speedboost1.cy-h);
        glTexCoord2f(tx + 1,     ty); glVertex2i(speedboost1.cx-w, speedboost1.cy+h);
        glTexCoord2f(tx,        ty ); glVertex2i(speedboost1.cx+w, speedboost1.cy+h);
        glTexCoord2f(tx,     ty + 1); glVertex2i(speedboost1.cx+w, speedboost1.cy-h);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        
	if ((mainChar.cx > (speedboost1.cx - 10)
        && mainChar.cx < (speedboost1.cx + 10))
        && ((mainChar.cy > (speedboost1.cy - 10))
        && (mainChar.cy < (speedboost1.cy + 10)))) {
            //if character picks up power up,
            //stop rendering and set x to -999999 to avoid
            //picking up invisible power ups
            printf("Picked up speed boost! Movement Speed: + 0.2!\n");
            gl.speedboost1Flag = false;
            spriteDisappear(&speedboost1);
            gl.movementSpeed = gl.movementSpeed + 0.2;
        }
    }
}

void renderShield1()
{
    if (gl.shield1Flag != false) {
        float h = 25;
        float w = 25;
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, gl.shieldTexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        int ix = 1, iy = 1;
        float tx = (float)ix;
        float ty = (float)iy;
        glBegin(GL_QUADS);
        glTexCoord2f(tx + 1, ty + 1); glVertex2i(shield1.cx-w, shield1.cy-h);
        glTexCoord2f(tx + 1, ty    ); glVertex2i(shield1.cx-w, shield1.cy+h);
        glTexCoord2f(tx,     ty    ); glVertex2i(shield1.cx+w, shield1.cy+h);
        glTexCoord2f(tx,     ty + 1); glVertex2i(shield1.cx+w, shield1.cy-h);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        
	if ((mainChar.cx > (shield1.cx - 10)
        && mainChar.cx < (shield1.cx + 10))
        && ((mainChar.cy > (shield1.cy - 10))
        && (mainChar.cy < (shield1.cy + 10)))) {
            //if character picks up power up,
            //stop rendering and set x to -999999 to avoid
            //picking up invisible power ups
            printf("Picked up Shield!\n");
            gl.shield1Flag = false;
            spriteDisappear(&shield1);
        }
    }
}

void renderHeart1()
{
    if (gl.heart1Flag != false) {
        float h = 25;
        float w = 25;
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, gl.heartaddTexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        int ix = 1, iy = 1;
        float tx = (float)ix;
        float ty = (float)iy;
        glBegin(GL_QUADS);
        glTexCoord2f(tx + 1, ty + 1); glVertex2i(heart1.cx-w, heart1.cy-h);
        glTexCoord2f(tx + 1, ty    ); glVertex2i(heart1.cx-w, heart1.cy+h);
        glTexCoord2f(tx,     ty    ); glVertex2i(heart1.cx+w, heart1.cy+h);
        glTexCoord2f(tx,     ty + 1); glVertex2i(heart1.cx+w, heart1.cy-h);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        
	if ((mainChar.cx > (heart1.cx - 10)
        && mainChar.cx < (heart1.cx + 10))
        && ((mainChar.cy > (heart1.cy - 10))
        && (mainChar.cy < (heart1.cy + 10)))) {
            //if character picks up power up,
            //stop rendering and set x to -999999 to avoid
            //picking up invisible power ups
            gl.heart1Flag = false;
            spriteDisappear(&heart1);
            printf("Picked up Health Pack! Health + 5\n");
            if (mainChar.health < 30) {
                mainChar.health = mainChar.health + 5;
                if (mainChar.health > 30) {
                    mainChar.health = 30;
                }
            }
        }
    }
}

void renderHeart2()
{
    if (gl.heart2Flag != false) {
        float h = 25;
        float w = 25;
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, gl.heartaddTexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        int ix = 1, iy = 1;
        float tx = (float)ix;
        float ty = (float)iy;
        glBegin(GL_QUADS);
        glTexCoord2f(tx, ty + 1); glVertex2i(heart2.cx-w, heart2.cy-h);
        glTexCoord2f(tx, ty    ); glVertex2i(heart2.cx-w, heart2.cy+h);
        glTexCoord2f(tx,     ty    ); glVertex2i(heart2.cx+w, heart2.cy+h);
        glTexCoord2f(tx,     ty + 1); glVertex2i(heart2.cx+w, heart2.cy-h);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        
	if ((mainChar.cx > (heart2.cx - 10)
        && mainChar.cx < (heart2.cx + 10))
        && ((mainChar.cy > (heart2.cy - 10))
        && (mainChar.cy < (heart2.cy + 10)))) {
            //if character picks up power up,
            //stop rendering and set x to -999999 to avoid
            //picking up invisible power ups
            gl.heart2Flag = false;
            printf("Picked up Health Pack! Health + 5\n");
            spriteDisappear(&heart2);
            if (mainChar.health < 30) {
                mainChar.health = mainChar.health + 5;
                if (mainChar.health > 30) { 
                    mainChar.health = 30;
                }
            }
        }
    }
}

void renderHeart3()
{
    if (gl.heart3Flag != false) {
        float h = 25;
        float w = 25;
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, gl.heartaddTexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        int ix = 1, iy = 1;
        float tx = (float)ix;
        float ty = (float)iy;
        glBegin(GL_QUADS);
        glTexCoord2f(tx + 1, ty + 1); glVertex2i(heart3.cx-w, heart3.cy-h);
        glTexCoord2f(tx + 1, ty    ); glVertex2i(heart3.cx-w, heart3.cy+h);
        glTexCoord2f(tx,     ty    ); glVertex2i(heart3.cx+w, heart3.cy+h);
        glTexCoord2f(tx,     ty + 1); glVertex2i(heart3.cx+w, heart3.cy-h);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        
	if ((mainChar.cx > (heart3.cx - 10)
        && mainChar.cx < (heart3.cx + 10))
        && ((mainChar.cy > (heart3.cy - 10))
        && (mainChar.cy < (heart3.cy + 10)))) {
            //if character picks up power up,
            //stop rendering and set x to -999999 to avoid
            //picking up invisible power ups
            gl.heart3Flag = false;
            spriteDisappear(&heart3);
            printf("Picked up Health Pack! Health + 5\n");
            if (mainChar.health < 30) {
                mainChar.health = mainChar.health + 5;
                if (mainChar.health > 30) {
                    mainChar.health = 30;
                }
            }
        }
    }
}

void renderHeart4()
{
    if (gl.heart4Flag != false) {
        float h = 25;
        float w = 25;
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, gl.heartaddTexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        int ix = 1, iy = 1;
        float tx = (float)ix;
        float ty = (float)iy;
        glBegin(GL_QUADS);
        glTexCoord2f(tx + 1, ty + 1); glVertex2i(heart4.cx-w, heart4.cy-h);
        glTexCoord2f(tx + 1, ty    ); glVertex2i(heart4.cx-w, heart4.cy+h);
        glTexCoord2f(tx,     ty    ); glVertex2i(heart4.cx+w, heart4.cy+h);
        glTexCoord2f(tx,     ty + 1); glVertex2i(heart4.cx+w, heart4.cy-h);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        
	if ((mainChar.cx > (heart4.cx - 10)
        && mainChar.cx < (heart4.cx + 10))
        && ((mainChar.cy > (heart4.cy - 10))
        && (mainChar.cy < (heart4.cy + 10)))) {
            //if character picks up power up,
            //stop rendering and set x to -999999 to avoid
            //picking up invisible power ups
            gl.heart4Flag = false;
            printf("Picked up Health Pack! Health + 5\n");
            spriteDisappear(&heart4);
            if (mainChar.health < 30) {
                mainChar.health = mainChar.health + 5;
                if (mainChar.health > 30) {
                    mainChar.health = 30;
                }
            }
        }
    }
}

void christianInit()
{
    addToShootableSprites();
    addToAllSprites();
    if (gl.levelSelect == 2) {
        turt2.cx = 850;
        turt2.cy = 95;
        turt1.cx = 500;
        turt1.cy = 95;
        mainChar.cy = 85;
        shield1.cx = 800;
        shield1.cy = 180;
        heart1.cx = 1000;
        heart1.cy = 90;
        heart2.cx = 800;
        heart2.cy = 120;
        heart3.cx = 800;
        heart3.cy = 90;
        heart4.cx = 800;
        heart4.cy = 200;
        speedboost1.cx= 800;
        speedboost1.cy = 90;
        // render sprite set to true.
        // when flase, stop rendering. This is for
        // picking up power ups
        gl.shield1Flag = true;
        gl.heart1Flag = true; 
        gl.heart2Flag = true; 
        gl.heart3Flag = true; 
        gl.heart4Flag = true;
        gl.speedboost1Flag = true;
        // these prevent the variables
        // from storing trash values which caused problems
        gl.finalJumpCy = mainChar.cy + 20;
        gl.initialJumpCy = mainChar.cy + 20;
    } else {
        //initialize my sprites' x and y positions
        turt2.cx = 850;
        turt2.cy = 95;
        turt1.cx = 500;
        turt1.cy = 95;
        mainChar.cy = 85;
        shield1.cx = 900;
        shield1.cy = 180;
        heart1.cx = 3000;
        heart1.cy = 90 + (32*4);
        heart2.cx = 700;
        heart2.cy = 120;
        heart3.cx = 700;
        heart3.cy = 90;
        heart4.cx = 6000;
        heart4.cy = (32*11);
        speedboost1.cx= 750;
        speedboost1.cy = 90;
        // render sprite set to true.
        // when flase, stop rendering. This is for
        // picking up power ups
        gl.shield1Flag = true;
        gl.heart1Flag = true; 
        gl.heart2Flag = true; 
        gl.heart3Flag = true; 
        gl.heart4Flag = true;
        gl.speedboost1Flag = true;
        // these prevent the variables
        // from storing trash values which caused problems
        gl.finalJumpCy = mainChar.cy + 20;
        gl.initialJumpCy = mainChar.cy + 20;
        //if different level
    }
}

void renderTurt1()
{
    float h = 25;
    float w = 25;
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.greenenemyTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ix = 1;
    int iy = 1;
    float tx = (float)ix;
    float ty = (float)iy;
    if (gl.turt1Frame >= 3)
        iy = 0;
    double timeSpan = 
        timers.timeDiff(&timers.turt1Time, &timers.timeCurrent);
    if (timeSpan > .5) {
        ++gl.turt1Frame;
        if (gl.turt1Frame >= 3)
            gl.turt1Frame-= 3;
        timers.recordTime(&timers.turt1Time);
    } 
    glBegin(GL_QUADS);
    glTexCoord2f(tx + (gl.turt1Frame - 1) * .333,   ty + 1);
    glVertex2i(turt1.cx-w, turt1.cy-h);
    glTexCoord2f(tx + (gl.turt1Frame - 1) * .333,   ty    );
    glVertex2i(turt1.cx-w, turt1.cy+h);
    glTexCoord2f(tx + (gl.turt1Frame* .333),       ty    );
    glVertex2i(turt1.cx+w, turt1.cy+h);
    glTexCoord2f(tx + (gl.turt1Frame* .333),      ty + 1);
    glVertex2i(turt1.cx+w, turt1.cy-h);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

void turt1Movement()
{
    if (gl.turt1Flag == 0)
    {
        turt1.cx++;
        if (turt1.cx >= 700)
            gl.turt1Flag = 1;
    }
    if (gl.turt1Flag == 1)
    {
        turt1.cx--;
        if (turt1.cx < 400)
            gl.turt1Flag = 0;
    }
}

void renderTurt2()
{
    float h = 25;
    float w = 25;
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.blueenemyTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ix = 1;
    int iy = 1;
    float tx = (float)ix;
    float ty = (float)iy;
    if (gl.turt2Frame >= 3)
        iy = 0;
    double timeSpan = 
        timers.timeDiff(&timers.turt2Time, &timers.timeCurrent);
    if (timeSpan > .5) {
        ++gl.turt2Frame;
        if (gl.turt2Frame >= 3)
            gl.turt2Frame-= 3;
            timers.recordTime(&timers.turt2Time);
    } 
    
    glBegin(GL_QUADS);
    glTexCoord2f(tx + (gl.turt2Frame - 1) * .333,   ty + 1);
    glVertex2i(turt2.cx-w, turt2.cy-h);
    glTexCoord2f(tx + (gl.turt2Frame - 1) * .333,   ty    );
    glVertex2i(turt2.cx-w, turt2.cy+h);
    glTexCoord2f(tx + (gl.turt2Frame* .333),       ty    );
    glVertex2i(turt2.cx+w, turt2.cy+h);
    glTexCoord2f(tx + (gl.turt2Frame* .333),      ty + 1);
    glVertex2i(turt2.cx+w, turt2.cy-h);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

void turt2Movement()
{
    if (gl.turt2Flag == 0)
    {
        turt2.cx++;
        if (turt2.cx >= 1000)
            gl.turt2Flag = 1;
    }
    if (gl.turt2Flag == 1)
    {
        turt2.cx--;
        if (turt2.cx < 800)
            gl.turt2Flag = 0;
    }
}

// render functions stored in here for sprites
void renderChristianSprites(int charSelect)
{
    renderMainCharacter(charSelect);
    renderShield1();
    renderSpeedboost1();
    renderHeart1();
    renderHeart2();
    renderHeart3();
    renderHeart4();
    renderTurt1();
    renderTurt2();
    turt1Movement();
    turt2Movement();
    particlePhysics(charSelect);
}

