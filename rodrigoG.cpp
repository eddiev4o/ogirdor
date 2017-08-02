// Created By: Rodrigo Garcia-Novoa
// Date: 6/15/17
// Purpose: Video Game Project Side Scroller

//===============================================
// Responsibilities and What the code does
//===============================================
// Create Main Menu
// Create Character Selection Menu
// Create Level Selection Menu
// Displaying

//===============================================
// Week 4 Progress
//===============================================
// Main Menu, Level Selection Menu, and Character
// Selection Menu both display and scroll. 
// Working on the logic for changing the display
// of screens with flags.
//===============================================

//===============================================
// Week 5 Progress
//===============================================
// Added Main Menu Background Image
//===============================================

//===============================================
// Week 6 Progress
//===============================================
// Trying menu system logic
// Finished Player name input for high scores
//===============================================

//===============================================
// Week 7 Progress
//===============================================
// 80% menu logic working
// Forward and backward menu flow
// Tutorial Page
//===============================================

//===============================================
// Week 8 Progress
//===============================================
// Made Characters with projectiles:
// 	Donald Trump
// 	Cyborg
// 	Purple Helmet
// 	Molten Girl
//===============================================

//===============================================
// Week 9 Progress
//===============================================
// Tutorial Adjustment(adding border)
// Fixing player name(name of 15 chars)
// Refactoring render
// Better Credits Screen
//===============================================

#include <iostream>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
//#include "ppm.cpp"
#include "fonts.h"
#include "game.h"

using namespace std;

// Global Instance
Shape *s;
NameBox nb1; 

// Function Prototypes
void start_menu(int, int);
void playername_menu(int, int, char [], UserInput &input);
void characterselection_menu(int, int);
void levelselection_menu(int, int);
void tutorial_menu(int, int);
void tutorial_menu2(int, int);
void tutorial_menu3(int, int);
void view_highscores();
void credits_screen(int, int); 
void standingPose(int);
void getPlayerName(int, UserInput &input);
void assign_playername(char [], UserInput &input);
void PlayerStart(int, char [], UserInput &input);
void removePPM(void);
void sCharPose(int, int, int, int, GLuint, int, int);
void renderFrame(int, int, int, int, GLuint);
void blackBox(int, int, int, int);
void rWithoutAlpha(GLuint, int, int);
void rWithAlpha(int, int, int, int, GLuint);
void shotPoseRight(int cy, int cx, int h, int w, 
        GLuint texture, int ix, int iy);
void shotPoseLeft(int cy, int cx, int h, int w, 
        GLuint texture, int ix, int iy);
void mainmenuShort(GLuint, int, int);
void border(int, int, int, int, int, int, int, int, int);
void stripes(int, int, int, int, int, int, int);
int menu_position_highlight;
unsigned int white = 0xffffff;
void extern eddieInit();

void start_menu(int xres, int yres)
{
    int highlight_x;
    int highlight_y;

    if (gl.menu_position == 1) {
        highlight_x = xres/2;
        highlight_y = yres*0.6;
    } else if (gl.menu_position == 2) {
        highlight_x = xres/2;
        highlight_y = yres*0.5;
    } else if (gl.menu_position == 3) {
        highlight_x = xres/2;
        highlight_y = yres*0.4;
    } else if (gl.menu_position == 4) {
        highlight_x = xres/2;
        highlight_y = yres*0.3;
    } else if (gl.menu_position == 5) {
        highlight_x = xres/2;
        highlight_y = yres*0.2;
    }

    //Main Menu Background=============================
    rWithoutAlpha(gl.mainmenubackgroundTexture, gl.xres, gl.yres);

    //Highlight========================================
    rWithAlpha(10, 240, highlight_x, highlight_y, gl.texthighlightTexture);

    //Logo Display=====================================
    rWithAlpha(100, 275, xres/2, yres*0.8, gl.logoTexture);

    //Play Display=====================================
    rWithAlpha(50, 100, xres/2, yres*0.6, gl.playTexture);

    //Tutorial Display=================================
    rWithAlpha(50, 170, xres/2, yres*0.485, gl.tutorialTexture);

    //High Scores Display==============================
    rWithAlpha(50, 225, xres/2, yres*0.39, gl.highscoresTexture);

    //Credits Display==================================
    rWithAlpha(90, 150, xres/2, yres*0.3, gl.creditsTexture);

    //Exit Display=====================================
    rWithAlpha(50, 75, xres/2, yres*0.2, gl.exitTexture);

    if (gl.keys[XK_Return]) {
        if (gl.menu_position == 1) {
            gl.display_startmenu = false;
            gl.display_playernamemenu = true;
            gl.keys[XK_Return] = false;
        } else if (gl.menu_position == 2) {
            gl.display_startmenu = false;
            gl.display_tutorialmenu = true;
            gl.keys[XK_Return] = false;
        } else if (gl.menu_position == 3) {
            gl.display_startmenu = true;
            //gl.display_highscoresmenu = true;
        } else if (gl.menu_position == 4) {
            gl.display_startmenu = false;
            gl.display_creditsmenu = true;
            gl.keys[XK_Return] = false;
        } else if (gl.menu_position == 5) {
            gl.display_startmenu = false;
            gl.done = 1;
        }
    }	
}

