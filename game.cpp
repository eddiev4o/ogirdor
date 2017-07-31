/*===============================================
//                  Ogirdor
//===============================================
Created By:
Rodrigo Garcia-Novoa
Christian Capusi
Eddie Velasco
Cesar Loya
Simran Singh

Original Framework by:
Gordon Griesel

//===============================================
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif
#include "ppm.h"
#include "fonts.h"
#include "game.h"
using namespace std;

//defined types
//typedef double Vec[3];
typedef Flt Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                             (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1

//===============================================
// Global Instance
//===============================================
Timers timers;
Global gl;
UserInput input;
Level lev;
Sprite turt2, turt1, heart4, heart3, heart2, heart1, speedboost1, shield1, mainChar, turret, turretbeam, enemy1, mariEnemy, godzilla, female, obama, sun,shooting_star,taco ,bird;
Particle particle[20];
Game game;
//X Windows variables
Display *dpy;
Window win;

//function prototypes
void initXWindows(void);
void init();
void initOpengl(void);
void cleanupXWindows(void);
void checkResize(XEvent *e);
void checkKeys(XEvent *e);
void physics(void);

void render(void);

extern void logo(int,int);
extern void start_menu(int, int);
extern void tutorial_menu(int, int);
extern void tutorial_menu2(int, int);
extern void tutorial_menu3(int, int);   
extern void characterselection_menu(int, int);
extern void levelselection_menu(int, int);
extern void credits_screen(int, int);
extern void shootingPoseLeft(int);
extern void shootingPoseRight(int);
extern void playername_menu(int, int, char [], UserInput &input);
extern void getPlayerName(int, UserInput &input);
extern void assign_playername(char [], UserInput &input);
extern void PlayerStart(int, char [], UserInput &input);
extern void removePPM(void);
extern void godzillaphysics(void);
extern void taco_physics(void);
extern void birdphysics(void);
extern void mari_physics(void);
extern void shooting_star_physics(void);

//extern void generateTextures(void);

//extern Ppmimage *characterImage(int);
extern Ppmimage *turretImage();
extern Ppmimage *turretbeamImage();
extern Ppmimage *enemy1image();
extern Ppmimage *godzillaimage();
extern Ppmimage *godzillaballimage();
extern Ppmimage *birdImage();
extern Ppmimage *mari_image();

//extern Ppmimage *female_image();
//extern Ppmimage *sun_image();
extern Ppmimage *shooting_star_image();
extern Ppmimage *obama_image();
extern Ppmimage *taco_image();
extern void shootWalkRight(float,float,float,float,float,float);
extern void shootWalkLeft(float,float,float,float,float,float);
extern void shootStandRight(float,float,float,float,float,float);
extern void shootStandLeft(float,float,float,float,float,float);
extern void standRight(float,float,float,float,float,float);
extern void standLeft(float,float,float,float,float,float);
extern void renderChristianSprites(int);
extern void christianInit();
extern void particleMove();
extern void clearScreen();
extern void moveLevelRight();
extern void moveLevelLeft();
extern void moveSpriteRight(Sprite *);
extern void moveSpriteLeft(Sprite *);
extern void csound(const char *a);
extern void showTurret();
extern void showturretBeam();
extern void showenemy1();
extern void showgodzilla();
extern void showbird();
extern void show_mari();
extern void show_taco();
//extern void show_female();
//extern void show_sun();
extern void show_obama();
extern void show_shooting_star();

extern void renderBackground(int);
extern void renderPlatform();
extern void healthBar(int, int);
extern void renderTiles();
extern void renderTimeDisplay();
extern void pauseScreen();
extern void levelCompletion();
extern void scoreTime();
extern void deathScreen();
extern void renderScore();
extern void hudHealth();
extern void renderCoin(Sprite *);
extern void eddieInit();
extern void CesarInit();
//-----------------------------------------------------------------------------
//Setup timers
//-----------------------------------------------------------------------------

int main(void)
{
    initXWindows();
    initOpengl();
    init();
    csound("./sound/a.wav");
    while (!gl.done) {
        while (XPending(dpy)) {
            XEvent e;
            XNextEvent(dpy, &e);
            checkResize(&e);
            //checkMouse(&e);
            checkKeys(&e);
        }
        if (gl.state == STATE_GAMEPLAY) {
            physics();
        }
        if (gl.state == STATE_MENU) {
            gl.camera[0] = 0.0;
            gl.camera[1] = 0.0;
            gl.minutes = 0.0;
            gl.gameFrame = 0.0;
            gl.score = 0.0;
            gl.xc[0] = 0.0;
            gl.yc[0] = 0.0;
            gl.xc[1] = 1.0;
            gl.yc[1] = 1.0;
            mainChar.health = 10;
            init();
        }
        render();
        glXSwapBuffers(dpy, win);
    }
    removePPM();
    cleanupXWindows();
    cleanup_fonts();
    return 0;
}

void init()
{
    //put initial x coordinates of enemies here.
    //can be offscreen. You can also set y here
    //gl.camera[1] = 0.0;
    //gl.camera[0] = 0.0;
    turt1.cx = 200;
    turt1.cx = 90;
    enemy1.cx = 600;
    enemy1.cy = 90;
    godzilla.cx = 1000;
    godzilla.cy = 90;
    bird.cx = 900;
    bird.cy = 10;	
    mainChar.pos[0] = 300.0;
    mainChar.pos[1] = mainChar.pos[2] = 0.0;
    mainChar.vel[0] = mainChar.vel[1] = mainChar.vel[2] = 0.0;
    mariEnemy.cx = 100;
    mariEnemy.cy = 0;
    shooting_star.cy = 300;
    obama.cy = 0;
    turret.cx = 300;
    turret.cy = 90;
    turretbeam.cx = 700;
    turretbeam.cy = 100;
    christianInit();
    CesarInit();
    eddieInit();
}

void cleanupXWindows(void)
{
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

void setTitle(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "3350 - Ogirdor");
}

void setupScreenRes(const int w, const int h)
{
    gl.xres = w;
    gl.yres = h;
}

void initXWindows(void)
{
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
    XSetWindowAttributes swa;
    setupScreenRes(gl.xres, gl.yres);
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        printf("\n\tcannot connect to X server\n\n");
        exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if (vi == NULL) {
        printf("\n\tno appropriate visual found\n\n");
        exit(EXIT_FAILURE);
    } 
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
        StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
            vi->depth, InputOutput, vi->visual,
            CWColormap | CWEventMask, &swa);
    GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
    setTitle();
}

void reshapeWindow(int width, int height)
{
    //window has been resized.
    setupScreenRes(width, height);
    //
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
    setTitle();
}

unsigned char *buildAlphaData(Ppmimage *img)
{
    //add 4th component to RGB stream...
    int i;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    unsigned char a,b,c;
    //use the first pixel in the image as the transparent color.
    unsigned char t0 = *(data+0);
    unsigned char t1 = *(data+1);
    unsigned char t2 = *(data+2);
    for (i=0; i<img->width * img->height * 3; i+=3) {
        a = *(data+0);
        b = *(data+1);
        c = *(data+2);
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;
        *(ptr+3) = 1;
        if (a==t0 && b==t1 && c==t2)
            *(ptr+3) = 0;
        //-----------------------------------------------
        ptr += 4;
        data += 3;
    }
    return newdata;
}

void initOpengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, gl.xres, gl.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
    //
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    //
    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
    //
    //load the images file into a ppm structure.
    //

    //---CHRISTIANS FUNCTION--------//
    //int characterSelect = 1; //Rodrigo, when you make selection in char selection, 
    //store it in something similar

    // Convertpng2ppm
    //system("convert ./images/mainChar.png ./images/mainChar.ppm");
    system("convert ./images/mainChar1.png ./images/mainChar1.ppm");
    system("convert ./images/mainChar2.png ./images/mainChar2.ppm");
    system("convert ./images/mainChar3.png ./images/mainChar3.ppm");
    system("convert ./images/mainChar4.png ./images/mainChar4.ppm");
    system("convert ./images/OgirdorLogo.png ./images/OgirdorLogo.ppm");
    system("convert ./images/MainMenuBackground.png ./images/MainMenuBackground.ppm");
    system("convert ./images/Play.png ./images/Play.ppm");
    system("convert ./images/Tutorial.png ./images/Tutorial.ppm");
    system("convert ./images/HighScores.png ./images/HighScores.ppm");
    system("convert ./images/Credits.png ./images/Credits.ppm");
    system("convert ./images/Exit.png ./images/Exit.ppm");
    system("convert ./images/TextHighlight.png ./images/TextHighlight.ppm");
    system("convert ./images/LevelSelection.png ./images/LevelSelection.ppm");
    system("convert ./images/CharacterSelection.png ./images/CharacterSelection.ppm");
    system("convert ./images/Frame.png ./images/Frame.ppm");	
    system("convert ./images/desertImage.png ./images/desertImage.ppm");
    system("convert ./images/sandImage.png ./images/sandImage.ppm");
    system("convert ./images/backgroundImage.png ./images/backgroundImage.ppm");
    system("convert ./images/platformImage.png ./images/platformImage.ppm");
    system("convert ./images/lavaImage.png ./images/lavaImage.ppm");
    system("convert ./images/metalImage.png ./images/metalImage.ppm");
    system("convert ./images/levelCompleteImage.png ./images/levelCompleteImage.ppm");	
    system("convert ./images/BronzeImage.png ./images/BronzeImage.ppm");
    system("convert ./images/SilverImage.png ./images/SilverImage.ppm");
    system("convert ./images/GoldImage.png ./images/GoldImage.ppm");
    system("convert ./images/PlatinumImage.png ./images/PlatinumImage.ppm");
    system("convert ./images/noobImage.png ./images/noobImage.ppm"); 
    system("convert ./images/coinImage.png ./images/coinImage.ppm"); 
    system("convert ./images/WelcomeImage.png ./images/WelcomeImage.ppm");
    system("convert ./images/gameoverImage.png ./images/gameoverImage.ppm");
    system("convert ./images/AttackDmg.png ./images/AttackDmg.ppm");
    system("convert ./images/BlueEnemy.png ./images/BlueEnemy.ppm");
    system("convert ./images/GreenEnemy.png ./images/GreenEnemy.ppm");
    system("convert ./images/HeartAdd.png ./images/HeartAdd.ppm");
    system("convert ./images/RedEnemy.png ./images/RedEnemy.ppm");
    system("convert ./images/Shield.png ./images/Shield.ppm");
    system("convert ./images/SpeedBoost.png ./images/SpeedBoost.ppm");
    system("convert ./images/TabKey.png ./images/TabKey.ppm");
    system("convert ./images/LeftArrowKey.png ./images/LeftArrowKey.ppm");
    system("convert ./images/RightArrowKey.png ./images/RightArrowKey.ppm");
    system("convert ./images/SpacebarKey.png ./images/SpacebarKey.ppm");
    system("convert ./images/UpArrowKey.png ./images/UpArrowKey.ppm");
    system("convert ./images/EnterKey.png ./images/EnterKey.ppm");
    system("convert ./images/BlueBox.png ./images/BlueBox.ppm");
    system("convert ./images/AmericaBall.png ./images/AmericaBall.ppm");
    system("convert ./images/MoltenBall.png ./images/MoltenBall.ppm");
    system("convert ./images/PurpleBall.png ./images/PurpleBall.ppm");
    system("convert ./images/mainChar1head.png ./images/mainChar1head.ppm");
    system("convert ./images/mainChar2head.png ./images/mainChar2head.ppm");
    system("convert ./images/mainChar3head.png ./images/mainChar3head.ppm");
    system("convert ./images/mainChar4head.png ./images/mainChar4head.ppm");
    system("convert ./images/PKey.png ./images/pkey.ppm");
    system("convert ./images/EKey.png ./images/ekey.ppm");
    system("convert ./images/keyboard.png ./images/keyboard.ppm");

    //===========================================================
    // Get Images	
    //===========================================================
    //gl.maincharacterImage = characterImage(gl.characterSelect);
    gl.maincharacter1Image = ppm6GetImage("./images/mainChar1.ppm");
    gl.maincharacter2Image = ppm6GetImage("./images/mainChar2.ppm");
    gl.maincharacter3Image = ppm6GetImage("./images/mainChar3.ppm");
    gl.maincharacter4Image = ppm6GetImage("./images/mainChar4.ppm");
    gl.maincharacter5Image = ppm6GetImage("./images/mainChar5.ppm");
    gl.logoImage = ppm6GetImage("./images/OgirdorLogo.ppm");
    gl.mainmenubackgroundImage = ppm6GetImage("./images/MainMenuBackground.ppm");
    gl.playImage = ppm6GetImage("./images/Play.ppm");
    gl.tutorialImage = ppm6GetImage("./images/Tutorial.ppm");
    gl.highscoresImage = ppm6GetImage("./images/HighScores.ppm");
    gl.creditsImage = ppm6GetImage("./images/Credits.ppm");
    gl.exitImage = ppm6GetImage("./images/Exit.ppm");
    gl.texthighlightImage = ppm6GetImage("./images/TextHighlight.ppm");
    gl.levelselectionImage = ppm6GetImage("./images/LevelSelection.ppm");
    gl.characterselectionImage = ppm6GetImage("./images/CharacterSelection.ppm");
    gl.frameImage = ppm6GetImage("./images/Frame.ppm");
    gl.turretImage = turretImage();
    gl.turretbeamImage = turretbeamImage();
    gl.enemy1Image = enemy1image();
    gl.godzillaImage = godzillaimage();
    gl.godzillaballImage = godzillaimage();
    gl.birdImage = birdImage();
    gl.mari_image = mari_image();
    gl.obama_image = obama_image();
    gl.taco_image = taco_image();
    //spacing is acting weird        
    gl.shooting_star_image = shooting_star_image();
    //gl.female_image = female_image();
    //gl.sun_image = sun_image();
    gl.gameoverImage = ppm6GetImage("./images/gameoverImage.ppm");
    gl.sandImage = ppm6GetImage("./images/sandImage.ppm");
    gl.desertImage = ppm6GetImage("./images/desertImage.ppm");
    gl.metalImage = ppm6GetImage("./images/metalImage.ppm");
    gl.lavaImage = ppm6GetImage("./images/lavaImage.ppm");
    gl.levelCompleteImage = ppm6GetImage("./images/levelCompleteImage.ppm");
    gl.BronzeImage = ppm6GetImage("./images/BronzeImage.ppm");
    gl.SilverImage = ppm6GetImage("./images/SilverImage.ppm");
    gl.GoldImage = ppm6GetImage("./images/GoldImage.ppm");
    gl.PlatinumImage = ppm6GetImage("./images/PlatinumImage.ppm");
    gl.noobImage = ppm6GetImage("./images/noobImage.ppm");
    gl.coinImage = ppm6GetImage("./images/coinImage.ppm");
    gl.WelcomeImage = ppm6GetImage("./images/WelcomeImage.ppm");
    gl.backgroundImage = ppm6GetImage("./images/backgroundImage.ppm");
    gl.platformImage = ppm6GetImage("./images/platformImage.ppm");
    gl.attackdmgImage = ppm6GetImage("./images/AttackDmg.ppm");
    gl.blueenemyImage = ppm6GetImage("./images/BlueEnemy.ppm");
    gl.greenenemyImage = ppm6GetImage("./images/GreenEnemy.ppm");
    gl.heartaddImage = ppm6GetImage("./images/HeartAdd.ppm");
    gl.redenemyImage = ppm6GetImage("./images/RedEnemy.ppm");
    gl.shieldImage = ppm6GetImage("./images/Shield.ppm");
    gl.speedboostImage = ppm6GetImage("./images/SpeedBoost.ppm");
    gl.tabkeyImage = ppm6GetImage("./images/TabKey.ppm");
    gl.leftarrowkeyImage = ppm6GetImage("./images/LeftArrowKey.ppm");
    gl.rightarrowkeyImage = ppm6GetImage("./images/RightArrowKey.ppm");
    gl.spacebarkeyImage = ppm6GetImage("./images/SpacebarKey.ppm");
    gl.uparrowkeyImage = ppm6GetImage("./images/UpArrowKey.ppm");
    gl.enterkeyImage = ppm6GetImage("./images/EnterKey.ppm");
    gl.blueboxImage = ppm6GetImage("./images/BlueBox.ppm");
    gl.americaballImage = ppm6GetImage("./images/AmericaBall.ppm");
    gl.applecoreImage = ppm6GetImage("./images/AppleCore.ppm");
    gl.moltenballImage = ppm6GetImage("./images/MoltenBall.ppm");
    gl.purpleballImage = ppm6GetImage("./images/PurpleBall.ppm");
    gl.mainchar1headImage = ppm6GetImage("./images/mainChar1head.ppm");
    gl.mainchar2headImage = ppm6GetImage("./images/mainChar2head.ppm");
    gl.mainchar3headImage = ppm6GetImage("./images/mainChar3head.ppm");
    gl.mainchar4headImage = ppm6GetImage("./images/mainChar4head.ppm");
    gl.mainchar4headImage = ppm6GetImage("./images/mainChar5head.ppm");
    gl.pkeyImage = ppm6GetImage("./images/pkey.ppm");
    gl.ekeyImage = ppm6GetImage("./images/ekey.ppm");
    gl.keyboardImage = ppm6GetImage("./images/keyboard.ppm");
    //===========================================================

    //===========================================================
    // Generate Textures
    //===========================================================
    //glGenTextures(1, &gl.maincharacterTexture);
    glGenTextures(1, &gl.mchar1Texture);
    glGenTextures(1, &gl.mchar2Texture);
    glGenTextures(1, &gl.mchar3Texture);
    glGenTextures(1, &gl.mchar4Texture);
    glGenTextures(1, &gl.mainmenubackgroundTexture);
    glGenTextures(1, &gl.turretTexture);
    glGenTextures(1, &gl.turretbeamTexture);
    glGenTextures(1, &gl.enemy1Texture);
    glGenTextures(1, &gl.godzillaTexture);
    glGenTextures(1, &gl.godzillaballTexture);
    glGenTextures(1, &gl.mari_Texture);
    glGenTextures(1, &gl.birdTexture);
    //glGenTextures(1, &gl.female_Texture);
    glGenTextures(1, &gl.obama_Texture);
    glGenTextures(1, &gl.taco_Texture);
    glGenTextures(1, &gl.shooting_star_Texture);
    //glGenTextures(1, &gl.sun_Texture);
    glGenTextures(1, &gl.logoTexture);
    glGenTextures(1, &gl.playTexture);
    glGenTextures(1, &gl.tutorialTexture);
    glGenTextures(1, &gl.highscoresTexture);
    glGenTextures(1, &gl.creditsTexture);
    glGenTextures(1, &gl.exitTexture);
    glGenTextures(1, &gl.texthighlightTexture);
    glGenTextures(1, &gl.levelselectionTexture);
    glGenTextures(1, &gl.characterselectionTexture);
    glGenTextures(1, &gl.frameTexture);
    glGenTextures(1, &gl.desertTexture);
    glGenTextures(1, &gl.sandTexture);
    glGenTextures(1, &gl.lavaTexture);
    glGenTextures(1, &gl.metalTexture);
    glGenTextures(1, &gl.gameoverTexture);
    glGenTextures(1, &gl.backgroundTexture);
    glGenTextures(1, &gl.platformTexture);
    glGenTextures(1, &gl.levelCompleteTexture);
    glGenTextures(1, &gl.BronzeTexture);
    glGenTextures(1, &gl.SilverTexture);
    glGenTextures(1, &gl.GoldTexture);
    glGenTextures(1, &gl.PlatinumTexture);
    glGenTextures(1, &gl.noobTexture);
    glGenTextures(1, &gl.coinTexture);
    glGenTextures(1, &gl.WelcomeTexture);
    glGenTextures(1, &gl.attackdmgTexture);
    glGenTextures(1, &gl.blueenemyTexture);
    glGenTextures(1, &gl.greenenemyTexture);
    glGenTextures(1, &gl.heartaddTexture);
    glGenTextures(1, &gl.redenemyTexture);
    glGenTextures(1, &gl.shieldTexture);
    glGenTextures(1, &gl.speedboostTexture);
    glGenTextures(1, &gl.tabkeyTexture);
    glGenTextures(1, &gl.leftarrowkeyTexture);
    glGenTextures(1, &gl.rightarrowkeyTexture);
    glGenTextures(1, &gl.spacebarkeyTexture);
    glGenTextures(1, &gl.uparrowkeyTexture);
    glGenTextures(1, &gl.enterkeyTexture);
    glGenTextures(1, &gl.blueboxTexture);    
    glGenTextures(1, &gl.americaballTexture);    
    glGenTextures(1, &gl.applecoreTexture);    
    glGenTextures(1, &gl.moltenballTexture);    
    glGenTextures(1, &gl.purpleballTexture);    
    glGenTextures(1, &gl.mainchar1headTexture);    
    glGenTextures(1, &gl.mainchar2headTexture);    
    glGenTextures(1, &gl.mainchar3headTexture);    
    glGenTextures(1, &gl.mainchar4headTexture);    
    glGenTextures(1, &gl.mainchar5headTexture);    
    glGenTextures(1, &gl.pkeyTexture);
    glGenTextures(1, &gl.ekeyTexture);
    glGenTextures(1, &gl.keyboardTexture);
    //===========================================================

    //==============================================
    // Main Character 1
    int w = gl.maincharacter1Image->width;
    int h = gl.maincharacter1Image->height;
    glBindTexture(GL_TEXTURE_2D, gl.mchar1Texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *maincharacter1Data = buildAlphaData(gl.maincharacter1Image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, maincharacter1Data);
    free(maincharacter1Data);
    unlink("./images/mainChar1.ppm");
    //==============================================

    //==============================================
    // Main Character 2
    w = gl.maincharacter2Image->width;
    h = gl.maincharacter2Image->height;
    glBindTexture(GL_TEXTURE_2D, gl.mchar2Texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *maincharacter2Data = buildAlphaData(gl.maincharacter2Image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, maincharacter2Data);
    free(maincharacter2Data);
    unlink("./images/mainChar2.ppm");
    //==============================================

    //==============================================
    // Main Character 3
    w = gl.maincharacter3Image->width;
    h = gl.maincharacter3Image->height;
    glBindTexture(GL_TEXTURE_2D, gl.mchar3Texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *maincharacter3Data = buildAlphaData(gl.maincharacter3Image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, maincharacter3Data);
    free(maincharacter3Data);
    unlink("./images/mainChar3.ppm");
    //==============================================

    //==============================================
    // Main Character 4
    w = gl.maincharacter4Image->width;
    h = gl.maincharacter4Image->height;
    glBindTexture(GL_TEXTURE_2D, gl.mchar4Texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *maincharacter4Data = buildAlphaData(gl.maincharacter4Image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, maincharacter4Data);
    free(maincharacter4Data);
    unlink("./images/mainChar4.ppm");
    //==============================================


    //==============================================
    // Main Character 5 
    w = gl.maincharacter5Image->width;
    h = gl.maincharacter5Image->height;
    glBindTexture(GL_TEXTURE_2D, gl.mchar5Texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *maincharacter5Data = buildAlphaData(gl.maincharacter5Image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, maincharacter5Data);
    free(maincharacter5Data);
    unlink("./images/mainChar5.ppm");
    //==============================================

    //==============================================
    // Main Menu Backgroound
    w = gl.mainmenubackgroundImage->width;
    h = gl.mainmenubackgroundImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.mainmenubackgroundTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *mainmenubackgroundData = 
        buildAlphaData(gl.mainmenubackgroundImage);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, mainmenubackgroundData);
    free(mainmenubackgroundData);
    unlink("./images/MainMenuBackground.ppm");
    //==============================================

    //==============================================
    //TURRET
    w = gl.turretImage->width;
    h = gl.turretImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.turretTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *turretstuff = buildAlphaData(gl.turretImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, 
            GL_RGBA, GL_UNSIGNED_BYTE, turretstuff);
    free(turretstuff);
    unlink("./images/Turret.ppm");
    //====================================================
    //turret beam;
    w = gl.turretbeamImage->width;
    h = gl.turretbeamImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.turretbeamTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *turretbeamstuff = buildAlphaData(gl.turretbeamImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, turretbeamstuff);
    free(turretbeamstuff);
    unlink("./images/beam.ppm");


    //====================================================

    //====================================================
    //ENEMY1
    w = gl.enemy1Image->width;
    h = gl.enemy1Image->height;
    glBindTexture(GL_TEXTURE_2D, gl.enemy1Texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *enemy1stuff = buildAlphaData(gl.enemy1Image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, enemy1stuff);
    free(enemy1stuff);
    unlink("./images/enemy1.ppm");
    //===================================================

    //===================================================
    //godzilla
    w = gl.godzillaImage->width;
    h = gl.godzillaImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.godzillaTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *godzillastuff = buildAlphaData(gl.godzillaImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, godzillastuff);
    free(godzillastuff);
    unlink("./images/godzilla.ppm");
    //=====================================================
    //godzill ball
    w = gl.godzillaballImage->width;
    h = gl.godzillaballImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.godzillaballTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *godzillaballstuff = buildAlphaData(gl.godzillaballImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, godzillaballstuff);
    free(godzillaballstuff);
    unlink("./images/godzillaballImage.ppm");
    //==========================================================
    //bird
    w = gl.birdImage->width;
    h = gl.birdImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.birdTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *birdstuff = buildAlphaData(gl.birdImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, birdstuff);
    free(birdstuff);
    unlink("./images/bird.ppm");

    //===================================================
    //Maricesces
    w = gl.mari_image->width;
    h = gl.mari_image->height; 
    glBindTexture(GL_TEXTURE_2D, gl.mari_Texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *mari_pointer = buildAlphaData(gl.mari_image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, mari_pointer);
    free(mari_pointer); 
    unlink("./images/Enemy_Mariachi_3.ppm");			
    //====================================================
    //Female
    //w = gl.female_image->width;
    //h = gl.female_image->height; 
    //glBindTexture(GL_TEXTURE_2D, gl.female_Texture);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //unsigned char *female_pointer = buildAlphaData(gl.female_image);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
    //		GL_RGBA, GL_UNSIGNED_BYTE, female_pointer);
    //free(female_pointer); 
    //unlink("./images/female.ppm");			
    //======================================================
    //Obama
    w = gl.obama_image->width;
    h = gl.obama_image->height; 
    glBindTexture(GL_TEXTURE_2D, gl.obama_Texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *obama_pointer = buildAlphaData(gl.obama_image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, obama_pointer);
    free(obama_pointer); 
    unlink("./images/obama.ppm");		
    ////=====================================================
    //w = gl.sun_image->width;
    //h = gl.sun_image->height; 
    //glBindTexture(GL_TEXTURE_2D, gl.sun_Texture);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //unsigned char *sun_pointer = buildAlphaData(gl.sun_image);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
    //		GL_RGBA, GL_UNSIGNED_BYTE, sun_pointer);        	
    //free(sun_pointer); 
    //unlink("./images/sun.ppm");			
    //===================================================
	//el Taco :0 
	     w = gl.taco_image->width;
     h = gl.taco_image->height;
     glBindTexture(GL_TEXTURE_2D, gl.taco_Texture);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
     unsigned char *taco_pointer = buildAlphaData(gl.taco_image);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
             GL_RGBA, GL_UNSIGNED_BYTE, taco_pointer);
     free(taco_pointer);
     unlink("./images/taco.ppm");
    //=======================================================
    //Shooting_star
    w = gl.shooting_star_image->width;
    h = gl.shooting_star_image->height;
    glBindTexture(GL_TEXTURE_2D, gl.shooting_star_Texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *shooting_star_pointer = buildAlphaData(gl.shooting_star_image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, shooting_star_pointer);
    free(shooting_star_pointer);
    unlink("./images/shooting_star.ppm");

    //===============================================================
    // Logo
    w = gl.logoImage->width;
    h = gl.logoImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.logoTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *logoData = buildAlphaData(gl.logoImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, logoData);
    free(logoData);
    unlink("./images/OgirdorLogo.ppm");
    //===============================================================

    //===============================================================
    // Play
    w = gl.playImage->width;
    h = gl.playImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.playTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *playData = buildAlphaData(gl.playImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, playData);
    free(playData);
    unlink("./images/Play.ppm");
    //===============================================================

    //===============================================================
    // Tutorial
    w = gl.tutorialImage->width;
    h = gl.tutorialImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.tutorialTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *tutorialData = buildAlphaData(gl.tutorialImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, tutorialData);
    free(tutorialData);
    unlink("./images/Tutorial.ppm");
    //===============================================================

    //===============================================================
    // High Scores
    w = gl.highscoresImage->width;
    h = gl.highscoresImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.highscoresTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *highscoresData = buildAlphaData(gl.highscoresImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, highscoresData);
    free(highscoresData);
    unlink("./images/HighScores.ppm");
    //===============================================================

    //===============================================================
    // Credits
    w = gl.creditsImage->width;
    h = gl.creditsImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.creditsTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *creditsData = buildAlphaData(gl.creditsImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, creditsData);
    free(creditsData);
    unlink("./images/Credits.ppm");
    //===============================================================

    //===============================================================
    // Exit
    w = gl.exitImage->width;
    h = gl.exitImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.exitTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *exitData = buildAlphaData(gl.exitImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, exitData);
    free(exitData);
    unlink("./images/Exit.ppm");
    //===============================================================

    //===============================================================
    // Highlight
    w = gl.texthighlightImage->width;
    h = gl.texthighlightImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.texthighlightTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *texthighlightData = buildAlphaData(gl.texthighlightImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, texthighlightData);
    free(exitData);
    unlink("./images/TextHighlight.ppm");
    //===============================================================

    //===============================================================
    // Level Selection Background
    w = gl.levelselectionImage->width;
    h = gl.levelselectionImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.levelselectionTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *levelselectionData = buildAlphaData(gl.levelselectionImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, levelselectionData);
    free(levelselectionData);
    unlink("./images/LevelSelection.ppm");
    //===============================================================

    //===============================================================
    // Character Selection Background
    w = gl.characterselectionImage->width;
    h = gl.characterselectionImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.characterselectionTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *characterselectionData = buildAlphaData(gl.characterselectionImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, characterselectionData);
    free(characterselectionData);
    unlink("./images/CharacterSelection.ppm");
    //===============================================================

    //===============================================================
    // Frame Background
    w = gl.frameImage->width;
    h = gl.frameImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.frameTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *frameData = buildAlphaData(gl.frameImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, frameData);
    free(frameData);
    unlink("./images/Frame.ppm");
    //===============================================================

    //===============================================================
    //Level Background
    w = gl.backgroundImage->width;
    h = gl.backgroundImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *backgroundData = buildAlphaData(gl.backgroundImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, backgroundData);
    free(backgroundData);
    unlink("./images/backgroundImage.ppm");
    gl.xc[0] = 0.0;
    gl.xc[1] = 1.0;
    gl.yc[0] = 0.0;
    gl.yc[1] = 1.0;
    //===============================================================

    //===============================================================
    //Level 2 Background
    w = gl.desertImage->width;
    h = gl.desertImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.desertTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *desertData = buildAlphaData(gl.desertImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, desertData);
    free(desertData);
    unlink("./images/desertImage.ppm");
    gl.xc[0] = 0.0;
    gl.xc[1] = 1.0;
    gl.yc[0] = 0.0;
    gl.yc[1] = 1.0;
    //===============================================================

    //===============================================================
    //Platform Texture
    w = gl.platformImage->width;
    h = gl.platformImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.platformTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *platformData = buildAlphaData(gl.platformImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, platformData);
    free(platformData);
    unlink("./images/platformImage.ppm");
    //===============================================================

    //===============================================================
    //Sand Platform Texture
    w = gl.sandImage->width;
    h = gl.sandImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.sandTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *sandData = buildAlphaData(gl.sandImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, sandData);
    free(sandData);
    unlink("./images/sandImage.ppm");
    //===============================================================

    //===============================================================
    //lava Texture
    w = gl.lavaImage->width;
    h = gl.lavaImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.lavaTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *lavaData = buildAlphaData(gl.lavaImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, lavaData);
    free(lavaData);
    unlink("./images/lavaImage.ppm");
    //===============================================================

    //===============================================================
    //metal Texture
    w = gl.metalImage->width;
    h = gl.metalImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.metalTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *metalData = buildAlphaData(gl.metalImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, metalData);
    free(metalData);
    unlink("./images/metalImage.ppm");
    //===============================================================

    //===============================================================
    //GameOver Texture
    w = gl.gameoverImage->width;
    h = gl.gameoverImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.gameoverTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *gameoverData = buildAlphaData(gl.gameoverImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, gameoverData);
    free(gameoverData);
    unlink("./images/gameoverImage.ppm");
    //===============================================================

    //===============================================================
    //Level Complete Texture
    w = gl.levelCompleteImage->width;
    h = gl.levelCompleteImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.levelCompleteTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *levelCompleteData = buildAlphaData(gl.levelCompleteImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, levelCompleteData);
    free(levelCompleteData);
    unlink("./images/levelCompleteImage.ppm");
    //===============================================================


    //===============================================================
    //Bronze Texture
    w = gl.BronzeImage->width;
    h = gl.BronzeImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.BronzeTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *BronzeData = buildAlphaData(gl.BronzeImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, BronzeData);
    free(BronzeData);
    unlink("./images/BronzeImage.ppm");
    //===============================================================

    //===============================================================
    //Silver Texture
    w = gl.SilverImage->width;
    h = gl.SilverImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.SilverTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *SilverData = buildAlphaData(gl.SilverImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, SilverData);
    free(SilverData);
    unlink("./images/SilverImage.ppm");
    //===============================================================

    //===============================================================
    //Gold Texture
    w = gl.GoldImage->width;
    h = gl.GoldImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.GoldTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *GoldData = buildAlphaData(gl.GoldImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, GoldData);
    free(GoldData);
    unlink("./images/GoldImage.ppm");
    //===============================================================

    //===============================================================
    //Platinum Texture
    w = gl.PlatinumImage->width;
    h = gl.PlatinumImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.PlatinumTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *PlatinumData = buildAlphaData(gl.PlatinumImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, PlatinumData);
    free(PlatinumData);
    unlink("./images/PlatinumImage.ppm");
    //===============================================================


    //===============================================================
    //Welcome Texture
    w = gl.WelcomeImage->width;
    h = gl.WelcomeImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.WelcomeTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *WelcomeData = buildAlphaData(gl.WelcomeImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, WelcomeData);
    free(WelcomeData);
    unlink("./images/WelcomeImage.ppm");
    //===============================================================

    //===============================================================
    //Noob Texture
    w = gl.noobImage->width;
    h = gl.noobImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.noobTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *noobData = buildAlphaData(gl.noobImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, noobData);
    free(noobData);
    unlink("./images/noobImage.ppm");
    //===============================================================

    //===============================================================
    //Coin Texture
    w = gl.coinImage->width;
    h = gl.coinImage->height;	
    glBindTexture(GL_TEXTURE_2D, gl.coinTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *coinData = buildAlphaData(gl.coinImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, coinData);
    free(coinData);
    unlink("./images/coinImage.ppm");
    //===============================================================

    //===============================================================
    //Attack Dmg
    w = gl.attackdmgImage->width;
    h = gl.attackdmgImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.attackdmgTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *attackdmgData = buildAlphaData(gl.attackdmgImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, attackdmgData);
    free(attackdmgData);
    unlink("./images/attackdmgImage.ppm");
    //===============================================================

    //===============================================================
    //Blue Enemy
    w = gl.blueenemyImage->width;
    h = gl.blueenemyImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.blueenemyTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *blueenemyData = buildAlphaData(gl.blueenemyImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, blueenemyData);
    free(blueenemyData);
    unlink("./images/blueenemyImage.ppm");
    //===============================================================

    //===============================================================
    //Green Enemy
    w = gl.greenenemyImage->width;
    h = gl.greenenemyImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.greenenemyTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *greenenemyData = buildAlphaData(gl.greenenemyImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, greenenemyData);
    free(greenenemyData);
    unlink("./images/greenenemyImage.ppm");
    //===============================================================

    //===============================================================
    //Heart Add
    w = gl.heartaddImage->width;
    h = gl.heartaddImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.heartaddTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *heartaddData = buildAlphaData(gl.heartaddImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, heartaddData);
    free(heartaddData);
    unlink("./images/heartaddImage.ppm");
    //===============================================================

    //===============================================================
    //Red Enemy
    w = gl.redenemyImage->width;
    h = gl.redenemyImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.redenemyTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *redenemyData = buildAlphaData(gl.redenemyImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, redenemyData);
    free(redenemyData);
    unlink("./images/redenemyImage.ppm");
    //===============================================================

    //===============================================================
    //Shield
    w = gl.shieldImage->width;
    h = gl.shieldImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.shieldTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *shieldData = buildAlphaData(gl.shieldImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, shieldData);
    free(shieldData);
    unlink("./images/shieldImage.ppm");
    //===============================================================

    //===============================================================
    //Speed Boost
    w = gl.speedboostImage->width;
    h = gl.speedboostImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.speedboostTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *speedboostData = buildAlphaData(gl.speedboostImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, speedboostData);
    free(speedboostData);
    unlink("./images/speedboostImage.ppm");
    //===============================================================

    //===============================================================
    //Tab Key
    w = gl.tabkeyImage->width;
    h = gl.tabkeyImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.tabkeyTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *tabkeyData = buildAlphaData(gl.tabkeyImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, tabkeyData);
    free(tabkeyData);
    unlink("./images/tabkeyImage.ppm");
    //===============================================================

    //===============================================================
    //Left Arrow Key
    w = gl.leftarrowkeyImage->width;
    h = gl.leftarrowkeyImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.leftarrowkeyTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *leftarrowkeyData = buildAlphaData(gl.leftarrowkeyImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, leftarrowkeyData);
    free(leftarrowkeyData);
    unlink("./images/leftarrowkeyImage.ppm");
    //===============================================================

    //===============================================================
    //Right Arrow Key
    w = gl.rightarrowkeyImage->width;
    h = gl.rightarrowkeyImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.rightarrowkeyTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *rightarrowkeyData = buildAlphaData(gl.rightarrowkeyImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, rightarrowkeyData);
    free(rightarrowkeyData);
    unlink("./images/rightarrowkeyImage.ppm");
    //===============================================================

    //===============================================================
    //Spacebar Key
    w = gl.spacebarkeyImage->width;
    h = gl.spacebarkeyImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.spacebarkeyTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *spacebarkeyData = buildAlphaData(gl.spacebarkeyImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, spacebarkeyData);
    free(spacebarkeyData);
    unlink("./images/spacebarkeyImage.ppm");
    //===============================================================

    //===============================================================
    //Up Arrow Key
    w = gl.uparrowkeyImage->width;
    h = gl.uparrowkeyImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.uparrowkeyTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *uparrowkeyData = buildAlphaData(gl.uparrowkeyImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, uparrowkeyData);
    free(uparrowkeyData);
    unlink("./images/uparrowkeyImage.ppm");
    //===============================================================

    //===============================================================
    //Enter Key
    w = gl.enterkeyImage->width;
    h = gl.enterkeyImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.enterkeyTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *enterkeyData = buildAlphaData(gl.enterkeyImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, enterkeyData);
    free(enterkeyData);
    unlink("./images/enterkeyImage.ppm");
    //===============================================================

    //===============================================================
    //Blue Box
    w = gl.blueboxImage->width;
    h = gl.blueboxImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.blueboxTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *blueboxData = buildAlphaData(gl.blueboxImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, blueboxData);
    free(blueboxData);
    unlink("./images/blueboxImage.ppm");
    //===============================================================

    //===============================================================
    //America Ball
    w = gl.americaballImage->width;
    h = gl.americaballImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.americaballTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *americaballData = buildAlphaData(gl.americaballImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, americaballData);
    free(americaballData);
    unlink("./images/americaballImage.ppm");
    //===============================================================


    //===============================================================
    //Apple Core
    w = gl.applecoreImage->width;
    h = gl.applecoreImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.applecoreTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *applecoreData = buildAlphaData(gl.applecoreImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, applecoreData);
    free(applecoreData);
    unlink("./images/applecoreImage.ppm");
    //===============================================================

    //===============================================================
    //Molten Ball
    w = gl.moltenballImage->width;
    h = gl.moltenballImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.moltenballTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *moltenballData = buildAlphaData(gl.moltenballImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, moltenballData);
    free(moltenballData);
    unlink("./images/moltenballImage.ppm");
    //===============================================================

    //===============================================================
    //Purple Ball
    w = gl.purpleballImage->width;
    h = gl.purpleballImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.purpleballTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *purpleballData = buildAlphaData(gl.purpleballImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, purpleballData);
    free(purpleballData);
    unlink("./images/purpleballImage.ppm");
    //===============================================================

    //===============================================================
    //Main Char 1 Head
    w = gl.mainchar1headImage->width;
    h = gl.mainchar1headImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.mainchar1headTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *mainchar1headData = buildAlphaData(gl.mainchar1headImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, mainchar1headData);
    free(mainchar1headData);
    unlink("./images/mainChar1head.ppm");
    //===============================================================

    //===============================================================
    //Main Char 2 Head
    w = gl.mainchar2headImage->width;
    h = gl.mainchar2headImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.mainchar2headTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *mainchar2headData = buildAlphaData(gl.mainchar2headImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, mainchar2headData);
    free(mainchar2headData);
    unlink("./images/mainChar2head.ppm");
    //===============================================================

    //===============================================================
    //Main Char 3 Head
    w = gl.mainchar3headImage->width;
    h = gl.mainchar3headImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.mainchar3headTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *mainchar3headData = buildAlphaData(gl.mainchar3headImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, mainchar3headData);
    free(mainchar3headData);
    unlink("./images/mainChar3head.ppm");
    //===============================================================

    //===============================================================
    //Main Char 4 Head
    w = gl.mainchar4headImage->width;
    h = gl.mainchar4headImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.mainchar4headTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *mainchar4headData = buildAlphaData(gl.mainchar4headImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, mainchar4headData);
    free(mainchar4headData);
    unlink("./images/mainChar4head.ppm");
    //===============================================================

    //===============================================================
    //Main Char 5 Head
    w = gl.mainchar5headImage->width;
    h = gl.mainchar5headImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.mainchar5headTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *mainchar5headData = buildAlphaData(gl.mainchar5headImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, mainchar5headData);
    free(mainchar5headData);
    unlink("./images/mainChar5head.ppm");
    //===============================================================


    //===============================================================
    //P Key
    w = gl.pkeyImage->width;
    h = gl.pkeyImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.pkeyTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *pkeyData = buildAlphaData(gl.pkeyImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, pkeyData);
    free(pkeyData);
    unlink("./images/pkey.ppm");
    //===============================================================

    //===============================================================
    //E Key
    w = gl.ekeyImage->width;
    h = gl.ekeyImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.ekeyTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *ekeyData = buildAlphaData(gl.ekeyImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, ekeyData);
    free(ekeyData);
    unlink("./images/ekey.ppm");
    //===============================================================

    //===============================================================
    //Keyboard
    w = gl.keyboardImage->width;
    h = gl.keyboardImage->height;
    glBindTexture(GL_TEXTURE_2D, gl.keyboardTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *keyboardData = buildAlphaData(gl.keyboardImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, keyboardData);
    free(keyboardData);
    unlink("./images/keyboard.ppm");
    //===============================================================

    //--------------------------------------------------------------------
}

void checkResize(XEvent *e)
{
    //The ConfigureNotify is sent by the
    //server if the window is resized.
    if (e->type != ConfigureNotify)
        return;
    XConfigureEvent xce = e->xconfigure;
    if (xce.width != gl.xres || xce.height != gl.yres) {
        //Window size did change.
        reshapeWindow(xce.width, xce.height);
    }
}

void checkKeys(XEvent *e)
{
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyRelease) {
        gl.keys[key] = 0;
        if (key == XK_Shift_L || key == XK_Shift_R)
            return;
    }
    if (e->type == KeyPress) {
        gl.keys[key] = 1;
        if (key == XK_Shift_L || key == XK_Shift_R)
            return;
    } else { 
        return;
    }

    if (gl.display_playernamemenu)
        PlayerStart(key, gl.p_name, input);

    switch(key)
    {
        case XK_Escape:
            gl.done = 1;
            break;
        case XK_Tab:
            if (gl.display_creditsmenu) {
                gl.display_creditsmenu = false;
                gl.display_startmenu = true;
                gl.keys[XK_Tab] = false;
            }
            if (gl.display_tutorialmenu) {
                gl.display_tutorialmenu = false;
                gl.display_startmenu = true;
                gl.keys[XK_Tab] = false;
            }
            if (gl.display_tutorialmenu2) {
                gl.display_tutorialmenu2 = false;
                gl.display_tutorialmenu = true;
                gl.keys[XK_Tab] = false;
            }
            if (gl.display_tutorialmenu3) {
                gl.display_tutorialmenu3 = false;
                gl.display_tutorialmenu = true;
                gl.keys[XK_Tab] = false;
            }
            break;
        case XK_p:
            if (gl.state == STATE_GAMEPLAY) {
                gl.state = STATE_PAUSE;
                break;
            }
            gl.state = STATE_GAMEPLAY;
            break;
        case XK_e:
            if (gl.state == STATE_PAUSE) {
                gl.display_startmenu = true;
                gl.display_startinggame = false;
                gl.display_levelselectionmenu = false;
                gl.display_playernamemenu = false;
                gl.menu_position = 1;
                gl.state = STATE_GAMEPLAY;
                break;
            }
        case XK_Right:
            if (gl.display_characterselectionmenu &&
                    gl.menu_position != 6)
                gl.menu_position++;
            if (gl.display_levelselectionmenu &&
                    gl.menu_position != 2)
                gl.menu_position++;
            break;
        case XK_Left:
            if ((gl.display_characterselectionmenu ||
                        gl.display_levelselectionmenu) &&
                    gl.menu_position != 1)
                gl.menu_position--;
            break;
        case XK_Down:
            if (gl.display_startmenu && gl.menu_position != 5) {
                gl.menu_position++;
            }
            if (gl.display_characterselectionmenu &&
                    (gl.menu_position >= 1 && gl.menu_position <= 3)) {
                gl.menu_position = gl.menu_position + 3;
            }
            break;
        case XK_Up:
            if (gl.display_startmenu && gl.menu_position != 1) {
                gl.menu_position--;
            }
            if (gl.display_characterselectionmenu &&
                    (gl.menu_position <= 6 && gl.menu_position >= 4)) {
                gl.menu_position = gl.menu_position - 3;
            }
            break;
        case XK_Return:
            if (gl.display_startmenu) {
                if (gl.menu_position == 1) {
                    gl.display_startmenu = false;
                    gl.display_playernamemenu = true;
                    gl.keys[XK_Return] = false;
                    //gl.display_characterselectionmenu = true;
                    //characterselection_menu(gl.xres, gl.yres);
                    //gl.display_playernamemenu = true;
                    //gl.state = CHARACTERSELECTIONMENU;
                } else if (gl.menu_position == 2) {
                    gl.display_startmenu = false;
                    gl.display_tutorialmenu = true;
                    gl.keys[XK_Return] = false;
                    //gl.menu_position = 1;
                } else if (gl.menu_position == 3) {
                    gl.display_startmenu = false;
                    gl.display_highscoresmenu = true;
                } else if (gl.menu_position == 4) {
                    gl.display_startmenu = false;
                    gl.display_creditsmenu = true;
                    gl.keys[XK_Return] = false;
                } else if (gl.menu_position == 5) {
                    gl.display_startmenu = false;
                    gl.done = 1;
                }
            }

            if (gl.display_creditsmenu) {
                if (gl.keys[XK_Tab]) {
                    gl.display_creditsmenu = false;
                    gl.display_startmenu = true;
                }
            }
    }
}

void physics(void)
{
    //Start of Height Calculation
    mainChar.pos[1] += mainChar.vel[1];
    mainChar.vel[1] -= 0.2;
    Flt dd = lev.ftsz[0];
    //Flt offy = lev.tile_base;
    //int ncols_to_render = gl.xres / lev.tilesize[0] + 2;
    int col = (int)(gl.camera[0] / dd) + (300.0 / lev.tilesize[0] + 0.0); //changed from 1.0 to 0.0
    col = col % lev.ncols;
    int hgt = 0.0;
    if (lev.dynamicHeight[col] != -1) {
        hgt = lev.dynamicHeight[col];
    } else {
        for (int i = 0; i < lev.nrows; i++) {
            if (lev.arr[i][col] != ' ') {
                hgt = i;
                lev.dynamicHeight[col] = i;
                break;
            }
        }
        //printf("%s %d \n", "Height Calculated for Column: ", col);
    }
    for (int i = 0; i < lev.nrows; i++) {
        if (lev.arr[i][col] != ' ') {
            hgt = i;
            break;
        }
    }
    //height of ball is (nrows-1-i)*tile_height + starting point.
    Flt h = lev.tilesize[1] * (lev.nrows-hgt) + lev.tile_base;
    if (mainChar.pos[1] <= h) {
        mainChar.vel[1] = 0.0;
        mainChar.pos[1] = h;
    }
    //End of Height


    if (gl.walk && gl.directionFlag == 0) {
        //man is walking...
        //when time is up, advance the frame.
        timers.recordTime(&timers.timeCurrent);
        double timeSpan =
            timers.timeDiff(&timers.maincharacterTime,
                    &timers.timeCurrent);
        if (timeSpan > gl.delay) {
            //advance
            ++gl.mcharFrame;
            if (gl.mcharFrame >= 8)
                gl.mcharFrame -= 8;
            timers.recordTime(&timers.maincharacterTime);
        }   
    } else if (gl.walk && gl.directionFlag == 1) {
        //man is walking...
        //when time is up, advance the frame.
        timers.recordTime(&timers.timeCurrent);
        double timeSpan =
            timers.timeDiff(&timers.maincharacterTime,
                    &timers.timeCurrent);
        if (timeSpan > gl.delay) {
            //advance
            ++gl.mcharFrame;
            if (gl.mcharFrame >= 8)
                gl.mcharFrame -= 8;
            timers.recordTime(&timers.maincharacterTime);
        }
    }
    if (gl.walk || gl.keys[XK_Right]) {
        //man is walking...
        //when time is up, advance the frame.
        moveSpriteLeft(&turt1);
        moveSpriteLeft(&turt2);
        moveSpriteLeft(&mariEnemy);
        moveSpriteLeft(&female);
        moveSpriteLeft(&shooting_star);
        moveSpriteLeft(&obama);
        moveSpriteLeft(&taco);
        moveSpriteLeft(&sun);
        moveSpriteLeft(&heart1);
        moveSpriteLeft(&heart2);
        moveSpriteLeft(&heart3);
        moveSpriteLeft(&heart4);
        moveSpriteLeft(&shield1);
        moveSpriteLeft(&speedboost1);
	for (int i = 0; i < 100; i++) {
		moveSpriteLeft(&gl.coins[i]);
	}
        moveSpriteLeft(&turret);
        moveSpriteLeft(&turretbeam);
        moveSpriteLeft(&enemy1);
        moveSpriteLeft(&godzilla);
        moveSpriteLeft(&bird);
        moveLevelLeft();
        timers.recordTime(&timers.timeCurrent);
        double timeSpan =
            timers.timeDiff(&timers.maincharacterTime,
                    &timers.timeCurrent);
        if (timeSpan > gl.delay) {
            ++gl.mcharFrame;
            if (gl.mcharFrame >= 8)
                gl.mcharFrame -= 8;
            timers.recordTime(&timers.maincharacterTime);
        }
        //gl.camera[0] += gl.movementSpeed;//2.0/lev.tilesize[0] * (0.05 / gl.delay);
        for (int i=0; i<20; i++) {
            gl.box[i].x -= 1.0 * (0.05 / gl.delay);
            if (gl.box[i].x < -10.0)
                gl.box[i].x += gl.xres + 10.0;
            //if (gl.camera[0] < 1.0)
            //    gl.camera[0] = 1.0;
        }
        gl.xc[0] += 0.001;
        gl.xc[1] += 0.001;
    }
    if (gl.walk || gl.keys[XK_Left]) {
        moveSpriteRight(&turt1);
        moveSpriteRight(&turt2);
        moveSpriteRight(&mariEnemy);
        moveSpriteRight(&shooting_star);
        moveSpriteRight(&female);
        moveSpriteRight(&sun);
        moveSpriteRight(&obama);
        moveSpriteRight(&taco);
        moveSpriteRight(&heart1);
        moveSpriteRight(&heart2);
        moveSpriteRight(&heart3);
        moveSpriteRight(&heart4);
        moveSpriteRight(&shield1);	
	for (int i = 0; i < 100; i++) {
		moveSpriteRight(&gl.coins[i]);
	}
        moveSpriteRight(&speedboost1);
        moveSpriteRight(&turret);
        moveSpriteRight(&turretbeam);
        moveSpriteRight(&enemy1);
        moveSpriteRight(&godzilla);
        moveSpriteRight(&bird);
        moveLevelRight();
        timers.recordTime(&timers.timeCurrent);
        double timeSpan =
            timers.timeDiff(&timers.maincharacterTime,
                    &timers.timeCurrent);
        if (timeSpan > gl.delay) {
            ++gl.mcharFrame;
            if (gl.mcharFrame >= 8)
                gl.mcharFrame -= 8;
            timers.recordTime(&timers.maincharacterTime);
        }
        for (int i=0; i<20; i++) {
            gl.box[i].x += 1.0 * (0.05 / gl.delay);
            if (gl.box[i].x > gl.xres + 10.0)
                gl.box[i].x -= gl.xres + 10.0;
            
	    //if (gl.camera[0] < 1.0)
            //    gl.camera[0] = 0.0;
        }
        if (gl.camera[0] > 0) {
            gl.xc[0] -= 0.001;
            gl.xc[1] -= 0.001;
        }
    }
    godzillaphysics();
    birdphysics();
    mari_physics();
    taco_physics();
    moveSpriteRight(&shooting_star);
}

void render(void)
{

    if (gl.display_startmenu) {
        start_menu(gl.xres, gl.yres);
        cout << "start menu" << endl;
        gl.state = STATE_MENU;
    }

    if (gl.display_tutorialmenu) {
        tutorial_menu(gl.xres, gl.yres);
        cout << "tutorial one" << endl;
    }

    if (gl.display_tutorialmenu2) {
        tutorial_menu2(gl.xres, gl.yres);
        cout << "tutorial two" << endl;
    }

    if (gl.display_tutorialmenu3) {
        tutorial_menu3(gl.xres, gl.yres);
        cout << "tutorial three" << endl;
    }

    if (gl.display_playernamemenu) {
        playername_menu(gl.xres, gl.yres, input.player_name, input);
        cout << "Player name: " << input.player_name << endl;
        cout << "player name menu" << endl;
    }

    if (gl.display_characterselectionmenu) {
        characterselection_menu(gl.xres, gl.yres);
        cout << "character selection" << endl;
    }

    if (gl.display_levelselectionmenu) {
        levelselection_menu(gl.xres, gl.yres);
        cout << "level selection" << endl;
    }

    if (gl.display_creditsmenu) {
        cout << "credit menu" << endl;
        credits_screen(gl.xres, gl.yres);
    }

    if (gl.display_startinggame) {
        //clearScreen();    
        renderBackground(gl.levelSelect);
        renderTiles();
        //renderPlatform();
        renderChristianSprites(gl.characterSelect); 
	for (int i = 0; i < 100; i++) {
		renderCoin(&gl.coins[i]);
	}
	showTurret();
        showenemy1();
        showgodzilla();
        showbird();
        show_mari();
        // start_menu(gl.xres, gl.yres);
        show_shooting_star();
        //show_female();
        //show_sun();
        show_obama();
        hudHealth();
        healthBar(gl.xres, gl.yres);
        renderTimeDisplay();
        renderScore();
        if (gl.state == STATE_PAUSE) {
            pauseScreen();
        }
        printf("camera[0] = %f\n", gl.camera[0]);
        if (gl.camera[0] > 20000) {
            gl.state = STATE_COMPLETION;
            levelCompletion();
        }
        if (mainChar.cy < 54 || mainChar.health <= 0) {
            gl.state = STATE_GAMEOVER;
            deathScreen();
        }
    }
}
