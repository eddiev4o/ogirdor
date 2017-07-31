#ifndef _GAME_H_
#define _GAME_H_
#include "ppm.h"
//game.h
//Read and write PPM files. Plain format.
//#include <sys/types.h>
#include <time.h>
typedef double Flt;
#define rnd() (((double)rand())/(double)RAND_MAX)

struct Vec {
    double x, y, z;
};

struct Shape {
    public:
        float width, height;
        float radius;
        Vec center;
};

struct NameBox
{
    Shape box[1];
};

struct Particle
{
    float cx, cy, velocity;
    int direction;
    Particle() {
        direction = 0;
        cx = 0;
        cy = 0;
    }
};

struct Game
{
    Shape box[5];
    int n;

};

enum State {
    STATE_NONE,
    STATE_MENU,
    STATE_GAMEPLAY,
    STATE_PAUSE,
    STATE_GAMEOVER,
    STATE_COMPLETION
};

extern class Timers
{
    public:
        double physicsRate;
        double oobillion;
        struct timespec timeStart, timeEnd, timeCurrent;
        struct timespec maincharacterTime;
        struct timespec turt1Time;
        struct timespec gameTime;
        struct timespec timeOut;
        Timers()
        {
            physicsRate = 1.0 / 30.0;
            oobillion = 1.0 / 1e9;
        }
        double timeDiff(struct timespec *start, struct timespec *end)
        {
            return (double)(end->tv_sec - start->tv_sec ) +
                (double)(end->tv_nsec - start->tv_nsec) * oobillion;
        }
        void timeCopy(struct timespec *dest, struct timespec *source)
        {
            memcpy(dest, source, sizeof(struct timespec));
        }
        void recordTime(struct timespec *t)
        {
            clock_gettime(CLOCK_REALTIME, t);
        }
} timers;