void tutorial_menu(int xres, int yres)
{
    //Main Menu Background=============================
    mainmenuShort(gl.mainmenubackgroundTexture, xres, yres);

    //Black Background=================================
    blackBox(600, 600, xres/2, yres/2);

    //Attack Dmg Power Up==============================
    rWithAlpha(28, 28, xres*0.18, yres*0.31, gl.attackdmgTexture);

    //Heart Power Up===================================
    rWithAlpha(28, 28, xres*0.18, yres*0.20, gl.heartaddTexture);

    //Shield Power Up==================================
    rWithAlpha(28, 28, xres*0.54, yres*0.31, gl.shieldTexture);

    //Speed Boost Power Up=============================
    rWithAlpha(28, 28, xres*0.54, yres*0.20, gl.speedboostTexture);

    //Turret===========================================
    rWithAlpha(28, 28, xres*0.55, yres*0.49, gl.turretTexture);

    //Char 1===========================================
    sCharPose(yres*0.61, xres*0.18, 35, 35*0.903, gl.mchar1Texture, 
            gl.mcharFrame%4, 3);
    //Char 2===========================================
    sCharPose(yres*0.61, xres*0.3, 35, 35*0.903, gl.mchar2Texture, 
            gl.mcharFrame%4, 3);
    //Char 3===========================================
    sCharPose(yres*0.61, xres*0.42, 35, 35*0.903, gl.mchar3Texture, 
            gl.mcharFrame%4, 3);
    //Char 4===========================================
    sCharPose(yres*0.49, xres*0.18, 35, 35*0.903, gl.mchar4Texture, 
            gl.mcharFrame%4, 3);
    //Char 5===========================================
    sCharPose(yres*0.49, xres*0.3, 35, 35*0.903, gl.mchar5Texture, 
            gl.mcharFrame%4, 3);
    //Char 6===========================================
    sCharPose(yres*0.49, xres*0.42, 35, 35*0.903, gl.mchar4Texture, 
            gl.mcharFrame%4, 3);

    Rect r;    

    r.bot = yres*0.49;
    r.left = xres*0.68;
    r.center = yres/2;
    ggprint16(&r, 20, white, "Godzilla");

    r.bot = yres*0.49;
    r.left = xres*0.8;
    r.center = yres/2;
    ggprint16(&r, 20, white, "Mariachi");

    r.bot = yres*0.61;
    r.left = xres*0.6;
    r.center = yres/2;
    ggprint16(&r, 20, white, "Green Enemy");

    r.bot = yres*0.61;
    r.left = xres*0.8;
    r.center = yres/2;
    ggprint16(&r, 20, white, "Blue Enemy");

    r.bot = yres*0.89;
    r.left = xres*0.235;
    r.center = yres/2;
    for (int i = 0; i < 26; i++) {
        ggprint13(&r, 20, white, "");
    }

    r.bot = yres*0.89;
    r.left = xres*0.77;
    r.center = yres/2;
    for (int i = 0; i < 26; i++) {
        ggprint13(&r, 20, white, "");
    }

    //Stripes here
    stripes(18, 300, xres/2, yres*0.85, 0, 0, 1);
    stripes(15, 300, xres/2, yres*0.7, 0, 0, 1);
    stripes(15, 300, xres/2, yres*0.4, 0, 0, 1);

    r.bot = yres*0.835;
    r.left = xres/2;
    r.center = yres/2;
    ggprint16(&r, 20, white, "                 Objective                    ");

    r.bot = yres*0.77;
    r.left = xres/3;
    r.center = yres/2;
    ggprint13(&r, 20, white, "Stuff here                                    ");
    ggprint13(&r, 20, white, "and here                                      ");

    r.bot = yres*0.685;
    r.left = xres/2;
    r.center = yres/2;
    ggprint16(&r, 20, white, "Player               |                 Enemies"); 

    r.bot = yres*0.385;
    r.left = xres/2;
    r.center = yres/2;
    ggprint16(&r, 20, white, "                 Power Ups                    ");

    r.bot = yres*0.325;
    r.left = xres*0.33;
    r.center = yres/2;
    ggprint13(&r, 20, white, "         Attack Damage");
    ggprint13(&r, 20, white, "        Increase player's damage");
    ggprint13(&r, 20, white, "                                              ");
    ggprint13(&r, 20, white, "          Health");
    ggprint13(&r, 20, white, "        Increase player's health");

    r.bot = yres*0.325;
    r.left = xres*0.66;
    r.center = yres/2;
    ggprint13(&r, 20, white, "                    Shield");
    ggprint13(&r, 20, white, "                    Shields player for one hit");
    ggprint13(&r, 20, white, "                                              ");
    ggprint13(&r, 20, white, "                    Speed");
    ggprint13(&r, 20, white, "               Increase player's ");
    ggprint13(&r, 20, white, "                        movement speed");

    r.bot = yres*0.1;
    r.left = xres*0.9;
    r.center = yres/2;
    ggprint13(&r, 20, white, "1/3");

    //Border Layout
    border(0, 10, xres, yres, xres-10, yres-10, 1, 1, 1); // white edge
    border(10, 40, xres-10, yres-10, xres-40, yres-40, 0, 0, 1); // blue edge
    border(40, 50, xres-40, yres-40, xres-50, yres-50, 1, 1, 1); // white-in edge


    if (gl.keys[XK_Return]) {
        gl.display_tutorialmenu = false;
        gl.display_tutorialmenu2 = true;
        gl.keys[XK_Return] = false;
    }

    if (gl.keys[XK_Tab]) {
        if (gl.display_tutorialmenu) {
            gl.display_tutorialmenu = false;
            gl.display_startmenu = true;
            gl.keys[XK_Tab] = false;
        }
    }
} 

