
//Author / Modified by : Cesar Loya
// Date June 24, 2017
//cmps3350 - Source code

//*******************************************************************
//Progress
//June 24 - Mariachi appears on screen
//June 24 - general Enemy class ready
//June 24 - Mari class inherited from Enemy
//June 24 - OOP foundation ready

//*******************************************************************
//Responsabilities
//code object oriented Enemies
//proper animation
//walk in inervals
//animation for death 
//Enemies may have unique traits

//Week 4 goal - OO foundation and display one enemy

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
//#include "log.h"
#include "ppm.h"
#include "fonts.h"
#include "game.h"
using namespace std;

//global variables
int taco_delay = .01;
int pika_delay = .01;
int p_position = -600; 
int m_position = -500;
int ss_position = -1500;
int taco_position = -300; // front of obama
int p_walkFrame = 0;


int light_delay = .01;
int light2_delay = .01;
int light3_delay = .01;
//int light_delay4 = .01;
//int light_delay5 = .01;
//int light_delay6 = .01;
int l_position = 200; //3100
int l2_position = 0; //3100
//int l3_position = 200; //3100
//int l_position4 = 200; //3100
//int l_position5 = 200; //3100
//int l_position6 = 200; //3100
int l_walkFrame = 0;
int l2_walkFrame = 0;
//int l3_walkFrame = 0;



// this array will store the positions of enemies
const int size = 5; // 0-9
int array[size]= {200,300,800,1180,1200};
int start = 0;


class my_time {
    public:
	double physicsRate;
	double oobillion;
	struct timespec timeStart, timeEnd, timeCurrent;
	struct timespec walkTime;
	my_time() {
	    physicsRate = 1.0 / 30.0;
	    oobillion = 1.0 / 1e9;
	}
	double timeDiff(struct timespec *start, struct timespec *end) {
	    return (double)(end->tv_sec - start->tv_sec ) +
		(double)(end->tv_nsec - start->tv_nsec) * oobillion;
	}
	void timeCopy(struct timespec *dest, struct timespec *source) {
	    memcpy(dest, source, sizeof(struct timespec));
	}
	void recordTime(struct timespec *t) {
	    clock_gettime(CLOCK_REALTIME, t);
	}

} tim,tacito, pika_time, light_time, light2_time;


void mari_physics(void)
{
    tim.recordTime(&tim.timeCurrent);
    double timeSpan = tim.timeDiff(&tim.walkTime, &tim.timeCurrent);
    if (timeSpan > gl.m_delay) {
	m_position++;
	gl.m_walkFrame++;
	if (gl.m_walkFrame >= 7){
	    gl.m_walkFrame -= 7;
	    m_position--;
	}
	tim.recordTime(&tim.walkTime);
    }
}									


void taco_physics(void)
{
    tacito.recordTime(&tacito.timeCurrent);
    double timeSpan = tacito.timeDiff(&tacito.walkTime, &tacito.timeCurrent);
    if (timeSpan > taco_delay) { // can aldo make "gl" vars in here
	(timeSpan > taco_delay?taco_position-=2:taco_position--);  // can aldo make "gl" vars in here
	//taco_position--;

	tacito.recordTime(&tacito.walkTime);
    }
    //cout << "TESTING !!!!!!!!!!! TACO ::::: " << taco_position << endl;
}                                                                       				

//fix lightning animation
void light_physics(void)
{
    light_time.recordTime(&light_time.timeCurrent);
    double timeSpan = light_time.timeDiff(&light_time.walkTime, &light_time.timeCurrent);
    if (timeSpan > light_delay) { // can aldo make "gl" vars in here
	(timeSpan > light_delay? l_position-=2:l_position--);  // can aldo make "gl" vars in here
	//l_position++;
	l_walkFrame++;
	if (l_walkFrame >= 3) {
	    l_walkFrame -= 4;
	    //l_position--;
	    //l_position++;
	}
	light_time.recordTime(&light_time.walkTime);
    }
}                                                                       				

void light2_physics(void)
{
    light2_time.recordTime(&light2_time.timeCurrent);
    double timeSpan = light2_time.timeDiff(&light2_time.walkTime, &light2_time.timeCurrent);
    if (timeSpan > light2_delay) { // can aldo make "gl" vars in here
	(timeSpan > light2_delay? l2_position-=2:l2_position--);  // can aldo make "gl" vars in here
	//l_position++;
	l2_walkFrame++;
	if (l_walkFrame >= 3) {
	    l2_walkFrame -= 4;
	    //l_position--;
	    //l_position++;
	}
	light2_time.recordTime(&light2_time.walkTime);
    }
}                                                                       				




void pika_physics(void)
{
    pika_time.recordTime(&pika_time.timeCurrent);
    double timeSpan = pika_time.timeDiff(&pika_time.walkTime, &pika_time.timeCurrent);
    if (timeSpan > pika_delay) {
	p_position++;
	p_walkFrame++;
	if (p_walkFrame >= 4){
	    p_walkFrame -= 4;
	    p_position--;
	    p_position++;
	}
	pika_time.recordTime(&pika_time.walkTime);
    }
}									

//figure out proper physics for the shooting star
void shooting_star_physics(void)
{
    tim.recordTime(&tim.timeCurrent);
    double timeSpan = tim.timeDiff(&tim.walkTime, &tim.timeCurrent);
    if (timeSpan > gl.ss_delay) {
	ss_position++;
	gl.ss_walkFrame++;
	if (gl.ss_walkFrame >= 64){
	    gl.ss_walkFrame -= 64;
	    ss_position=ss_position-2;
	}
	tim.recordTime(&tim.walkTime);
    }
}


