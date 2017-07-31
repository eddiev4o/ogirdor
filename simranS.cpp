//Well This code Displays few enemies
//Some of the enemies can move around
//There is sound
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <time.h>
#include "game.h"
#include "ppm.h"
#include <sys/stat.h>
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif

using namespace std;


int i = -400;

int birdm = -400;

int birdy= -300;

int turretm = -800;

class T {

    public:
        double prate;
        double obi;
        struct timespec ts, te,tc;
        struct timespec wt;
        T() {
            prate=1.0/30.0;
            obi=1.0/1e9;
        }
        double td(struct timespec *start, struct timespec *end) {
            return (double)(end->tv_sec - start->tv_sec) + 
                (double)(end->tv_nsec - start->tv_nsec) * obi;
        }
        void tcpy(struct timespec *dest, struct timespec *source) {
            memcpy(dest, source, sizeof(struct timespec));
        }
        void rt(struct timespec *t) {
            clock_gettime(CLOCK_REALTIME, t);
        }

} t, ti;


bool h = false;

void Playsound(ALuint tum) 
{
    alSourcePlay(tum);
}

void csound(const char *a) 
{

#ifdef USE_OPENAL_SOUND
    alutInit(0, NULL);
    if (alGetError() != AL_NO_ERROR) {
        printf(" There is an error  in alutInit()\n");
        return;
    }

    alGetError();

    float termoil[6] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, termoil);
    alListenerf(AL_GAIN, 1.0);

    ALuint toom;
    toom = alutCreateBufferFromFile(a); 

    ALuint tum;

    alGenSources(1, &tum);
    alSourcei(tum, AL_BUFFER, toom);

    alSourcef(tum, AL_GAIN, 0.20);
    alSourcef(tum, AL_PITCH, 1.0);
    alSourcei(tum, AL_LOOPING, 0);

    if (alGetError() != AL_NO_ERROR) {
        printf("cannot set the source\n");
        return;
    }
    Playsound(tum);
    if(h == true) {
        alDeleteSources(1, &tum);
        alDeleteBuffers(1, &toom);
    }
#endif
}

void dsound()
{ 
#ifdef USE_OPENAL_SOUND
    h = true;

    ALCcontext *stuff = alcGetCurrentContext();
    ALCdevice *yantar = alcGetContextsDevice(stuff);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(stuff);
    alcCloseDevice(yantar);

#endif
}

Ppmimage *turretImage() 
{
    system ("convert ./images/Turret.png ./images/Turret.ppm");
    return ppm6GetImage("./images/Turret.ppm");
}


Ppmimage *turretbeamImage() 
{
    system ("convert ./images/beam.png ./images/beam.ppm");
    return ppm6GetImage("./images/beam.ppm");
}

void showturretBeam() 
{
    float y = 170;
    float ht = 100.0;
    float w = ht*.5;
    int move = 200;

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.turretbeamTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    int ax = 1;
    int ay = 1;
    float tx = (float)ax / 4.0;
    float ty = (float)ay;

    glBegin(GL_QUADS);    
    glTexCoord2f(tx+.82,      ty+1); glVertex2i(turretbeam.cx+move+w, y-ht);
    glTexCoord2f(tx+.82,      ty);    glVertex2i(turretbeam.cx+move+w, y+ht);
    glTexCoord2f(tx-.35, ty);    glVertex2i(turretbeam.cx+move-w, y+ht);
    glTexCoord2f(tx-.35, ty+1); glVertex2i(turretbeam.cx+move-w, y-ht);

    if (mainChar.cx > turretbeam.cx+move-w && mainChar.cx < turretbeam.cx+move+w)
    {
        mainChar.health--;

    }
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);

}