void tutorial_menu2(int xres, int yres)
{
    //Main Menu Background=============================
    mainmenuShort(gl.mainmenubackgroundTexture, xres, yres);

    //Black Background=================================
    blackBox(600, 600, xres/2, yres/2);

    //Keyboard==========================================
    rWithAlpha(80, 275, xres/2, yres*0.66, gl.keyboardTexture);

    //Tab Key==========================================
    rWithAlpha(25, 34, xres*0.2, yres*0.45, gl.tabkeyTexture);

    //Left Arrow Key===================================
    rWithAlpha(25, 25, xres*0.67, yres*0.23, gl.leftarrowkeyTexture);

    //Right Arrow Key==================================
    rWithAlpha(25, 25, xres*0.79, yres*0.23, gl.rightarrowkeyTexture);

    //Spacebar Key=====================================
    rWithAlpha(20, 90, xres*0.39, yres*0.23, gl.spacebarkeyTexture);

    //P Key============================================
    rWithAlpha(25, 25, xres*0.46, yres*0.45, gl.pkeyTexture);

    //E Key============================================
    rWithAlpha(25, 25, xres*0.33, yres*0.45, gl.ekeyTexture);

    //Up Arrow Key=====================================
    rWithAlpha(25, 25, xres*0.73, yres*0.31, gl.uparrowkeyTexture);

    //=================================================
    //Down Arrow Key(flips up key)=====================
    //=================================================
    int h = 25;
    int w = 25;
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glTranslatef(gl.xres*0.73, gl.yres*0.23, 0);
    glBindTexture(GL_TEXTURE_2D, gl.uparrowkeyTexture);
    glEnable(GL_ALPHA_TEST);
    //glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex2i(-w,-h);
    glTexCoord2f(0.0, 1.0); glVertex2i(-w,h);
    glTexCoord2f(1.0, 1.0); glVertex2i(w,h);
    glTexCoord2f(1.0, 0.0); glVertex2i(w,-h);
    glEnd();
    glPopMatrix();
    //glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
    //glDisable(GL_TEXTURE_2D);
    //=================================================

    //Enter Key========================================
    rWithAlpha(22, 50, xres*0.56, yres*0.34, gl.enterkeyTexture);

    Rect r;    

    r.bot = yres*0.89;
    r.left = xres*0.235;
    r.center = yres/2;
    for (int i = 0; i < 26; i++) {
        ggprint13(&r, 20, white, "");
    }

    r.bot = yres*0.89;
    r.left = xres*0.77;
    r.center = yres/2;
    for (int i = 0; i < 26; i++) {
        ggprint13(&r, 20, white, "");
    }

    r.bot = yres*0.36;
    r.left = xres/1.36;
    r.center = yres/2;
    ggprint13(&r, 20, white, "|");

    //Stripes here
    stripes(18, 300, xres/2, yres*0.85, 0, 0, 1);

    r.bot = yres*0.835;
    r.left = xres/2;
    r.center = yres/2;
    ggprint16(&r, 20, white, "                  Controls                    ");

    //Tab Key==========================================
    r.bot = yres*0.36;
    r.left = xres/5;
    r.center = yres/2;
    ggprint13(&r, 20, white, "Go Back");
    ggprint13(&r, 20, white, "a Screen");

    //E Key============================================
    r.bot = yres*0.36;
    r.left = xres/3;
    r.center = yres/2;
    ggprint13(&r, 20, white, "Exit to");
    ggprint13(&r, 20, white, "Main Menu");

    //P Key============================================
    r.bot = yres*0.38;
    r.left = xres/2.15;
    r.center = yres/2;
    ggprint13(&r, 20, white, "Pause/Resume");

    //Enter Key========================================
    r.bot = yres*0.27;
    r.left = xres/1.8;
    r.center = yres/2;
    ggprint13(&r, 20, white, "Select/Start/Accept");

    //Space Key========================================
    r.bot = yres*0.16;
    r.left = xres/2.6;
    r.center = yres/2;
    ggprint13(&r, 20, white, "Shoot");

    //Arrow Key========================================
    r.bot = yres*0.42;
    r.left = xres/1.38;
    r.center = yres/2;
    ggprint13(&r, 20, white, "   Navigate/Move");
    ggprint13(&r, 20, white, "   Jump");

    r.bot = yres*0.1;
    r.left = xres*0.9;
    r.center = yres/2;
    ggprint13(&r, 20, white, "2/3");

    //Border Layout
    border(0, 10, xres, yres, xres-10, yres-10, 1, 1, 1); // white edge
    border(10, 40, xres-10, yres-10, xres-40, yres-40, 0, 0, 1); // blue edge
    border(40, 50, xres-40, yres-40, xres-50, yres-50, 1, 1, 1); // white-in edge

    if (gl.keys[XK_Return]) {
        gl.display_tutorialmenu2 = false;
        gl.display_tutorialmenu3 = true;
        gl.keys[XK_Return] = false;
    }

    if (gl.keys[XK_Tab]) {
        if (gl.display_tutorialmenu) {
            gl.display_tutorialmenu2 = false;
            gl.display_tutorialmenu = true;
            gl.keys[XK_Tab] = false;
        }
    }
} 

