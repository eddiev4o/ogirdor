//Modified by: Eddie Velasco
//Date: June 20, 2017
//Purpose: Group Project source code
//
// Source code for generating level from 
// background, tiles, and foreground. Also
// will include functionality for offscreen
// death falls, power-ups, health bar display
// and end of level completion
//
// ============================================
// Week 4 Progress
// --------------
// Added Background, Platform w/Textures
// ============================================
// Week 5 Progress
// --------------
// Added Tile System
// Modified to act as Base + Platforms
// ============================================
// Week 6 Progress
// --------------
// Added Time and Health Bar
// ============================================
// Week 7 Progress
// --------------
// Added new Textures
// & more Level Design with .txt
// ============================================
// Week 8 Progress
// Pause Menu, Exit to Menu, Reset Gameplay
// Score, GameOver Frameworks
// Level 2 Progress
//=============================================

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "ppm.h"
#include "fonts.h"
#include "game.h"
extern void tileCollision(Vec *);
extern void checkUnder(Vec *);
extern void spriteDisappear(Sprite*);
void renderBackground(int levelSelect)
{
    // Texture of main character depends on what they select
    if (levelSelect == 1)
        gl.tempBackgroundTexture = gl.backgroundTexture;
    if (levelSelect == 2)
        gl.tempBackgroundTexture = gl.desertTexture;


	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	glBindTexture(GL_TEXTURE_2D, gl.tempBackgroundTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(gl.xc[0], gl.yc[1]); glVertex2i(0,0);
		glTexCoord2f(gl.xc[0], gl.yc[0]); glVertex2i(0, gl.yres);
		glTexCoord2f(gl.xc[1], gl.yc[0]); glVertex2i(gl.xres, gl.yres);
		glTexCoord2f(gl.xc[1], gl.yc[1]); glVertex2i(gl.xres, 0);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void renderTiles()
{
	if (gl.levelSelect == 1) {
		FILE *fpi1 = fopen("level.txt","r");
		if (fpi1) {
			lev.nrows=0;
			char line[700];
			while (fgets(line, 700, fpi1) != NULL) {
				lev.removeCrLf(line);
				int slen = strlen(line);
				lev.ncols = slen;
				for (int j=0; j<slen; j++) {
					lev.arr[lev.nrows][j] = line[j];
				}
				++lev.nrows;
			}
			fclose(fpi1);
		}
	}

	if (gl.levelSelect == 2) {
		FILE *fpi2 = fopen("level2.txt","r");
		if (fpi2) {
			lev.nrows=0;
			char line[700];
			while (fgets(line, 700, fpi2) != NULL) {
				lev.removeCrLf(line);
				int slen = strlen(line);
				lev.ncols = slen;
				//Log("line: %s\n", line);
				for (int j=0; j<slen; j++) {
					lev.arr[lev.nrows][j] = line[j];
				}
				++lev.nrows;
			}
			fclose(fpi2);
		}
	}
	//
	//----------------------------
	//Render the level tile system
	//----------------------------
	Flt dd = lev.ftsz[0];
	Flt offy = lev.tile_base;
	int ncols_to_render = gl.xres / lev.tilesize[0] + 2;
	int col = (int)(gl.camera[0] / dd);
	col = col % lev.ncols;
	Flt cdd = gl.camera[0] / dd;
	Flt flo = floor(cdd);
	Flt dec = (cdd - flo);
	Flt offx = -dec * dd;
	for (int j=0; j<ncols_to_render; j++) {
		int row = lev.nrows-1;
		for (int i=0; i<lev.nrows; i++) {
			if (lev.arr[row][col] == 'w') {
				//render brick texture
				glColor3f(0.8, 0.8, 0.6);
				glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, gl.platformTexture);
				Vec tr = { (Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0 };
				glTranslated(tr.x,tr.y,tr.z);
				int tx = lev.tilesize[0];
				int ty = lev.tilesize[1];
				tileCollision(&tr);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 1.0); glVertex2i( 0,  0);
					glTexCoord2f(0.0, 0.0); glVertex2i( 0, ty);
					glTexCoord2f(1.0, 0.0); glVertex2i(tx, ty);
					glTexCoord2f(1.0, 1.0); glVertex2i(tx,  0);
				glEnd();
				glPopMatrix();
				glBindTexture(GL_TEXTURE_2D, 0);
			}	
			if (lev.arr[row][col] == 'l') {
				//render lava texture
				glColor3f(0.8, 0.8, 0.6);
				glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, gl.lavaTexture);
				Vec tr = { (Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0 };
				glTranslated(tr.x,tr.y,tr.z);
				int tx = lev.tilesize[0];
				int ty = lev.tilesize[1];
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 1.0); glVertex2i( 0,  0);
					glTexCoord2f(0.0, 0.0); glVertex2i( 0, ty);
					glTexCoord2f(1.0, 0.0); glVertex2i(tx, ty);
					glTexCoord2f(1.0, 1.0); glVertex2i(tx,  0);
				glEnd();
				glPopMatrix();
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			if (lev.arr[row][col] == 'b') {
				//render metal texture
				glColor3f(0.6, 0.6, 0.6);
				glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, gl.metalTexture);
				Vec tr = { (Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0 };
				glTranslated(tr.x,tr.y,tr.z);
				int tx = lev.tilesize[0];
				int ty = lev.tilesize[1];
				tileCollision(&tr);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 1.0); glVertex2i( 0,  0);
					glTexCoord2f(0.0, 0.0); glVertex2i( 0, ty);
					glTexCoord2f(1.0, 0.0); glVertex2i(tx, ty);
					glTexCoord2f(1.0, 1.0); glVertex2i(tx,  0);
				glEnd();
				glPopMatrix();
				glBindTexture(GL_TEXTURE_2D, 0);
			}	
			if (lev.arr[row][col] == 's') {
				//render sand texture
				glColor3f(0.8, 0.8, 0.6);
				glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, gl.sandTexture);
				Vec tr = { (Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0 };
				glTranslated(tr.x,tr.y,tr.z);
				tileCollision(&tr);
				int tx = lev.tilesize[0];
				int ty = lev.tilesize[1];
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 1.0); glVertex2i( 0,  0);
					glTexCoord2f(0.0, 0.0); glVertex2i( 0, ty);
					glTexCoord2f(1.0, 0.0); glVertex2i(tx, ty);
					glTexCoord2f(1.0, 1.0); glVertex2i(tx,  0);
				glEnd();
				glPopMatrix();
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			
			if (lev.arr[row][col] == 'a') {
				//render water texture
				glPushMatrix();
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glColor4f(0.0, 0.8, 1.0, .8);
				Vec tr = { (Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0 };
				glTranslated(tr.x,tr.y,tr.z);
				int tx = lev.tilesize[0];
				int ty = lev.tilesize[1];
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 1.0); glVertex2i( 0,  0);
					glTexCoord2f(0.0, 0.0); glVertex2i( 0, ty);
					glTexCoord2f(1.0, 0.0); glVertex2i(tx, ty);
					glTexCoord2f(1.0, 1.0); glVertex2i(tx,  0);
				glEnd();
				glDisable(GL_BLEND);
				glPopMatrix();
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			if (lev.arr[row][col] == ' ') {
				Vec tr = { (Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0 };
				checkUnder(&tr);
			}
			--row;
		}
		++col;
		col = col % lev.ncols;
	}
}

void pauseScreen()
{
	Rect r; 
	float h = 100.0;
	float w = 200.0;
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.58,0.58,0.72,0.8);
	glTranslated(gl.xres/2, gl.yres/2, 0);
	glBegin(GL_QUADS);
	        glVertex2i(-w, -h);
	        glVertex2i(-w,  h);
	        glVertex2i(w,   h);
	        glVertex2i(w,  -h);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();
	r.bot = gl.yres/2 + 80;
	r.left = gl.xres/2;
	r.center = 1; 
	ggprint8b(&r, 16, 0, "PAUSE SCREEN");
	r.center = 0;
	r.left = gl.xres/2 - 100;
	ggprint8b(&r, 16, 0, "Press P - Play");
	ggprint8b(&r, 16, 0, "Press E - Exit to Menu");
	ggprint8b(&r, 16, 0, "Press Esc - Exit Game");

}

void startTimer()
{
//Include a starting timer that begins the allowment of movement
//start the scoreTime after this function
//Use Enum State
}

void levelCompletion()
{
	float h = 300;
	float w = 300;
	if(STATE_COMPLETION) {
		int scoreM = gl.minutes * 60;
		gl.score = scoreM + gl.gameFrame; //add gl.score if kill points
		timers.recordTime(&timers.timeCurrent);
		double timeSpan = timers.timeDiff(&timers.timeOut, &timers.timeCurrent);
		if (timeSpan > gl.gameDelay) {
			++gl.timeOutFrame;
			timers.recordTime(&timers.timeOut);
		}	
		if(gl.timeOutFrame <=5 && gl.levelSelect == 2) {
			if (gl.timeOutFrame <= 2) {
				float h1 = 200;
				float w1 = 200;
				glPushMatrix();
				glColor3f(1.0,1.0,1.0);	
				glTranslatef(gl.xres/2,gl.yres/2, 0);
				glBindTexture(GL_TEXTURE_2D, gl.levelCompleteTexture);
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0.0f);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 1.0); glVertex2i(-w1, -h1);
					glTexCoord2f(0.0, 0.0); glVertex2i(-w1, h1);
					glTexCoord2f(1.0, 0.0); glVertex2i(w1, h1);
					glTexCoord2f(1.0, 1.0); glVertex2i(w1, -h1);
				glEnd();
				glPopMatrix();
				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_ALPHA_TEST);
			}
			if(gl.timeOutFrame > 2 && gl.timeOutFrame <= 5) {
			glColor3f(1.0,1.0,1.0);	
			glBindTexture(GL_TEXTURE_2D, gl.WelcomeTexture);
			glEnable(GL_ALPHA_TEST);
			glColor4ub(255,255,255,255);
			//glEnable(GL_TEXTURE_2D);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0); glVertex2i(0,0);
				glTexCoord2f(0.0, 0.0); glVertex2i(0,gl.yres);
				glTexCoord2f(1.0, 0.0); glVertex2i(gl.xres,gl.yres);
				glTexCoord2f(1.0, 1.0); glVertex2i(gl.xres,0);
			glEnd();
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_ALPHA_TEST);
			}
		}
		if (gl.timeOutFrame <= 5 && gl.levelSelect == 1) {
		float h1 = 200;
		float w1 = 200;
		glPushMatrix();
		glColor3f(1.0,1.0,1.0);	
		glTranslatef(gl.xres/2,gl.yres/2, 0);
		glBindTexture(GL_TEXTURE_2D, gl.levelCompleteTexture);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0); glVertex2i(-w1, -h1);
			glTexCoord2f(0.0, 0.0); glVertex2i(-w1, h1);
			glTexCoord2f(1.0, 0.0); glVertex2i(w1, h1);
			glTexCoord2f(1.0, 1.0); glVertex2i(w1, -h1);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
		}
		if (gl.timeOutFrame > 5 && gl.timeOutFrame <= 10) {
			if (gl.score <= 60) {
			//Platinum
			glPushMatrix();
			glColor3f(1.0,1.0,1.0);
			glTranslatef(gl.xres/2,gl.yres/2, 0);
			glBindTexture(GL_TEXTURE_2D, gl.PlatinumTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0); glVertex2i(-w, -h);
				glTexCoord2f(0.0, 0.0); glVertex2i(-w, h);
				glTexCoord2f(1.0, 0.0); glVertex2i(w, h);
				glTexCoord2f(1.0, 1.0); glVertex2i(w, -h);
			glEnd();
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_ALPHA_TEST);
			printf("Rank: Platinum\n Score is: %i\n", gl.score);
			}
			if (gl.score > 60 && gl.score <= 120) {
			//Gold
			glPushMatrix();
			glColor3f(1.0,1.0,1.0);
			glTranslatef(gl.xres/2,gl.yres/2, 0);
			glBindTexture(GL_TEXTURE_2D, gl.GoldTexture);	
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0); glVertex2i(-w, -h);
				glTexCoord2f(0.0, 0.0); glVertex2i(-w, h);
				glTexCoord2f(1.0, 0.0); glVertex2i(w, h);
				glTexCoord2f(1.0, 1.0); glVertex2i(w, -h);
			glEnd();
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, 0);	
			glDisable(GL_ALPHA_TEST);
			printf("Rank: Gold\n Score is: %i\n", gl.score);
			}
			if (gl.score > 120 && gl.score <= 180) {
			//Silver
			glPushMatrix();
			glColor3f(1.0,1.0,1.0);
			glTranslatef(gl.xres/2,gl.yres/2, 0);
			glBindTexture(GL_TEXTURE_2D, gl.SilverTexture);	
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0); glVertex2i(-w, -h);
				glTexCoord2f(0.0, 0.0); glVertex2i(-w, h);
				glTexCoord2f(1.0, 0.0); glVertex2i(w, h);
				glTexCoord2f(1.0, 1.0); glVertex2i(w, -h);
			glEnd();
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, 0);	
			glDisable(GL_ALPHA_TEST);
			printf("RanK: Silver\n Score is: %i\n", gl.score);
			}
			if (gl.score > 180 && gl.score <= 240) {
			//Bronze
			glPushMatrix();
			glColor3f(1.0,1.0,1.0);
			glTranslatef(gl.xres/2,gl.yres/2, 0);
			glBindTexture(GL_TEXTURE_2D, gl.BronzeTexture);	
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0); glVertex2i(-w, -h);
				glTexCoord2f(0.0, 0.0); glVertex2i(-w, h);
				glTexCoord2f(1.0, 0.0); glVertex2i(w, h);
				glTexCoord2f(1.0, 1.0); glVertex2i(w, -h);
			glEnd();
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_ALPHA_TEST);
			printf("Rank: Bronze\n Score is: %i\n", gl.score);
			}
			if (gl.score > 240 && gl.score < 600) {
			//Noob	
			glPushMatrix();
			glColor3f(1.0,1.0,1.0);
			glTranslatef(gl.xres/2,gl.yres/2, 0);
			glBindTexture(GL_TEXTURE_2D, gl.noobTexture);	
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0); glVertex2i(-w, -h);
				glTexCoord2f(0.0, 0.0); glVertex2i(-w, h);
				glTexCoord2f(1.0, 0.0); glVertex2i(w, h);
				glTexCoord2f(1.0, 1.0); glVertex2i(w, -h);
			glEnd();
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, 0);	
			glDisable(GL_ALPHA_TEST);
			printf("N00B\n Score is: %i\n", gl.score);
			}
		}
		if (gl.timeOutFrame >= 15) {
			gl.display_startmenu = true;
			gl.display_startinggame = false;
			gl.display_levelselectionmenu = false;
			gl.display_playernamemenu = false;
			gl.menu_position = 1;
			gl.timeOutFrame = 0;
			gl.state = STATE_GAMEPLAY;
		}
	}
}