// make tacos, shoot tacos
void make_tacos()
{
    //gl.taco[gl.taco_count].cx =  y ; // find bug
    gl.taco[gl.taco_count].cx =        obama.cx;
    gl.taco[gl.taco_count].cy =        obama.cy;
    gl.taco[gl.taco_count].velocity =  gl.ob_particle_velocity;
    gl.taco[gl.taco_count].direction = gl.facing;
    gl.taco_count++;
}


void shoot_tacos()
{
    if(gl.isPressed == true){
	printf("TACOS GOOOOOO");
	make_tacos();
	gl.isPressed = false;
    }
}


Ppmimage* mari_image() 
{
    system("convert ./images/Enemy_Mariachi_3.gif ./images/Enemy_Mariachi_3.ppm");
    return ppm6GetImage("./images/Enemy_Mariachi_3.ppm");
}		



Ppmimage* female_image() 
{
    system("convert ./images/female_fighter.png ./images/female_fighter.ppm");
    return ppm6GetImage("./images/female_fighter.ppm");
}


Ppmimage* obama_image()
{
    system("convert ./images/obama.gif ./images/obama.ppm");
    return ppm6GetImage("./images/obama.ppm");
}


Ppmimage* obama_image2()
{
    system("convert ./images/obama.gif ./images/obama.ppm");
    return ppm6GetImage("./images/obama.ppm");
}


Ppmimage* obama_image3()
{
    system("convert ./images/obama.gif ./images/obama.ppm");
    return ppm6GetImage("./images/obama.ppm");
}



Ppmimage* sun_image()
{
    system("convert ./images/sun.gif ./images/sun.ppm");
    return ppm6GetImage("./images/sun.ppm");
}

Ppmimage* shooting_star_image()
{
    system("convert ./images/shooting_star.jpg ./images/shooting_star.ppm");
    return ppm6GetImage("./images/shooting_star.ppm");
}                                                                               

Ppmimage* taco_image()
{
    system("convert ./images/taco.jpg ./images/taco.ppm");
    return ppm6GetImage("./images/taco.ppm");
}                                                                               


Ppmimage* pika_image()
{
    system("convert ./images/pikachu.png ./images/pikachu.ppm");
    return ppm6GetImage("./images/pikachu.ppm");
}								


Ppmimage* pika2_image()
{
    system("convert ./images/pikachu.png ./images/pikachu.ppm");
    return ppm6GetImage("./images/pikachu.ppm");
}								

Ppmimage* pika3_image()
{
    system("convert ./images/pikachu.png ./images/pikachu.ppm");
    return ppm6GetImage("./images/pikachu.ppm");
}												
Ppmimage* pika4_image()
{
    system("convert ./images/pikachu.png ./images/pikachu.ppm");
    return ppm6GetImage("./images/pikachu.ppm");
}								

/*
Ppmimage* pika5_image()
{
    system("convert ./images/pikachu.png ./images/pikachu.ppm");
    return ppm6GetImage("./images/pikachu.ppm");
}								

Ppmimage* pika6_image()
{
    system("convert ./images/pikachu.png ./images/pikachu.ppm");
    return ppm6GetImage("./images/pikachu.ppm");
}								
*/

Ppmimage* light_image()
{
    system("convert ./images/light.png ./images/light.ppm");
    return ppm6GetImage("./images/light.ppm");
}								

Ppmimage* light2_image()
{
    system("convert ./images/light.png ./images/light.ppm");
    return ppm6GetImage("./images/light.ppm");
}								

Ppmimage* light3_image()
{
    system("convert ./images/light.png ./images/light.ppm");
    return ppm6GetImage("./images/light.ppm");
}								

Ppmimage* light4_image()
{
    system("convert ./images/light.png ./images/light.ppm");
    return ppm6GetImage("./images/light.ppm");
}								

Ppmimage* light5_image()
{
    system("convert ./images/light.png ./images/light.ppm");
    return ppm6GetImage("./images/light.ppm");
}								

Ppmimage* light6_image()
{
    system("convert ./images/light.png ./images/light.ppm");
    return ppm6GetImage("./images/light.ppm");
}								