void tutorial_menu3(int xres, int yres)
{
    //Main Menu Background=============================
    mainmenuShort(gl.mainmenubackgroundTexture, xres, yres);

    //Black Background=================================
    blackBox(600, 600, xres/2, yres/2);

    //Ingame===========================================
    rWithAlpha(175, 275, xres/2, yres/2, gl.ingameTexture);

    Rect r;

    r.bot = yres*0.8;
    r.left = xres*0.25;
    r.center = yres/2;
    ggprint16(&r, 20, white, "Health Bar");

    r.bot = yres*0.8;
    r.left = xres*0.5;
    r.center = yres/2;
    ggprint16(&r, 20, white, "Time");

    r.bot = yres*0.8;
    r.left = xres*0.75;
    r.center = yres/2;
    ggprint16(&r, 20, white, "Score");

    r.bot = yres*0.33;
    r.left = xres*0.5;
    r.center = yres/2;
    ggprint16(&r, 20, white, "Player");

    r.bot = yres*0.1;
    r.left = xres*0.9;
    r.center = yres/2;
    ggprint13(&r, 20, white, "3/3");

    //Border Layout
    border(0, 10, xres, yres, xres-10, yres-10, 1, 1, 1); // white edge
    border(10, 40, xres-10, yres-10, xres-40, yres-40, 0, 0, 1); // blue edge
    border(40, 50, xres-40, yres-40, xres-50, yres-50, 1, 1, 1); // white-in edge

    if (gl.keys[XK_Return]) {
        gl.display_tutorialmenu3 = false;
        gl.display_startmenu = true;
        gl.keys[XK_Return] = false;
    }

    if (gl.keys[XK_Tab]) {
        if (gl.display_tutorialmenu) {
            gl.display_tutorialmenu3 = false;
            gl.display_tutorialmenu2 = true;
            gl.keys[XK_Tab] = false;
        }
    }
}

void playername_menu(int xres, int yres, char player_name[], UserInput &input)
{
    //Main Menu Background=============================
    mainmenuShort(gl.mainmenubackgroundTexture, xres, yres);

    //unsigned int white = 0xffffff;
    //unsigned int black = 0x00000000;
    float w, h;

    //Namebox nb1;
    //Shape *s;

    nb1.box[0].width = 100;
    nb1.box[0].height = 15;
    nb1.box[0].center.x = xres/2;
    nb1.box[0].center.y = yres/2;
    glColor3ub(0, 0, 0);
    s = &nb1.box[0];
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glTranslatef(s->center.x, s->center.y, 0);
    w = s->width;
    h = s->height;
    glBegin(GL_QUADS);
    glVertex2i(-w,-h);
    glVertex2i(-w,h);
    glVertex2i(w,h);
    glVertex2i(w,-h);
    glEnd();
    glPopMatrix();

    Rect r;

    r.bot = s->center.y + 25;
    r.left = s->center.x;
    r.center = s->center.y;
    ggprint13(&r, 20, white, "ENTER PLAYER NAME");

    r.bot = s->center.y - 8;
    r.left = s->center.x;
    r.center = s->center.y;
    ggprint13(&r, 20, white, "%s", input.player_name);

    /*if (gl.keys[XK_Tab])
      {
      if (!gl.display_startinggame)
      {
      gl.display_playernamemenu = false;
      gl.display_startmenu = true;
      }
      else
      {
      if (player_name[0] == '\0')
      return;
      else
      {
      gl.display_playernamemenu = false;
      gl.display_startmenu = true;
      }
      }
      }*/

    if (gl.keys[XK_Return]) {
        gl.display_playernamemenu = false;
        gl.display_characterselectionmenu = true;
        gl.keys[XK_Return] = false;
        if (player_name[0] == '\0')
            return;
    }

    if (gl.keys[XK_Tab]) {
        gl.display_playernamemenu = false;
        gl.display_startmenu = true;
        gl.keys[XK_Tab] = false;
    }
}