void deathScreen()
{
	mainChar.health -= 1;	
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.timeOut, &timers.timeCurrent);
	if (timeSpan > gl.gameDelay) {
		++gl.timeOutFrame;
		timers.recordTime(&timers.timeOut);
	}
	if (gl.timeOutFrame <= 3) {
		printf("GAMEOVER\n");
		glColor3f(1.0,1.0,1.0);	
		glBindTexture(GL_TEXTURE_2D, gl.gameoverTexture);
		glEnable(GL_ALPHA_TEST);
		glColor4ub(255,255,255,255);
		//glEnable(GL_TEXTURE_2D);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0); glVertex2i(0,0);
			glTexCoord2f(0.0, 0.0); glVertex2i(0,gl.yres);
			glTexCoord2f(1.0, 0.0); glVertex2i(gl.xres,gl.yres);
			glTexCoord2f(1.0, 1.0); glVertex2i(gl.xres,0);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
	}
	if (gl.timeOutFrame > 3) {	
		gl.display_startmenu = true;
		gl.display_startinggame = false;
		gl.display_levelselectionmenu = false;
		gl.display_playernamemenu = false;
		gl.menu_position = 1;
		gl.timeOutFrame = 0;
		gl.state = STATE_GAMEPLAY;
	}
}

void healthBar(int xres, int yres)
{
//generate a healthbar on top left of the screen
	Rect r;
	unsigned int c = 0x002d88d8;
	r.bot = yres-30;
	r.left = (xres/xres) + 70;
	r.center = 0;
	ggprint8b(&r, 16, c, "HEALTH");
	Shape s;
	Shape box[200];
	for (int i = 0; i < mainChar.health; i++) {
		box[i].width = 3;
		box[i].height = 10;
		box[i].center.x = 72 + (i*6);
		box[i].center.y = 555;
		box[i].center.z = 0;
		s = box[i];
		glPushMatrix();
		glColor3ub(255, 0, 255);
		glTranslatef(s.center.x, s.center.y, s.center.z);
		float w = s.width;
		float h = s.height;
		glBegin(GL_QUADS);
		glVertex2i(-w, -h);
		glVertex2i(-w, h);
		glVertex2i(w, h);
		glVertex2i(w, -h);
		glEnd();
		glPopMatrix();
	}
}