//first enemy
void show_mari() 
{
    //cout << "main Character PositionX ----> : " << mainChar.cx << endl;
    //cout << "main Character Position y ====>: " << mainChar.cy << endl;
    int y = 165;
    //mariEnemy.cy = 175; // y cord
    float ht = 100.0;//height of mari
    float w = ht*0.5; 

    int temp = 1000;

    //make variables for borders

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.mari_Texture); 
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = gl.m_walkFrame % 7;  //animation  
    int ay = 0;

    //takes care of animation
    if (gl.m_walkFrame >= 7){
	//if animation reaches LAST sprite
	//start from start again
	ay = 1;
    }
    float tx = (float)ax / 7.0;
    float ty = (float)ay / 1.0;


    //aqui
    //if (mainChar.cx < mariEnemy.cx) {



    if(m_position < -600){  //was 600
	//walks to the left 
	m_position++;
	glBegin(GL_QUADS);
	glTexCoord2f(tx,      ty+.6); glVertex2i(mariEnemy.cx+ m_position+ temp +w, y-ht);
	glTexCoord2f(tx,      ty+0);    glVertex2i(mariEnemy.cx+ m_position+ temp +w, y+ht);
	glTexCoord2f(tx+.14, ty+0);    glVertex2i(mariEnemy.cx +m_position+ temp -w, y+ht);
	glTexCoord2f(tx+.14, ty+.6); glVertex2i(mariEnemy.cx + m_position+ temp -w, y-ht);

	//hopefully
	if(mainChar.cx >= mariEnemy.cx+m_position+temp-w &&
		mainChar.cx <= mariEnemy.cx+m_position+temp+w &&
		mainChar.cy <= y-ht-67 && mainChar.cy >= y+ht-67)
	{
	    mainChar.health--;
	}
	//if (mainChar.cx < mariEnemy.cx) {
	(m_position > -602? m_position=-500: m_position++);
	// } else {
	//m_position = -999999;
	//mariEnemy.cx = 5000;
    }
    //}


    if (m_position > -600) { //was 600
	// walk right
	m_position--;
	glBegin(GL_QUADS);
	glTexCoord2f(tx,      ty+.6);  glVertex2i(mariEnemy.cx+ m_position+ temp +w, y-ht);
	glTexCoord2f(tx,      ty+0);   glVertex2i(mariEnemy.cx+m_position+temp+w, y+ht);
	glTexCoord2f(tx-.14, ty+0);    glVertex2i(mariEnemy.cx +m_position+temp -w, y+ht);
	glTexCoord2f(tx-.14, ty+.6);   glVertex2i(mariEnemy.cx + m_position+temp -w, y-ht);


	if (mainChar.cx >= mariEnemy.cx+m_position+temp-w &&
		mainChar.cx <= mariEnemy.cx+m_position+temp+w &&
		mainChar.cy >= y-ht-67 && mainChar.cy <= y+ht-67){
	    mainChar.health--;
	}
	(m_position < -599?m_position=-700:m_position--);
    }
    // aqui
    //m_position = -9999;


    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);


    //WORK ON TIME library to sort enemies every 3 seconds

}																                                                                               

void show_female() 
{
    if (gl.initDone == 0) {
	float x = gl.xres/1; 
	x = x-60; //x cord
    }
    female.cy = 110; // y cord
    float ht = 50.0;//estatura de la mujer
    float w = ht*0.5; 

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.female_Texture); 
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = 1;   
    int ay = 1;
    if (1 >= 1)
	ay = 0;
    float tx = (float)ax / 8.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);
    //image starts at 12.5 , since 8 x 8
    glTexCoord2f(tx,      ty+ .12); glVertex2i(female.cx +w, female.cy-ht);
    glTexCoord2f(tx,      ty);    glVertex2i(female.cx +w, female.cy+ht);
    glTexCoord2f(tx+.08, ty);    glVertex2i(female.cx-w, female.cy+ht);
    glTexCoord2f(tx+.08, ty+.12); glVertex2i(female.cx-w, female.cy-ht);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}                                                                               

void show_light(void);
void show_light2(void);
void show_light3(void);
void show_light4(void);
void show_light5(void);
void show_light6(void);

void show_pika() 
{
    //if (mainChar.cx !> pika.cx) (
    //show_light();
    //distance of lightning
    //(l_position <= -50? l_position= 200:l_position--);

    // }else{
    // l_position 
    // <=
/*  ----------------------- DEBUGGING PURPOSES
    if (l_position <= -300) {
    l_position = 200;  //pika.cx; //3200
//l_position = pika.cx; //3200
cout << "RREEEEEEEE ASSIGNED !!!! " << l_position << endl;
} else {
l_position--;
cout << " :( :( :( regular\t: " << l_position << endl;
cout << " :( :( :( ========= >.<\t: " << pika.cx << endl;
}
*/
if(mainChar.cx > pika.cx){
    pika.cx = -999999;
    light.cy = 5000;
} else {
    show_light();
    (l_position <= -50? l_position= 200:l_position--);
}

if (gl.initDone == 0) {
    float x = gl.xres/1; 
    x = x-60; //x cord
}
pika.cy = 100; // y cord
float ht = 100.0;//estatura de la mujer
float w = ht*0.5; 

glPushMatrix();
glColor3f(1.0, 1.0, 1.0);
glBindTexture(GL_TEXTURE_2D, gl.pika_Texture); 
glEnable(GL_ALPHA_TEST);
glAlphaFunc(GL_GREATER, 0.0f);
glColor4ub(255,255,255,255);
int ax = p_walkFrame % 4;
int ay = 0;
if (p_walkFrame >= 4){ // work on this later
    if(p_position % 10 == 0){
	ay = 1;
    }
}
float tx = (float)ax / 4.0;
float ty = (float)ay / 1.0;
glBegin(GL_QUADS);
//image starts at 12.5 , since 8 x 8
//first try all 1s
//first two have to be opposites tx only
//ty + - - +     x -- ++
// .3 1
glTexCoord2f(tx ,       ty + 1); glVertex2i(pika.cx +w, pika.cy-ht);
glTexCoord2f(tx ,       ty); glVertex2i(pika.cx +w, pika.cy+ht);
glTexCoord2f(tx + .25,       ty); glVertex2i(pika.cx-w, pika.cy+ht);
glTexCoord2f(tx + .25,       ty + 1); glVertex2i(pika.cx-w, pika.cy-ht);
glEnd(); // .3
glPopMatrix();
glBindTexture(GL_TEXTURE_2D, 0);
glDisable(GL_ALPHA_TEST);
}  										                                                                             