void getPlayerName(int key, UserInput &input)
{
    if (strlen(input.player_name) < 15) {
        if (key >= XK_a && key <= XK_z) {
            char keyin[2];
            keyin[0] = key;
            keyin[1] = '\0';
            strcat(input.player_name, keyin);
            return;
        }
    }

    if (key == XK_BackSpace) {
        int slen = strlen(input.player_name);
        if (slen > 0) {
            input.player_name[slen-1] = '\0';
        }
        return;
    }
}

void PlayerStart(int key, char player_name[], UserInput &input)
{
    getPlayerName(key, input);
    assign_playername(player_name, input);
}

void assign_playername(char player_name[], UserInput &input)
{
    for (int i = 0; i < 15; i++)
        player_name[i] = input.player_name[i];
}

void characterselection_menu(int xres, int yres)
{
    int highlight_x;
    int highlight_y;

    if (gl.menu_position == 1) {
        highlight_x = xres*0.22;
        highlight_y = yres*0.68;
    } else if (gl.menu_position == 2) {
        highlight_x = xres*0.5;
        highlight_y = yres*0.68;
    } else if (gl.menu_position == 3) {
        highlight_x = xres*0.78;
        highlight_y = yres*0.68;
    } else if (gl.menu_position == 4) {
        highlight_x = xres*0.22;
        highlight_y = yres*0.31;
    } else if (gl.menu_position == 5) {
        highlight_x = xres*0.5;
        highlight_y = yres*0.31;
    } else if (gl.menu_position == 6) {
        highlight_x = xres*0.78;
        highlight_y = yres*0.31;
    }

    //=================================================
    //Background Display for Character Selection=======
    //=================================================
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, gl.characterselectionTexture);
    //glEnable(GL_ALPHA_TEST);
    //glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0); glVertex2i(0,0);
    glTexCoord2f(0.0, 0.0); glVertex2i(0,yres);
    glTexCoord2f(1.0, 0.0); glVertex2i(xres,yres);
    glTexCoord2f(1.0, 1.0); glVertex2i(xres,0);
    glEnd();
    glPopMatrix();
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glDisable(GL_ALPHA_TEST);
    //=================================================

    //Text Highlight===================================
    rWithAlpha(125, 125, highlight_x, highlight_y, gl.texthighlightTexture);

    //Frame 1 Display==================================
    renderFrame(80, 80, xres*0.22, yres*0.31, gl.frameTexture);

    //Frame 2 Display==================================
    renderFrame(80, 80, xres*0.5, yres*0.31, gl.frameTexture);

    //Frame 3 Display==================================
    renderFrame(80, 80, xres*0.78, yres*0.31, gl.frameTexture);

    //Frame 4 Display==================================
    renderFrame(80, 80, xres*0.22, yres*0.68, gl.frameTexture);

    //Frame 5 Display==================================
    renderFrame(80, 80, xres*0.5, yres*0.68, gl.frameTexture);

    //Frame 6 Display==================================
    renderFrame(80, 80, xres*0.78, yres*0.68, gl.frameTexture);

    //Char 1 Display===================================
    sCharPose(yres*0.68, xres*0.22, 50, 50*0.903, gl.mchar1Texture, 
            gl.mcharFrame%4, 3);

    //Char 2 Display===================================
    sCharPose(yres*0.68, xres*0.5, 50, 50*0.903, gl.mchar2Texture, 
            gl.mcharFrame%4, 3);

    //Char 3 Display===================================
    sCharPose(yres*0.68, xres*0.78, 50, 50*0.903, gl.mchar3Texture, 
            gl.mcharFrame%4, 3);

    //Char 4 Display===================================
    sCharPose(yres*0.31, xres*0.22, 50, 50*0.903, gl.mchar4Texture, 
            gl.mcharFrame%4, 3);

    //Char 5 Display===================================
    sCharPose(yres*0.31, xres*0.5, 50, 50*0.903, gl.mchar5Texture, 
            gl.mcharFrame%4, 3);

    //Char 6 Display===================================
    sCharPose(yres*0.31, xres*0.78, 50, 50*0.903, gl.mchar4Texture, 
            gl.mcharFrame%4, 3);

    Rect r;    

    r.bot = yres/2 - 250;
    r.left = xres/2;
    r.center = yres/2;
    ggprint13(&r, 20, white, "CHARACTER SELECTION");

    if (gl.keys[XK_Return]) {
        if (gl.menu_position == 1) {
            gl.characterSelect = 1;
            gl.display_characterselectionmenu = false;
            gl.display_levelselectionmenu = true;
            gl.menu_position = 1;
            gl.keys[XK_Return] = false;
        } else if (gl.menu_position == 2) {
            gl.characterSelect = 2;        // need to change later
            gl.display_characterselectionmenu = false;
            gl.display_levelselectionmenu = true;
            gl.menu_position = 1;
            gl.keys[XK_Return] = false; 
        } else if (gl.menu_position == 3) {
            gl.characterSelect = 3;        
            gl.display_characterselectionmenu = false;
            gl.display_levelselectionmenu = true;
            gl.menu_position = 1;
            gl.keys[XK_Return] = false;
        } else if (gl.menu_position == 4) {
            gl.characterSelect = 4;        
            gl.display_characterselectionmenu = false;
            gl.display_levelselectionmenu = true;
            gl.menu_position = 1;
            gl.keys[XK_Return] = false;
        } else if (gl.menu_position == 5) {
            gl.characterSelect = 5;       
            gl.display_characterselectionmenu = false;
            gl.display_levelselectionmenu = true;
            gl.menu_position = 1;
            gl.keys[XK_Return] = false;
        } else if (gl.menu_position == 6) {
            gl.characterSelect = 4;      
            gl.display_characterselectionmenu = false;
            gl.display_levelselectionmenu = true;
            gl.menu_position = 1;
            gl.keys[XK_Return] = false;
        }
    }

    if (gl.keys[XK_Tab]) {
        gl.display_characterselectionmenu = false;
        gl.display_playernamemenu = true;
        gl.menu_position = 1;
        gl.keys[XK_Tab] = false;
    }
}