void showTurret()
{
    int move = 600;

    if (turretm > -950) {
        turretm--;
        showturretBeam();

        if ( turretm < -949) {
            turretm = -1200;
        }
    }

    if (turretm < -950) {
        turretm++;

        if (turretm > -951) {
            turretm = -800;
        }
    }

    float y = 70;
    float ht = 30.0;
    float w = ht*2;

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.turretTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    int ax = 1;
    int ay = 1;
    float tx = (float)ax / 4.0;
    float ty = (float)ay;

    glBegin(GL_QUADS);    
    glTexCoord2f(tx,      ty+.5); glVertex2i(turret.cx+move+w, y-ht);
    glTexCoord2f(tx,      ty);    glVertex2i(turret.cx+move+w, y+ht);
    glTexCoord2f(tx+.5, ty);    glVertex2i(turret.cx+move-w, y+ht);
    glTexCoord2f(tx+.5, ty+.5); glVertex2i(turret.cx+move-w, y-ht);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

Ppmimage *enemy1image()
{
    system ("convert ./images/enemy1.png ./images/enemy1.ppm");
    return ppm6GetImage("./images/enemy1.ppm");
}

void showenemy1() 
{
    float y = 100;
    float ht = 30.0;
    float w = ht*2;

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.enemy1Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    int ax = 1;
    int ay = 1;
    float tx = (float)ax / 4.0;
    float ty = (float)ay;

    glBegin(GL_QUADS);
    glTexCoord2f(tx,      ty+.5); glVertex2i(enemy1.cx+w, y-ht);
    glTexCoord2f(tx,      ty);    glVertex2i(enemy1.cx+w, y+ht);
    glTexCoord2f(tx+.5, ty);    glVertex2i(enemy1.cx-w, y+ht);
    glTexCoord2f(tx+.5, ty+.5); glVertex2i(enemy1.cx-w, y-ht);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

void godzillaphysics(void)
{
    t.rt(&t.tc);
    double tspan = t.td(&t.wt, &t.tc);
    if (tspan > gl.godzilladelay) {
        gl.godzillawf++;
        i++;
        if (gl.godzillawf >= 3) {
            gl.godzillawf -= 3;
            i--;
        }
        t.rt(&t.wt);
    }
}


Ppmimage *godzillaimage()
{
    system ("convert ./images/godzilla.png ./images/godzilla.ppm");
    return ppm6GetImage("./images/godzilla.ppm");
}

Ppmimage *godzillaballimage()
{
    system ("convert ./images/godzillaball.png ./images/godzillaball.ppm");
    return ppm6GetImage("./images/godzillaball.ppm");
}

void showgodzilla()
{
    float y = 160;
    float ht = 100.0;
    float w = ht*2;
    int move = 2133;

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.godzillaTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    int ax = gl.godzillawf % 3;
    int ay = 0;
    if (gl.godzillawf >= 3) {
        ay = 1;
    }
    float tx = (float)ax / 3.0;
    float ty = (float)ay/ 1.0;

    if (i > -950) {
        i--;
        glBegin(GL_QUADS);
        glTexCoord2f(tx-.334,      ty+1.0); 
        glVertex3s(godzilla.cx+i+move+w, y-ht,0);
        glTexCoord2f(tx-.334,      ty);    
        glVertex3s(godzilla.cx+i+move+w, y+ht,0);
        glTexCoord2f(tx, ty);    glVertex3s(godzilla.cx+i+move-w, y+ht,0);
        glTexCoord2f(tx, ty+1.0); glVertex3s(godzilla.cx+i+move-w, y-ht,0);

        if (mainChar.cx >= godzilla.cx+i+move-w &&
                mainChar.cx <= godzilla.cx+i+move+w) {
            if (mainChar.cy <= y-ht || 
                    mainChar.cy >= y+ht || 
                    mainChar.cy == y+ht || 
                    mainChar.cy == y-ht) {
                mainChar.health = mainChar.health;
            } else {
                mainChar.health--;
            }
        }

        if( i < -949) {
            i = -1099;
        }
    }

    if ( i < -950) { 
        i++;
        glBegin(GL_QUADS);
        glTexCoord2f(tx+.33,      ty+1.0); 
        glVertex3s(godzilla.cx+i+move+w, y-ht,0);
        glTexCoord2f(tx+.33,      ty);    
        glVertex3s(godzilla.cx+i+move+w, y+ht,0);
        glTexCoord2f(tx, ty);    glVertex3s(godzilla.cx+i+move-w, y+ht,0);
        glTexCoord2f(tx, ty+1.0); glVertex3s(godzilla.cx+i+move-w, y-ht,0);

        if (mainChar.cx <= godzilla.cx+i+move-w && 
                mainChar.cx >= godzilla.cx+i+move+w) {

            if (mainChar.cy <= y-ht || 
                    mainChar.cy >= y+ht || 
                    mainChar.cy == y+ht || 
                    mainChar.cy == y-ht) {
                mainChar.health = mainChar.health;
            } else {
                mainChar.health--;
            }
        }

        if (i > -951) {
            i = -800;
        }
    }

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);

}

void birdphysics(void)
{
    ti.rt(&ti.tc);
    double tspan = ti.td(&ti.wt, &ti.tc);
    if (tspan > gl.birddelay) {
        gl.birdwf++;
        birdm++;
        birdy--;
        turretm++;
        if (gl.birdwf >= 4) {
            gl.birdwf -= 4;
            birdm--;
            turretm--;
            birdy++;
        }
        ti.rt(&ti.wt);
    }
}

Ppmimage *birdImage()
{
    system ("convert ./images/bird.png ./images/bird.ppm");
    return ppm6GetImage("./images/bird.ppm");
}

void showbird()
{
    float y = 100;
    float ht = 30.0;
    float w = ht*2;
    int move = 1700;

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.birdTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    int ax = gl.birdwf % 4;
    int ay = 0;
    if (gl.birdwf >= 4) {
        ay = 1;
    }
    float tx = (float)ax / 4.0;
    float ty = (float)ay / 1.0;

    if(birdm > -950) {
        birdy++;
        birdm--;
        glBegin(GL_QUADS);
        glTexCoord2f(tx+.25,      ty+1); 
        glVertex2i(bird.cx+birdm+move+w, y+birdy-ht);
        glTexCoord2f(tx+.25,      ty);    
        glVertex2i(bird.cx+birdm+move+w, y+birdy+ht);
        glTexCoord2f(tx, ty);    glVertex2i(bird.cx+birdm+move-w, y+birdy+ht);
        glTexCoord2f(tx, ty+1); glVertex2i(bird.cx+birdm+move-w, y+birdy-ht); 
        if (birdm < -949) {
            birdm = -1099;
        }
    }

    if ( birdm < -950) {
        birdm++;
        birdy--;
        glBegin(GL_QUADS);
        glTexCoord2f(tx-.25,      ty+1); 
        glVertex2i(bird.cx+birdm+move+w, y+birdy-ht);
        glTexCoord2f(tx-.25,      ty);    
        glVertex2i(bird.cx+birdm+move+w, y+birdy+ht);
        glTexCoord2f(tx, ty);    glVertex2i(bird.cx+birdm+move-w, y+birdy+ht);
        glTexCoord2f(tx, ty+1); glVertex2i(bird.cx+birdm+move-w, y+birdy-ht);

        if (birdm > -951) {
            birdm = -800;
        }
    }
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