void show_pika2() 
{
   show_light2();
   cout << "light 2 : \t\t" << light2.cx <<  endl;
    //distance of lightning
    //(l2_position <= -200? l2_position= 0:l2_position--);




    if(mainChar.cx > pika2.cx){
	pika2.cx = -999999;
	light2.cy = 5000;
    } else {
	show_light2(); // was -200 .. why ? 
	(l2_position <= 6700? l2_position= 7000:l2_position--);
    }




    if (gl.initDone == 0) {
	float x = gl.xres/1; 
	x = x-60; 
    }
    pika2.cy = 100; 
    float ht = 75.0;
    float w = ht*0.3; 

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.pika2_Texture); 
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = p_walkFrame % 4;
    int ay = 0;
    if (p_walkFrame >= 4){ // 
	if(p_position % 10 == 0){
	    ay = 1;
	}
    }
    float tx = (float)ax / 4.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);
    glTexCoord2f(tx ,       ty + 1); glVertex2i(pika2.cx +w, pika2.cy-ht);
    glTexCoord2f(tx ,       ty); glVertex2i(pika2.cx +w, pika2.cy+ht);
    glTexCoord2f(tx + .25,       ty); glVertex2i(pika2.cx-w, pika2.cy+ht);
    glTexCoord2f(tx + .25,       ty + 1); glVertex2i(pika2.cx-w, pika2.cy-ht);
    glEnd(); // .3
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}  										                                      


void show_pika3()
{
    show_light3();
    //distance of lightning
    //(l_position <= -50? l_position= 200:l_position--);

    if (gl.initDone == 0) {
	float x = gl.xres/1;
	x = x-60;
    }
    pika3.cy = 100;
    float ht = 100.0;
    float w = ht*0.5;

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.pika3_Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = p_walkFrame % 4;
    int ay = 0;
    if (p_walkFrame >= 4){ // 
	if(p_position % 10 == 0){
	    ay = 1;
	}
    }
    float tx = (float)ax / 4.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);
    glTexCoord2f(tx ,       ty + 1); glVertex2i(pika3.cx +w, pika3.cy-ht);
    glTexCoord2f(tx ,       ty); glVertex2i(pika3.cx +w, pika3.cy+ht);
    glTexCoord2f(tx + .25,       ty); glVertex2i(pika3.cx-w, pika3.cy+ht);
    glTexCoord2f(tx + .25,       ty + 1); glVertex2i(pika3.cx-w, pika3.cy-ht);
    glEnd(); // .3
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

void show_pika4()
{
    show_light4();
    //distance of lightning
    //(l_position <= -50? l_position= 200:l_position--);

    if (gl.initDone == 0) {
	float x = gl.xres/1;
	x = x-60;
    }
    pika4.cy = 485;
    float ht = 100.0;
    float w = ht*0.5;

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.pika4_Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = p_walkFrame % 4;
    int ay = 0;
    if (p_walkFrame >= 4){ // 
	if(p_position % 10 == 0){
	    ay = 1;
	}
    }
    float tx = (float)ax / 4.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);
    glTexCoord2f(tx ,       ty + 1); glVertex2i(pika4.cx +w, pika4.cy-ht);
    glTexCoord2f(tx ,       ty); glVertex2i(pika4.cx +w, pika4.cy+ht);
    glTexCoord2f(tx + .25,       ty); glVertex2i(pika4.cx-w, pika4.cy+ht);
    glTexCoord2f(tx + .25,       ty + 1); glVertex2i(pika4.cx-w, pika4.cy-ht);
    glEnd(); // .3
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}										

/*
void show_pika5()
{
    show_light5();
    //distance of lightning
    //(l_position <= -50? l_position= 200:l_position--);

    if (gl.initDone == 0) {
	float x = gl.xres/1;
	x = x-60;
    }
    pika5.cy = 325;
    float ht = 100.0;
    float w = ht*0.5;

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.pika5_Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = p_walkFrame % 4;
    int ay = 0;
    if (p_walkFrame >= 4){ // 
	if(p_position % 10 == 0){
	    ay = 1;
	}
    }
    float tx = (float)ax / 4.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);
    glTexCoord2f(tx ,       ty + 1); glVertex2i(pika5.cx +w, pika5.cy-ht);
    glTexCoord2f(tx ,       ty); glVertex2i(pika5.cx +w, pika5.cy+ht);
    glTexCoord2f(tx + .25,       ty); glVertex2i(pika5.cx-w, pika5.cy+ht);
    glTexCoord2f(tx + .25,       ty + 1); glVertex2i(pika5.cx-w, pika5.cy-ht);
    glEnd(); // .3
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}										

void show_pika6()
{
    show_light6();
    //distance of lightning
    //(l_position <= -50? l_position= 200:l_position--);

    if (gl.initDone == 0) {
	float x = gl.xres/1;
	x = x-60;
    }
    pika6.cy = 100;
    float ht = 100.0;
    float w = ht*0.5;

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.pika6_Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = p_walkFrame % 4;
    int ay = 0;
    if (p_walkFrame >= 4){ // 
	if(p_position % 10 == 0){
	    ay = 1;
	}
    }
    float tx = (float)ax / 4.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);
    glTexCoord2f(tx ,       ty + 1); glVertex2i(pika6.cx +w, pika6.cy-ht);
    glTexCoord2f(tx ,       ty); glVertex2i(pika6.cx +w, pika6.cy+ht);
    glTexCoord2f(tx + .25,       ty); glVertex2i(pika6.cx-w, pika6.cy+ht);
    glTexCoord2f(tx + .25,       ty + 1); glVertex2i(pika6.cx-w, pika6.cy-ht);
    glEnd(); // .3
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}										
*/




void show_taco();