void levelselection_menu(int xres, int yres)
{
    int highlight_x;
    int highlight_y;

    if (gl.menu_position == 1) {
        highlight_x = xres*0.25;
        highlight_y = yres*0.5;
    } else if (gl.menu_position == 2) {
        highlight_x = xres*0.75;
        highlight_y = yres*0.5;
    }

    //=================================================
    //Background Display for Level Selection===========
    //=================================================
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, gl.levelselectionTexture);
    //glEnable(GL_ALPHA_TEST);
    //glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0); glVertex2i(0,0);
    glTexCoord2f(0.0, 0.0); glVertex2i(0,yres);
    glTexCoord2f(1.0, 0.0); glVertex2i(xres,yres);
    glTexCoord2f(1.0, 1.0); glVertex2i(xres,0);
    glEnd();
    glPopMatrix();
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glDisable(GL_ALPHA_TEST);
    //=================================================


    //Highlight========================================
    rWithAlpha(175, 175, highlight_x, highlight_y, gl.texthighlightTexture);

    //Frame 1 Display==================================
    renderFrame(100, 100, xres*0.25, yres*0.5, gl.frameTexture);

    //Frame 2 Display==================================
    renderFrame(100, 100, xres*0.75, yres*0.5, gl.frameTexture);

    //Level 1 Display==================================
    renderFrame(72, 72, xres*0.25, yres*0.5, gl.backgroundTexture);

    //Level 2 Display==================================
    renderFrame(72, 72, xres*0.75, yres*0.5, gl.desertTexture);

    Rect r;    

    r.bot = yres/2 - 250;
    r.left = xres/2;
    r.center = yres/2;
    ggprint13(&r, 20, white, "LEVEL SELECTION");

    if (gl.keys[XK_Return]) {
        if (gl.menu_position == 1) {
            gl.levelSelect = 1;
            eddieInit();
            gl.display_levelselectionmenu = false;
            gl.display_startinggame = true;
            gl.keys[XK_Return] = false;
            gl.menu_position = 1;
            //gl.display_levelselectionmenu = false;
            gl.state = STATE_GAMEPLAY;
        } else if (gl.menu_position == 2) {
            gl.levelSelect = 2;        // need to change later
            eddieInit();
            gl.display_levelselectionmenu = false;
            gl.display_startinggame = true;
            gl.keys[XK_Return] = false;
            gl.menu_position = 1;
            //gl.display_levelselectionmenu = false;
            gl.state = STATE_GAMEPLAY;
        }
    }

    if (gl.keys[XK_Tab]) {
        gl.display_levelselectionmenu = false;
        gl.display_characterselectionmenu = true;
        gl.menu_position = 1;
        gl.keys[XK_Tab] = false;
    }
}