extern class Global {
    public:
        int m_delay;
        int m_walkFrame;
        int ss_walkFrame;
        int ss_delay;
        int done;
        int keys[65536];
        int xres, yres;
        int initDone;
        int walk;
        int godzillawf;
        int birdwf;
        double godzilladelay;
        double birddelay;
        int directionFlag, attackFlag, isJumpingFlag, jumpDirectionFlag;
        //Cesar 
        int facing; // where my character is facing
        int initialJumpCy, finalJumpCy;
        Particle particle[30];
	Particle godzillaP[3000];
        Particle taco[100]; // recycling Capusi structure
        float jumpRate, jumpHeight;
        float particleVelocity;
	float godzillaVelocity;

        //stuff that will be used for tacos
        //===================================
        int taco_count;
        float taco_velocity;
        int ob_particle_velocity;
	int godzillaPcount;
        int particleCount;
        float particleWidth, particleHeight;
	float godzillaPw, godzillaPh;
        //====================================

        int mcharFrame;
        int turt1Frame;
        double delay;
        int menu_position;
        int characterSelect;
        Flt movementSpeed;
        int levelSelect;
        char p_name[100];
        Flt camera[2];
        Vec box[20];
        int timeOutFrame;
        int gameFrame;
        int gameDelay;
        int minutes;
        int score;
        State state;
        Flt xc[2];
        Flt yc[2];

        Ppmimage *maincharacterImage;
        Ppmimage *maincharacter1Image;
        Ppmimage *maincharacter2Image;
        Ppmimage *maincharacter3Image;
        Ppmimage *maincharacter4Image;
        Ppmimage *mainmenubackgroundImage;
        Ppmimage *birdImage;
        Ppmimage *turretImage; 
        Ppmimage *enemy1Image;
        Ppmimage *godzillaImage;
        Ppmimage *godzillaballImage;
        Ppmimage *shooting_star_image;
        Ppmimage *mari_image; 
        Ppmimage *female_image; 
        Ppmimage *obama_image; 
        Ppmimage *sun_image; 
        Ppmimage *logoImage;
        Ppmimage *playImage;
        Ppmimage *tutorialImage;
        Ppmimage *highscoresImage;
        Ppmimage *creditsImage;
        Ppmimage *exitImage;
        Ppmimage *texthighlightImage;
        Ppmimage *levelselectionImage;
        Ppmimage *characterselectionImage;
        Ppmimage *frameImage;
        Ppmimage *backgroundImage;	
        Ppmimage *platformImage;
        Ppmimage *lavaImage;
        Ppmimage *metalImage;
        Ppmimage *desertImage;
        Ppmimage *sandImage;
        Ppmimage *gameoverImage;	
        Ppmimage *levelCompleteImage;
        Ppmimage *BronzeImage;
        Ppmimage *SilverImage;
        Ppmimage *GoldImage;
        Ppmimage *PlatinumImage;
        Ppmimage *noobImage;
        Ppmimage *WelcomeImage;
        Ppmimage *attackdmgImage;
        Ppmimage *blueenemyImage;
        Ppmimage *greenenemyImage;
        Ppmimage *heartaddImage;
        Ppmimage *redenemyImage;
        Ppmimage *shieldImage;
        Ppmimage *speedboostImage;
        Ppmimage *tabkeyImage;
        Ppmimage *leftarrowkeyImage;
        Ppmimage *rightarrowkeyImage;
        Ppmimage *spacebarkeyImage;
        Ppmimage *uparrowkeyImage;
        Ppmimage *enterkeyImage;
        Ppmimage *blueboxImage;
        Ppmimage *americaballImage;
        Ppmimage *moltenballImage;
        Ppmimage *purpleballImage;
        Ppmimage *mainchar1headImage;
        Ppmimage *mainchar2headImage;
        Ppmimage *mainchar3headImage;
        Ppmimage *mainchar4headImage;
        Ppmimage *pkeyImage;
        Ppmimage *ekeyImage;
        Ppmimage *keyboardImage;

        GLuint tempBackgroundTexture;
        GLuint tempMainCharacterTexture;
        GLuint tempHeadTexture;
        GLuint mcharTexture;            // (m)ain character
        GLuint mchar1Texture;
        GLuint mchar2Texture;
        GLuint mchar3Texture;
        GLuint mchar4Texture;
        GLuint mainmenubackgroundTexture;
        GLuint logoTexture;
        GLuint playTexture;
        GLuint tutorialTexture;
        GLuint highscoresTexture;
        GLuint creditsTexture;
        GLuint exitTexture;
        GLuint texthighlightTexture;
        GLuint levelselectionTexture;
        GLuint characterselectionTexture;
        GLuint frameTexture;
        GLuint turretTexture; 
        GLuint enemy1Texture;
        GLuint godzillaTexture;
        GLuint godzillaballTexture;
		GLuint birdTexture;
        GLuint mari_Texture;
        GLuint female_Texture;
        GLuint shooting_star_Texture;
        GLuint obama_Texture;
        GLuint sun_Texture;
        GLuint backgroundTexture;
        GLuint platformTexture;
        GLuint lavaTexture;
        GLuint metalTexture;
        GLuint desertTexture;
        GLuint sandTexture;
        GLuint gameoverTexture;
        GLuint levelCompleteTexture;
        GLuint BronzeTexture;
        GLuint SilverTexture;
        GLuint GoldTexture;
        GLuint PlatinumTexture;
        GLuint noobTexture;
        GLuint WelcomeTexture;
        GLuint attackdmgTexture;
        GLuint blueenemyTexture;
        GLuint greenenemyTexture;
        GLuint heartaddTexture;
        GLuint redenemyTexture;
        GLuint shieldTexture;
        GLuint speedboostTexture;
        GLuint tabkeyTexture;
        GLuint leftarrowkeyTexture;
        GLuint uparrowkeyTexture;
        GLuint rightarrowkeyTexture;
        GLuint spacebarkeyTexture;
        GLuint enterkeyTexture;
        GLuint blueboxTexture;
        GLuint americaballTexture;
        GLuint moltenballTexture;
        GLuint purpleballTexture;
        GLuint tempTexture;
        GLuint mainchar1headTexture;
        GLuint mainchar2headTexture;
        GLuint mainchar3headTexture;
        GLuint mainchar4headTexture;
        GLuint pkeyTexture;
        GLuint ekeyTexture;
        GLuint keyboardTexture;

        bool display_startmenu;
        bool display_tutorialmenu;
        bool display_tutorialmenu2;
        bool display_tutorialmenu3;
        bool display_highscoresmenu;
        bool display_creditsmenu;
        bool display_characterselectionmenu;
        bool display_levelselectionmenu;
        bool display_playernamemenu;
        bool display_playername;
        bool display_startinggame;

        bool turt1Flag;
        bool cantMove;
        bool oneOffShootFlag;
        bool isPressed; // for now, change later
        bool shield1Flag;
        bool heart1Flag;
        bool heart2Flag;
        bool heart3Flag;
        bool heart4Flag;
        bool speedboost1Flag;

        Global() {

            taco_count = 0;
            m_walkFrame=0;
            ss_walkFrame=0;
            m_delay = 0;
            state = STATE_GAMEPLAY;
            camera[0] = camera[1] = 0.0;
            gameDelay = 1;
            minutes = 0;
            score = 0;
            godzillawf = 0;
            birdwf = 0;
            godzilladelay = 0.1;
            birddelay = 0.5;
            gameFrame = 0;
            timeOutFrame = 0;
            attackFlag = 0;
            particleVelocity = 6;
	    godzillaVelocity = 6;
            //use this one instead of pV
            ob_particle_velocity = 6;
            particleCount = 0;
	    godzillaPcount = 0;
            particleWidth = 7;
            particleHeight = 7;
	    godzillaPw = 7;
	    godzillaPh = 7;
            directionFlag = 0;
            isJumpingFlag = 0;
            facing = 0; // cesar , where character is facing
            jumpDirectionFlag = 0;
            jumpRate = .06;
            jumpHeight = 100;
            movementSpeed = 2;
            initialJumpCy = 0;
            finalJumpCy = 0;
            done=0;
            xres=800;
            yres=600;
            walk=0;
            menu_position = 1;
            characterSelect = 0;
            levelSelect = 0;
            turt1Frame=0;
            mcharFrame=0;
            delay= 0.06;
            initDone = 1;
            p_name[0] = '\0';

            turt1Flag = 0;
            cantMove = false;
            isPressed = false;
            oneOffShootFlag = false;
            shield1Flag = 1;
            speedboost1Flag = 1;
            heart1Flag = 1;
            heart2Flag = 1;
            heart3Flag = 1;
            heart4Flag = 1;

            maincharacterImage = NULL;
            maincharacter1Image = NULL;
            maincharacter2Image = NULL;
            maincharacter3Image = NULL;
            maincharacter4Image = NULL;
            mainmenubackgroundImage = NULL;
            turretImage = NULL; 
            enemy1Image = NULL;
            godzillaImage = NULL;
            godzillaballImage = NULL;
			birdImage = NULL;
            mari_image = NULL; 
            female_image = NULL; 
            shooting_star_image=NULL;
            obama_image = NULL; 
            sun_image = NULL; 
            logoImage = NULL;
            playImage = NULL;
            tutorialImage = NULL;
            highscoresImage = NULL;
            creditsImage = NULL;
            exitImage = NULL;
            texthighlightImage = NULL;
            levelselectionImage = NULL;
            characterselectionImage = NULL;
            frameImage = NULL;
            gameoverImage = NULL;
            lavaImage = NULL;
            metalImage = NULL;
            sandImage = NULL;
            desertImage = NULL;
            backgroundImage = NULL;
            platformImage = NULL;
            levelCompleteImage = NULL;
            BronzeImage = NULL;
            SilverImage = NULL;
            GoldImage = NULL;
            PlatinumImage = NULL;
            noobImage = NULL;
            WelcomeImage = NULL;
            attackdmgImage = NULL;
            blueenemyImage = NULL;
            greenenemyImage = NULL;
            heartaddImage = NULL;
            redenemyImage = NULL;
            shieldImage = NULL;
            speedboostImage = NULL;
            tabkeyImage = NULL;
            leftarrowkeyImage = NULL;
            rightarrowkeyImage = NULL;
            spacebarkeyImage = NULL;
            uparrowkeyImage = NULL;
            enterkeyImage = NULL;
            blueboxImage = NULL;
            americaballImage = NULL;
            moltenballImage = NULL;
            purpleballImage = NULL;
            mainchar1headImage = NULL;
            mainchar2headImage = NULL;
            mainchar3headImage = NULL;
            mainchar4headImage = NULL;
            pkeyImage = NULL;
            ekeyImage = NULL;
            keyboardImage = NULL;

            display_startmenu = true;
            display_tutorialmenu = false;
            display_tutorialmenu2 = false;
            display_tutorialmenu3 = false;
            display_highscoresmenu = false;
            display_creditsmenu = false;
            display_characterselectionmenu = false;
            display_levelselectionmenu = false;
            display_playernamemenu = false;
            display_playername = false;
            display_startinggame = false;

            for (int i = 0; i<20; i++) {
                box[i].x = rnd() * xres;
                box[i].y = rnd() * (yres - 100) + 100;
                box[i].z = 0.0;
            }
            memset(keys, 0, 65536); //
        }
        ~Global() {

        }
} gl;