void show_obama()
{
    //position starts at -300

    if (mainChar.cx < obama.cx) {
	show_taco();
	(taco_position == -500? taco_position= -30:taco_position--);
    } else {
	obama.cx = 5000;
	taco_position = -999999;
    }

    //obama.cx = 200;   charceter.cx is to make him follow megaman
    obama.cy = 300; // y cord
    float ht = 75.0;//height of obama
    float w = ht*0.5;

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.obama_Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = 1;
    int ay = 1;
    if (1 >= 1)
	ay = 0;
    float tx = (float)ax / 7.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);
    glTexCoord2f(tx,      ty+1.0); glVertex2i(obama.cx+w, obama.cy-ht);
    glTexCoord2f(tx,      ty+0);    glVertex2i(obama.cx+w, obama.cy+ht);
    glTexCoord2f(tx-1.0, ty+0);    glVertex2i(obama.cx-w, obama.cy+ht);
    glTexCoord2f(tx-1.0, ty+1.0); glVertex2i(obama.cx-w, obama.cy-ht);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}                                                                               

void show_taco()
{
    taco.cy = 300; // y cord
    float ht = 25.0;//estatura del tacito bien delicioso 
    float w = ht*1.0; // was .5

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.taco_Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = 1;
    int ay = 1;
    if (1 >= 1)
	ay = 0;
    float tx = (float)ax / 7.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);
    glTexCoord2f(tx,      ty+1.0); glVertex2i(taco.cx+ taco_position  +w, taco.cy-ht);
    glTexCoord2f(tx,      ty+0);    glVertex2i(taco.cx+ taco_position  +w, taco.cy+ht);
    glTexCoord2f(tx+1.0, ty+0);    glVertex2i(taco.cx+ taco_position-w, taco.cy+ht);
    glTexCoord2f(tx+1.0, ty+1.0); glVertex2i(taco.cx+ taco_position-w, taco.cy-ht);

    //CONTAC1 WITH TACO WILL CAUSE DAMAGE ... REALLL
    if(mainChar.cx >= taco.cx+taco_position-w && mainChar.cx <= taco.cx+taco_position+w &&
	    mainChar.cy >= taco.cy-ht && mainChar.cy <= taco.cy+ht)
    {
	//if(mainChar.cy >= taco.cx + taco_position - w && mainChar.cy <= taco.cy + taco_position+w)
	mainChar.health--;
    }

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

//lightning
void show_light()
{
    light.cy = 100; // y cord
    float ht = 50.0;//estatura del tacito bien delicioso 
    float w = ht*.3; // was .5

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.light_Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = l_walkFrame  % 3;
    int ay = 1;
    if (l_walkFrame >= 3)
	ay = 1;
    float tx = (float)ax / 3.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);
    glTexCoord2f(tx,      ty+1.0); glVertex2i(light.cx+ l_position+  w, light.cy-ht);
    glTexCoord2f(tx,      ty+0);    glVertex2i(light.cx+ l_position+ w, light.cy+ht);
    glTexCoord2f(tx+.3, ty+0);    glVertex2i(light.cx  +l_position -w, light.cy+ht);
    glTexCoord2f(tx+.3, ty+1.0); glVertex2i(light.cx   +l_position-w, light.cy-ht);

    //CONTACT WITH TACO WILL CAUSE DAMAGE ... REALLL
    // ---------- 
    // luz  
    if(mainChar.cx >= light.cx+l_position-w && mainChar.cx <= light.cx+l_position+w &&
	    mainChar.cy >= light.cy-ht && mainChar.cy <= light.cy+ht)
    {
	//if(mainChar.cy >= light.cx + light_position - w && mainChar.cy <= light.cy + light_position+w)
	mainChar.health-=.2;
    }

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
} 																								

void show_light2()
{
    light2.cy = 150; // y cord
    float ht = 75.0;//estatura del tacito bien delicioso 
    float w = ht*.3; // was .5


    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.light2_Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = l_walkFrame  % 3;
    int ay = 1;
    if (l_walkFrame >= 3)
	ay = 1;
    float tx = (float)ax / 3.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);
    glTexCoord2f(tx,      ty+1.0); glVertex2i(light2.cx /*+ l_position2*/+  w, light2.cy-ht);
    glTexCoord2f(tx,      ty+0);    glVertex2i(light2.cx/*+ l_position2*/+ w, light2.cy+ht);
    glTexCoord2f(tx+.3, ty+0);    glVertex2i(light2.cx  /*+l_position2*/ -w, light2.cy+ht);
    glTexCoord2f(tx+.3, ty+1.0); glVertex2i(light2.cx   /*+l_position2*/-w, light2.cy-ht);

    //CONTACT WITH TACO WILL CAUSE DAMAGE ... REALLL
    // ---------- 
    // MAKE MODIFICATION for LIGHT
    /*if(mainChar.cx >= taco.cx+taco_position-w && mainChar.cx <= taco.cx+taco_position+w &&
      mainChar.cy >= taco.cy-ht && mainChar.cy <= taco.cy+ht)
      {
    //if(mainChar.cy >= taco.cx + taco_position - w && mainChar.cy <= taco.cy + taco_position+w)
    mainChar.health--;
    }*/

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
} 													

void show_light3()
{
    light3.cy = 200; // y cord
    float ht = 150.0;//estatura del tacito bien delicioso 
    float w = ht*1.0; // was .5

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.light3_Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = l_walkFrame  % 3;
    int ay = 1;
    if (l_walkFrame >= 3)
	ay = 1;
    float tx = (float)ax / 3.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);
    glTexCoord2f(tx,      ty+1.0); glVertex2i(light3.cx /*+ l_position2*/+  w, light3.cy-ht);
    glTexCoord2f(tx,      ty+0);    glVertex2i(light3.cx/*+ l_position2*/+ w, light3.cy+ht);
    glTexCoord2f(tx+.3, ty+0);    glVertex2i(light3.cx  /*+l_position2*/ -w, light3.cy+ht);
    glTexCoord2f(tx+.3, ty+1.0); glVertex2i(light3.cx   /*+l_position2*/-w, light3.cy-ht);

    //CONTACT WITH TACO WILL CAUSE DAMAGE ... REALLL
    // ---------- 
    // MAKE MODIFICATION for LIGHT
    /*if(mainChar.cx >= taco.cx+taco_position-w && mainChar.cx <= taco.cx+taco_position+w &&
      mainChar.cy >= taco.cy-ht && mainChar.cy <= taco.cy+ht)
      {
    //if(mainChar.cy >= taco.cx + taco_position - w && mainChar.cy <= taco.cy + taco_position+w)
    mainChar.health--;
    }*/

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
} 													