void credits_screen(int xres, int yres)
{

    //=================================================
    //Main Menu Background=============================
    //=================================================
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, gl.mainmenubackgroundTexture);
    //glEnable(GL_ALPHA_TEST);
    //glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0); glVertex2i(0,0);
    glTexCoord2f(0.0, 0.0); glVertex2i(0,yres);
    glTexCoord2f(1.0, 0.0); glVertex2i(xres,yres);
    glTexCoord2f(1.0, 1.0); glVertex2i(xres,0);
    glEnd();
    glPopMatrix();
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glDisable(GL_ALPHA_TEST);
    //glDisable(GL_TEXTURE_2D);
    //=================================================

    //Left Top
    shotPoseRight(401, xres*0.15, 100, 100*.903, gl.mchar5Texture, 
            gl.mcharFrame%4, 3);
    renderFrame(5, 195, xres*0.5, yres*0.6865, gl.applecoreTexture);
    //Left Mid
    shotPoseRight(278, xres*0.15, 100, 100*.903, gl.mchar3Texture, 
            gl.mcharFrame%4, 3);
    renderFrame(5, 195, xres*0.5, yres*0.478, gl.texthighlightTexture);
    //Left Bot
    shotPoseRight(153, xres*0.15, 100, 100*.903, gl.mchar4Texture, 
            gl.mcharFrame%4, 3);
    renderFrame(5, 195, xres*0.5, yres*0.2665, gl.americaballTexture);
    //Right Top
    shotPoseLeft(338, xres*0.85, 100, 100*.903, gl.mchar1Texture, 
            gl.mcharFrame%4, 3);
    renderFrame(5, 195, xres*0.5, yres*0.579, gl.purpleballTexture);
    //Right Bot
    shotPoseLeft(212, xres*0.85, 100, 100*.903, gl.mchar2Texture, 
            gl.mcharFrame%4, 3);
    renderFrame(5, 195, xres*0.5, yres*0.367, gl.moltenballTexture);

    Rect r;

    r.bot = yres*.78;
    r.left = xres/2;
    r.center = yres/2;

    //unsigned int white = 0xffffff;
    ggprint16(&r, 16, white, "Developers");
    ggprint16(&r, 16, white, "=======================");
    ggprint16(&r, 16, white, "                      ");
    ggprint13(&r, 16, white, "Lead Character Animation and Physics Developer");
    ggprint16(&r, 16, white, "                      ");
    ggprint13(&r, 16, white, "Christian Capusi");
    ggprint16(&r, 16, white, "                      ");
    ggprint13(&r, 16, white, "Senior Manager AI Developer");
    ggprint16(&r, 16, white, "                      ");
    ggprint13(&r, 16, white, "Cesar Loya");
    ggprint16(&r, 16, white, "                      ");
    ggprint13(&r, 16, white, "Lead Menu System and Graphics Developer");
    ggprint16(&r, 16, white, "                      ");
    ggprint13(&r, 16, white, "Rodrigo Garcia-Novoa");
    ggprint16(&r, 16, white, "                      ");
    ggprint13(&r, 16, white, "Yet to choose");
    ggprint16(&r, 16, white, "                      ");
    ggprint13(&r, 16, white, "Simran Singh");
    ggprint16(&r, 16, white, "                      ");
    ggprint13(&r, 16, white, "Lead Level Design Programmer - Honorary Fluffer");
    ggprint16(&r, 16, white, "                      ");
    ggprint13(&r, 16, white, "Eddie Velasco");

    if (gl.keys[XK_Tab]) {
        if (gl.display_creditsmenu) {
            gl.display_creditsmenu = false;
            gl.display_startmenu = true;
        }
    }

    if (gl.keys[XK_Return]) {
        if (gl.display_creditsmenu) {
            gl.display_creditsmenu = false;
            gl.display_startmenu = true;
            gl.keys[XK_Return] = false;
        }
    }
}

void border(int v1, int v2, int v3, int v4, int v5, 
        int v6, int c1, int c2, int c3)
{
    glPushMatrix();
    glColor3f(c1, c2, c3);
    glBegin(GL_QUAD_STRIP);
    glVertex2i(v1,v1);
    glVertex2i(v2,v2);
    glVertex2i(v3,v1);
    glVertex2i(v5,v2);
    glVertex2i(v3,v4);
    glVertex2i(v5,v6);
    glVertex2i(v1,v4);
    glVertex2i(v2,v6);
    glVertex2i(v1,v1);
    glVertex2i(v2,v2);
    glEnd();
    glPopMatrix();
}

void stripes(int h, int w, int trans_x, int trans_y, int c1, int c2, int c3)
{
    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(c1,c2,c3,1.0);
    glTranslatef(trans_x, trans_y, 0);
    glBegin(GL_QUADS);
    glVertex2i(-w,-h);
    glVertex2i(-w,h);
    glVertex2i(w,h);
    glVertex2i(w,-h);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
}

void mainmenuShort(GLuint texture, int xres, int yres)
{
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0); glVertex2i(0,0);
    glTexCoord2f(0.0, 0.0); glVertex2i(0,yres);
    glTexCoord2f(1.0, 0.0); glVertex2i(xres,yres);
    glTexCoord2f(1.0, 1.0); glVertex2i(xres,0);
    glEnd();
    glPopMatrix();
}

void standingPose(int xres)
{
    float cy = 310;
    float cx = xres/3.5;
    float h = 40.0;
    float w = h * .903;
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, gl.mchar3Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ix = gl.mcharFrame % 4;
    int iy = 3;
    glBegin(GL_QUADS);
    glTexCoord2f(ix + .25, iy + .333); glVertex2i(cx+w,cy-h);
    glTexCoord2f(ix      , iy + .333); glVertex2i(cx-w,cy-h);
    glTexCoord2f(ix      , iy       ); glVertex2i(cx-w,cy+h);
    glTexCoord2f(ix + .25, iy       ); glVertex2i(cx+w,cy+h);
    glEnd();
    glPopMatrix();
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glDisable(GL_ALPHA_TEST);
}