extern class Level {
    public:
        unsigned char arr[21][700];
        int nrows, ncols;
        int dynamicHeight[180];
        int tilesize[2];
        Flt ftsz[2];
        Flt tile_base;
        Level() {
            for (int i=0; i < 180; i++) {
                dynamicHeight[i] = -1;
            }
            //Log("Level constructor\n");
            tilesize[0] = 32;
            tilesize[1] = 32;
            ftsz[0] = (Flt)tilesize[0];
            ftsz[1] = (Flt)tilesize[1];
            tile_base = (gl.yres/gl.yres);
            //read level
            if (gl.levelSelect == 0) {
                FILE *fpi = fopen("level.txt","r");
                if (fpi) {
                    nrows=0;
                    char line[700];
                    while (fgets(line, 700, fpi) != NULL) {
                        removeCrLf(line);
                        int slen = strlen(line);
                        ncols = slen;
                        //Log("line: %s\n", line);
                        for (int j=0; j<slen; j++) {
                            arr[nrows][j] = line[j];
                        }
                        ++nrows;
                    }
                    fclose(fpi);
                    //printf("nrows of background data: %i\n", nrows);
                }
            }
        }
        void removeCrLf(char *str) {
            char *p = str;
            while (*p) {
                if (*p == 10 || *p == 13) {
                    *p = '\0';
                    break;
                }
                ++p;
            }
        }
} lev;

extern class UserInput
{
    public:
        int location[1];
        int size;
        char player_name[15];
        UserInput()
        {
            location[0] = 0;
            player_name[0] = '\0';
            size = 32;
        }
} input;

extern class Sprite {
    public:
        Flt pos[2];
        Flt vel[2];
        float cx;
        float cy;
        float height;
        float width;
        int health;
        Sprite () {
            height = 20;
            width = 20;
            health = 10;
            cx = gl.xres/2;
            cy = 100;
        }
} turt1, heart4, heart3, heart2, heart1, speedboost1,
    shield1, mainChar, turret, enemy1, mariEnemy,
    godzilla, sun, female, obama,shooting_star, bird;
#endif