void show_light4()
{
    light4.cy = 485; // y cord
    float ht = 150.0;//estatura del tacito bien delicioso 
    float w = ht*1.0; // was .5

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.light4_Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = l_walkFrame  % 3;
    int ay = 1;
    if (l_walkFrame >= 3)
	ay = 1;
    float tx = (float)ax / 3.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);
    glTexCoord2f(tx,      ty+1.0); glVertex2i(light4.cx /*+ l_position4*/+  w, light4.cy-ht);
    glTexCoord2f(tx,      ty+0);    glVertex2i(light4.cx/*+ l_position4*/+ w, light4.cy+ht);
    glTexCoord2f(tx+.3, ty+0);    glVertex2i(light4.cx  /*+l_position4*/ -w, light4.cy+ht);
    glTexCoord2f(tx+.3, ty+1.0); glVertex2i(light4.cx   /*+l_position4*/-w, light4.cy-ht);

    //CONTACT WITH TACO WILL CAUSE DAMAGE ... REALLL
    // ---------- 
    // MAKE MODIFICATION for LIGHT
    /*if(mainChar.cx >= taco.cx+taco_position-w && mainChar.cx <= taco.cx+taco_position+w &&
      mainChar.cy >= taco.cy-ht && mainChar.cy <= taco.cy+ht)
      {
    //if(mainChar.cy >= taco.cx + taco_position - w && mainChar.cy <= taco.cy + taco_position+w)
    mainChar.health--;
    }*/

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
} 													
/*
void show_light5()
{
    light5.cy = 325; // y cord
    float ht = 150.0;//estatura del tacito bien delicioso 
    float w = ht*1.0; // was .5

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.light5_Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = l_walkFrame  % 3;
    int ay = 1;
    if (l_walkFrame >= 3)
	ay = 1;
    float tx = (float)ax / 3.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);

    */
   // glTexCoord2f(tx,      ty+1.0); glVertex2i(light5.cx /*+ l_position5*/+  w, light5.cy-ht);
   // glTexCoord2f(tx,      ty+0);    glVertex2i(light5.cx/*+ l_position5*/+ w, light5.cy+ht);
   // glTexCoord2f(tx+.3, ty+0);    glVertex2i(light5.cx  /*+l_position5*/ -w, light5.cy+ht);
   // glTexCoord2f(tx+.3, ty+1.0); glVertex2i(light5.cx   /*+l_position5*/-w, light5.cy-ht);

    //CONTACT WITH TACO WILL CAUSE DAMAGE ... REALLL
    // ---------- 
    // MAKE MODIFICATION for LIGHT
    /*if(mainChar.cx >= taco.cx+taco_position-w && mainChar.cx <= taco.cx+taco_position+w &&
      mainChar.cy >= taco.cy-ht && mainChar.cy <= taco.cy+ht)
      {
    //if(mainChar.cy >= taco.cx + taco_position - w && mainChar.cy <= taco.cy + taco_position+w)
    mainChar.health--;
    }*/
/*
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
} 													
*/

/*
void show_light6()
{
    light6.cy = 100; // y cord
    float ht = 150.0;//estatura del tacito bien delicioso 
    float w = ht*1.0; // was .5

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.light6_Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = l_walkFrame  % 3;
    int ay = 1;
    if (l_walkFrame >= 3)
	ay = 1;
    float tx = (float)ax / 3.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);
  */  
    //glTexCoord2f(tx,      ty+1.0); glVertex2i(light6.cx /*+ l_position6*/+  w, light6.cy-ht);
    //glTexCoord2f(tx,      ty+0);    glVertex2i(light6.cx/*+ l_position6*/+ w, light6.cy+ht);
    //glTexCoord2f(tx+.3, ty+0);    glVertex2i(light6.cx  /*+l_position6*/ -w, light6.cy+ht);
    //glTexCoord2f(tx+.3, ty+1.0); glVertex2i(light6.cx   /*+l_position6*/-w, light6.cy-ht);
/*
    //CONTACT WITH TACO WILL CAUSE DAMAGE ... REALLL
    // ---------- 
    // MAKE MODIFICATION for LIGHT
    if(mainChar.cx >= taco.cx+taco_position-w && mainChar.cx <= taco.cx+taco_position+w &&
      mainChar.cy >= taco.cy-ht && mainChar.cy <= taco.cy+ht)
      {
    //if(mainChar.cy >= taco.cx + taco_position - w && mainChar.cy <= taco.cy + taco_position+w)
    mainChar.health--;
    }

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
} 
*/													