void shotPoseRight(int cy, int cx, int h, int w, 
        GLuint texture, int ix, int iy)
{
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(ix + .25, iy + .666); glVertex2i(cx+w,cy-h);
    glTexCoord2f(ix      , iy + .666); glVertex2i(cx-w,cy-h);
    glTexCoord2f(ix      , iy + .333); glVertex2i(cx-w,cy+h);
    glTexCoord2f(ix + .25, iy + .333); glVertex2i(cx+w,cy+h);
    glEnd();
    glPopMatrix();
}

void shotPoseLeft(int cy, int cx, int h, int w, 
        GLuint texture, int ix, int iy)
{
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(ix      , iy + .666); glVertex2i(cx+w,cy-h);
    glTexCoord2f(ix + .25, iy + .666); glVertex2i(cx-w,cy-h);
    glTexCoord2f(ix + .25, iy + .333); glVertex2i(cx-w,cy+h);
    glTexCoord2f(ix      , iy + .333); glVertex2i(cx+w,cy+h);
    glEnd();
    glPopMatrix();
}

void sCharPose(int cy, int cx, int h, int w, GLuint texture, int ix, int iy)
{
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(ix + .25, iy + .333); glVertex2i(cx+w,cy-h);
    glTexCoord2f(ix      , iy + .333); glVertex2i(cx-w,cy-h);
    glTexCoord2f(ix      , iy       ); glVertex2i(cx-w,cy+h);
    glTexCoord2f(ix + .25, iy       ); glVertex2i(cx+w,cy+h);
    glEnd();
    glPopMatrix();
} 

void renderFrame(int h, int w, int trans_x, int trans_y, GLuint texture)
{
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glTranslatef(trans_x, trans_y, 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0); glVertex2i(-w,-h);
    glTexCoord2f(0.0, 0.0); glVertex2i(-w,h);
    glTexCoord2f(1.0, 0.0); glVertex2i(w,h);
    glTexCoord2f(1.0, 1.0); glVertex2i(w,-h);
    glEnd();
    glPopMatrix();
}

void blackBox(int h, int w, int trans_x, int trans_y)
{
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslatef(trans_x, trans_y, 0);
    glBegin(GL_QUADS);
    glVertex2i(-w,-h);
    glVertex2i(-w,h);
    glVertex2i(w,h);
    glVertex2i(w,-h);
    glEnd();
    glPopMatrix();
}

void rWithoutAlpha(GLuint texture, int xres, int yres)
{
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0); glVertex2i(0,0);
    glTexCoord2f(0.0, 0.0); glVertex2i(0,yres);
    glTexCoord2f(1.0, 0.0); glVertex2i(xres,yres);
    glTexCoord2f(1.0, 1.0); glVertex2i(xres,0);
    glEnd();
    glPopMatrix();        
}

void rWithAlpha(int h, int w, int trans_x, int trans_y, GLuint texture)
{
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glTranslatef(trans_x, trans_y, 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0); glVertex2i(-w,-h);
    glTexCoord2f(0.0, 0.0); glVertex2i(-w,h);
    glTexCoord2f(1.0, 0.0); glVertex2i(w,h);
    glTexCoord2f(1.0, 1.0); glVertex2i(w,-h);
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
} 

// Problem with PPM files not unlinking
void removePPM(void)
{
    remove("./images/mainChar1.ppm");
    remove("./images/mainChar2.ppm");
    remove("./images/mainChar3.ppm");
    remove("./images/mainChar4.ppm");
    remove("./images/OgirdorLogo.ppm");
    remove("./images/MainMenuBackground.ppm");
    remove("./images/Play.ppm");
    remove("./images/Tutorial.ppm");
    remove("./images/HighScores.ppm");
    remove("./images/Credits.ppm");
    remove("./images/Exit.ppm");
    remove("./images/TextHighlight.ppm");
    remove("./images/LevelSelection.ppm");
    remove("./images/CharacterSelection.ppm");
    remove("./images/Frame.ppm");
    remove("./images/backgroundImage.ppm");
    remove("./images/platformImage.ppm");
    remove("./images/AttackDmg.ppm");
    remove("./images/BlueEnemy.ppm");
    remove("./images/GreenEnemy.ppm");
    remove("./images/HeartAdd.ppm");
    remove("./images/RedEnemy.ppm");
    remove("./images/Shield.ppm");
    remove("./images/SpeedBoost.ppm");
    remove("./images/TabKey.ppm");
    remove("./images/LeftArrowKey.ppm");
    remove("./images/RightArrowKey.ppm");
    remove("./images/SpacebarKey.ppm");
    remove("./images/UpArrowKey.ppm");
    remove("./images/EnterKey.ppm");
    remove("./images/BlueBox.ppm");
    remove("./images/AmericaBall.ppm");
    remove("./images/MoltenBall.ppm");
    remove("./images/PurpleBall.ppm");
}