void renderTimeDisplay()
{
	Rect r;
	unsigned int c = 0x002d88d8;
	r.bot = gl.yres - 30;
	r.left = (gl.xres/2);
	r.center = 0;
	if (gl.state == STATE_GAMEPLAY) {
		timers.recordTime(&timers.timeCurrent);
        	double timeSpan = timers.timeDiff(&timers.gameTime, &timers.timeCurrent);
        	if (timeSpan > gl.gameDelay) {
                	//advance
                	++gl.gameFrame;
                	timers.recordTime(&timers.gameTime);
        	}
        	if (gl.gameFrame >= 60)
        	{
                	++gl.minutes;
                	gl.gameFrame = 0;
        	}
	}
	ggprint16(&r, 20, c, "TIME");
	ggprint16(&r, 32, c, "%d:%02i", gl.minutes, gl.gameFrame);

}

void eddieInit()
{
	gl.coins[0].cx = 100;
	gl.coins[0].cy = 90;
	gl.coins[1].cx = 100;
	gl.coins[1].cy = 180;
	gl.coins[2].cx = 800;
	gl.coins[2].cy = 90;
}
void renderCoin(Sprite* coinSprite)
{
	if (coinSprite->collected != true) {
	float h = 15;
	float w = 15;
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, gl.coinTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	int ix = 1, iy = 1;
	float tx = (float)ix;
	float ty = (float)iy;
	glBegin(GL_QUADS);
	glTexCoord2f(tx + 1, ty + 1); glVertex2i(coinSprite->cx-w, coinSprite->cy-h);
	glTexCoord2f(tx + 1,     ty); glVertex2i(coinSprite->cx-w, coinSprite->cy+h);
	glTexCoord2f(tx,        ty ); glVertex2i(coinSprite->cx+w, coinSprite->cy+h);
	glTexCoord2f(tx,     ty + 1); glVertex2i(coinSprite->cx+w, coinSprite->cy-h);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	if ((mainChar.cx > (coinSprite->cx - 10) && mainChar.cx < (coinSprite->cx + 10))
	&& ((mainChar.cy > (coinSprite->cy - 10)) && (mainChar.cy < (coinSprite->cy + 10)))) {
		//if character picks up power up,
		//stop rendering and set x to -999999 to avoid
		//picking up invisible power ups
		printf("Picked up speed boost! Movement Speed: + 0.2!\n");
		coinSprite->collected = true;
		spriteDisappear(coinSprite);
		gl.score += 10;
	}
    }

}
void renderScore()
{
	Rect r;
	unsigned int c = 0x002d88d8;
	r.bot = gl.yres - 30;
	r.left = gl.xres - 100;
	r.center = 0;
	if (gl.state == STATE_GAMEPLAY) {
		timers.recordTime(&timers.timeCurrent);
        	double timeSpan = timers.timeDiff(&timers.gameTime, &timers.timeCurrent);
        	if (timeSpan > gl.gameDelay) {
                	//advance
                	++gl.gameFrame;
                	timers.recordTime(&timers.gameTime);
        	}
        	if (gl.gameFrame >= 60)
        	{
                	++gl.minutes;
                	gl.gameFrame = 0;
        	}	
	}
	ggprint16(&r, 20, c, "SCORE");	
	r.bot = gl.yres - 50;
	r.left = gl.xres - 72;
	r.center = 0;
	ggprint16(&r, 32, c, "%d", gl.score);
}
void hudHealth ()
{
	
	if (gl.characterSelect == 1)
		gl.tempHeadTexture = gl.mainchar1headTexture;
	if (gl.characterSelect == 2)
		gl.tempHeadTexture = gl.mainchar2headTexture;
	if (gl.characterSelect == 3)
		gl.tempHeadTexture = gl.mainchar3headTexture;
	if (gl.characterSelect == 4)
		gl.tempHeadTexture = gl.mainchar4headTexture;

	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.58,0.58,0.72,0.8);
	glBegin(GL_QUAD_STRIP);
		glVertex2i(10, gl.yres-30);
		glVertex2i(10, gl.yres-60);
		glVertex2i(25, gl.yres-10);
		glVertex2i(25, gl.yres-80);
		glVertex2i(55, gl.yres-10);
		glVertex2i(55, gl.yres-80);
		glVertex2i(70, gl.yres-30);
		glVertex2i(70, gl.yres-60);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();
		
	Shape s;
	Shape box[200];
		if (mainChar.health > 25 && mainChar.health <= 30) {
			
			for (int i = 0; i < 30 + 1; i++) {
			box[i].width = 3;
			box[i].height = 15;
			box[i].center.x = 72 + (i*6);
			box[i].center.y = 555;
			box[i].center.z = 0;
			s = box[i];
			glPushMatrix();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(0.58,0.58,0.72, 0.8);
			glTranslatef(s.center.x, s.center.y, s.center.z);
			float w = s.width;
			float h = s.height;
			glBegin(GL_QUADS);
			glVertex2i(-w, -h);
			glVertex2i(-w, h);
			glVertex2i(w, h);
			glVertex2i(w, -h);
			glEnd();
			glDisable(GL_BLEND);
			glPopMatrix();
			}
		}
		if (mainChar.health > 20 && mainChar.health <= 25) {		
			for (int i = 0; i < 25 + 1; i++) {
			box[i].width = 3;
			box[i].height = 15;
			box[i].center.x = 72 + (i*6);
			box[i].center.y = 555;
			box[i].center.z = 0;
			s = box[i];
			glPushMatrix();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(0.58,0.58,0.72, 0.8);
			glTranslatef(s.center.x, s.center.y, s.center.z);
			float w = s.width;
			float h = s.height;
			glBegin(GL_QUADS);
			glVertex2i(-w, -h);
			glVertex2i(-w, h);
			glVertex2i(w, h);
			glVertex2i(w, -h);
			glEnd();
			glDisable(GL_BLEND);
			glPopMatrix();
			}
		}	
		if (mainChar.health > 15 && mainChar.health <= 20) {
			
			for (int i = 0; i < 20 + 1; i++) {
			box[i].width = 3;
			box[i].height = 15;
			box[i].center.x = 72 + (i*6);
			box[i].center.y = 555;
			box[i].center.z = 0;
			s = box[i];
			glPushMatrix();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(0.58,0.58,0.72, 0.8);
			glTranslatef(s.center.x, s.center.y, s.center.z);
			float w = s.width;
			float h = s.height;
			glBegin(GL_QUADS);
			glVertex2i(-w, -h);
			glVertex2i(-w, h);
			glVertex2i(w, h);
			glVertex2i(w, -h);
			glEnd();
			glDisable(GL_BLEND);
			glPopMatrix();
			}
		}
		if (mainChar.health > 10 && mainChar.health <= 15) {
			
			for (int i = 0; i < 15 + 1; i++) {
			box[i].width = 3;
			box[i].height = 15;
			box[i].center.x = 72 + (i*6);
			box[i].center.y = 555;
			box[i].center.z = 0;
			s = box[i];
			glPushMatrix();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(0.58,0.58,0.72, 0.8);
			glTranslatef(s.center.x, s.center.y, s.center.z);
			float w = s.width;
			float h = s.height;
			glBegin(GL_QUADS);
			glVertex2i(-w, -h);
			glVertex2i(-w, h);
			glVertex2i(w, h);
			glVertex2i(w, -h);
			glEnd();
			glDisable(GL_BLEND);
			glPopMatrix();
			}
		}
		if (mainChar.health <= 10) {		
			for (int i = 0; i < 10 + 1; i++) {
			box[i].width = 3;
			box[i].height = 15;
			box[i].center.x = 72 + (i*6);
			box[i].center.y = 555;
			box[i].center.z = 0;
			s = box[i];
			glPushMatrix();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(0.58,0.58,0.72, 0.8);
			glTranslatef(s.center.x, s.center.y, s.center.z);
			float w = s.width;
			float h = s.height;
			glBegin(GL_QUADS);
			glVertex2i(-w, -h);
			glVertex2i(-w, h);
			glVertex2i(w, h);
			glVertex2i(w, -h);
			glEnd();
			glDisable(GL_BLEND);
			glPopMatrix();
			}
		}

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, gl.tempHeadTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2i(-10, gl.yres-90);
		glTexCoord2f(0.0, 0.0); glVertex2i(-10, gl.yres);
		glTexCoord2f(1.0, 0.0); glVertex2i(95,  gl.yres);
		glTexCoord2f(1.0, 1.0); glVertex2i(95,  gl.yres-90);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
}