void show_sun()
{
    if (gl.initDone == 0) {
	float x = gl.xres/1;
	x = x-60; //x cord
    }
    sun.cx = 200;   //charceter.cx is to make him follow megaman
    sun.cy = 475; // y cord
    float ht = 75.0;//height of mari
    float w = ht*0.5;

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.sun_Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ax = 1;
    int ay = 1;
    if (1 >= 1)
	ay = 0;
    float tx = (float)ax / 7.0;
    float ty = (float)ay / 1.0;
    glBegin(GL_QUADS);
    glTexCoord2f(tx,      ty+1.0); glVertex2i(sun.cx+w, sun.cy-ht);
    glTexCoord2f(tx,      ty+0);    glVertex2i(sun.cx+w, sun.cy+ht);
    glTexCoord2f(tx+1.0, ty+0);    glVertex2i(sun.cx-w, sun.cy+ht);
    glTexCoord2f(tx+1.0, ty+1.0); glVertex2i(sun.cx-w, sun.cy-ht);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}


void show_shooting_star()
{
    int y = 450; //this might be it
    float ht = 50.0;//height of mari
    float w = ht*0.9;  //was .5

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.shooting_star_Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);  
    int ax = gl.ss_walkFrame % 64;  //animation  
    int ay = 0;


    if (gl.m_walkFrame >= 64){
	//if animation reaches LAST sprite
	//start from start again
	ay = 1;
    }
    float tx = (float)ax / 64.0;
    float ty = (float)ay / 8.0;

    glBegin(GL_QUADS);    
    //12.5
    glTexCoord2f(tx,      ty+.09); glVertex2i(shooting_star.cx- ss_position +w, y-ht);
    glTexCoord2f(tx,      ty);    glVertex2i(shooting_star.cx- ss_position+w, y+ht);
    glTexCoord2f(tx+.14, ty);    glVertex2i(shooting_star.cx -ss_position -w, y+ht);
    glTexCoord2f(tx+.14, ty+.09); glVertex2i(shooting_star.cx - ss_position -w, y-ht);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

void quickSort(int*, int, int);
void bubbleSort(int*, int);
void selectionSort(int*, int);
void Merge(int*,int,int,int);
void MergeSort(int*,int,int);
void heapSort(int arr[],int n);
void heapify(int arr[], int n, int i);

void bubbleSort(int* arr, int size){
    int temp;
    for(int i = 0; i <size; i++){
	for(int j = 0; j < size - i - 1; j++){
	    if(arr[j] > arr[j+1]){
		temp = arr[j];
		arr[j] = arr[j+1];
		arr[j+1] = temp;
	    }
	}
    }
}

void selectionSort(int* arr, int n)
{
    //pos_min is short for position of min
    int pos_min,temp;

    for (int i=0; i < n-1; i++)
    {
	pos_min = i;//set pos_min to the current index of array

	for (int j=i+1; j < n; j++)
	{

	    if (arr[j] < arr[pos_min])
		pos_min=j;
	    //pos_min will keep track of the index that min is in, 
	    //this is needed when a swap happens
	}

	//if pos_min no longer equals i than a smaller value must have been found, 
	//so a swap must occur
	if (pos_min != i)
	{
	    temp = arr[i];
	    arr[i] = arr[pos_min];
	    arr[pos_min] = temp;
	}
    }
}



void Merge(int *a, int low, int high, int mid)
{
    // We have low to mid and mid+1 to high already sorted.
    int i, j, k, temp[high-low+1];
    i = low;
    k = 0;
    j = mid + 1;

    // Merge the two parts into temp[].
    while (i <= mid && j <= high)
    {
	if (a[i] < a[j])
	{
	    temp[k] = a[i];
	    k++;
	    i++;
	}
	else
	{
	    temp[k] = a[j];
	    k++;
	    j++;
	}
    }

    // Insert all the remaining values from i to mid into temp[].
    while (i <= mid) {
	temp[k] = a[i];
	k++;
	i++;
    }

    // Insert all the remaining values from j to high into temp[].
    while (j <= high)
    {
	temp[k] = a[j];
	k++;
	j++;
    }

    // Assign sorted data stored in temp[] to a[].
    for (i = low; i <= high; i++)
    {
	a[i] = temp[i-low];
    }
}           

void MergeSort(int *a, int low, int high)
{
    int mid;
    if (low < high)
    {
	mid=(low+high)/2;
	// Split the data into two half.
	MergeSort(a, low, mid);
	MergeSort(a, mid+1, high);
	//
	//                         // Merge them to get sorted output.
	Merge(a, low, high, mid);
    }
}

void heapify(int arr[], int n, int i)
{
    int largest = i;  // Initialize largest as root
    int l = 2*i + 1;  // left = 2*i + 1
    int r = 2*i + 2;  // right = 2*i + 2

    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
	largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
	largest = r;

    // If largest is not root
    if (largest != i)
    {
	swap(arr[i], arr[largest]);

	// Recursively heapify the affected sub-tree
	heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
	heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i=n-1; i>=0; i--)
    {
	// Move current root to end
	swap(arr[0], arr[i]);

	// call max heapify on the reduced heap
	heapify(arr, i, 0);
    }
}

void quickSort(int* arr, int start, int end){
    int temp;
    int i = start;
    int j = end;
    int pivot = arr[(start + end) / 2];

    while(i <= j){
	while(arr[i] < pivot){
	    i++;
	}
	while(arr[j] > pivot){
	    j--;
	}
	if(i <= j){
	    temp = arr[i];
	    arr[i] = arr[j];
	    arr[j] = temp;
	    i++;
	    j--;
	}
    }

    if(start < j){
	quickSort(arr, start, j);
    }
    if(i < end){
	quickSort(arr, i, end);
    }
}

// random algorithm every time
// FIX the IF() spaces !!!!!


class IntQueue{
    private:
	int *array;
	int size;
	int front;
	int back;
	int items;
    public:
	IntQueue(int);
	IntQueue(const IntQueue &);
	~IntQueue();

	void enqueue(int);
	void dequeue(int &);
	bool isEmpty();
	bool isFull();
	void clear();
	void print();
};



IntQueue::IntQueue(int temp){
    array= new int[temp];
    size=temp;
    front= -1;
    back= -1;
    items=0;
}

IntQueue::IntQueue(const IntQueue &obj){
    front = obj.front;
    back = obj.back;
    items = obj.items;
    size = obj.size;

    array= new int[obj.size];
    for(int count=0; count < obj.size; count++){
	//array[count] = *(array + count);
	array[count] = obj.array[count];
	cout << "in copy constructor: " << array[count] << endl;
    }
}

IntQueue::~IntQueue(){
    delete [] array;
}

void IntQueue::enqueue(int elem){
    if(isFull()){
	//cout << "queue is full\n";
	return;
    }else{
	back++;
	//rear = (rear+1) % size;
	array[back]= elem;
	items++;
    }
}

void IntQueue::dequeue(int &recover){
    if(isEmpty()){
	//cout << "is empty\n";
    }else{
	front++;
	recover= array[front];
	items--;

    }
}

bool IntQueue::isEmpty(){
    return(items >= 0?true:false);
}

bool IntQueue::isFull(){
    return(items < size?true:false);
}

void IntQueue::clear(){
    front = size-1;
    back = size-1;
    items = 0;
}

void IntQueue::print(){
    for (int i =0; i < size; i++)
	cout << "in the queue: " << array[i] << endl;

}

int prime(int starting, int ending){
    int res = rand() % ending + starting;
    int a = 0;
    while ((res%2==0) ||(res%3==0) || (res%4==0)||(res%5==0)||(res%6==0)|| (res%7==0)||(res%8==0)||(res%9==0)) {
	a++;
	res = rand() % ending + starting;
    }
    cout << "total attempts: " << a << endl;
    cout << "result is: " << res << endl;
    return res;
}

//use random prime number generator to fill an array,
//with each subscript having a distance of 50 NOT 150
//then sort the array,
//and throw into Queue
//have queue asisgn positions
void init_round2(){
    int i =0; //cherka cherka
    int catcher;

    //3100
    int s1 = 3089, e1 = 3109, p1=0; //3100
    int s2 = 3190, e2 = 3210, p2=0; //3200
    int s3 = 7128, e3 = 7460, p3=0; //7150

    //store rands in here
    int r1,r2,r3;

    cout << "first round ****\n";
    r1 = prime(s1,e1);

    cout << "second round ****\n";
    r2 = prime(s2,e2);

    cout << "third round *****\n";
    r3 = prime(s3,e3);

    // r 1 2 3 are positions of enemies
    const int SIZE = 3;
    int tmp[SIZE] = {3200,3100,7100};
    bubbleSort(tmp,SIZE);

    for(i=0; i< SIZE; i++)
	cout << "pos : " << tmp[i] << endl;
    //now that it is sorted, throw into a queue
    IntQueue Q(SIZE);

    for(i=0; i<SIZE; i++){
	Q.enqueue(tmp[i]);//gets the random values into queue
    }


    Q.print();

    //get three enemies to use this on .. female is one of em
    for (i=0; i < SIZE; i++){
	Q.dequeue(catcher);
	if (i == 0)
	    p1 = catcher;
	if (i==1)
	    p2 = catcher;
	if (i==2)
	    p3 = catcher; // were p3 etc
    }

    //p1 .. p2 .. p3  will be the position of 3 enemies
    //female.cx  =

    //cout << "light position is : " << p1 << endl;
    //light.cx = r1;
    // cout << "pika  position is : " << p2 << endl;
    // pika.cx = r2;
    //cout << "pika 2 position is : " << p3 << endl;
    // pika2.cx = r3;
    //enemy 3.cx = 
    // */

    pika.cx  = 3200;
    cout << "pika1.cx  \t: " << pika.cx << endl;
    pika2.cx = 7100; // second
    cout << "pika2.cx  \t: " << pika2.cx << endl;


    light.cx =  3100;
    light2.cx = 7000;
    light3.cx = 7900;
    light4.cx = 8700;
    //light5.cx = 10400;
    //light6.cx = 12000;

    //pika4.cx = 6300;
    //pika5.cx = 7000;
    // pika6.cx = 8000;

    //7300 is too close   .. 7500
    pika3.cx = 8000; //  
    cout << "pika3.cx  \t: " << pika3.cx << endl;
    pika4.cx = 8800; //8k
    cout << "pika4.cx  \t: " << pika4.cx << endl;

   // pika5.cx = 10500; 
    //pika6.cx = 12100; 

}



void CesarInit(){

    //find POSITIONS of where you want them to be

    //dummy numbers right now 
    srand(time(NULL));
    int random_number = rand() % 5 + 1;


    switch (random_number) {
	case 1:
	    //bubble
	    bubbleSort(array,size);
	    break;
	case 2:
	    //selection
	    selectionSort(array,size);
	    break;
	case 3:
	    //Insertion
	    MergeSort(array, 0, size-1);
	    break;
	case 4:
	    //quick
	    quickSort(array, start, size-1);
	case 5:
	    //heap
	    heapSort(array, size);
	    break;
    }

    // once it is fully ready


    shooting_star.cx = *(array + 0);
    shooting_star.cy = *(array + 1);

    mariEnemy.cx = *(array + 2);
    mariEnemy.cy = 0;

    taco.cx = array[3];
    obama.cx = array[4];  //array[3]     // check out array
    //    taco.cx =  1180;  //array[4]

    init_round2();
}

