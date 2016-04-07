// Ass2.cpp : Defines the entry point for the console application.
//
/*****************************************************************************
        FILE : submit.cpp (csci3260 2015-2016 Assignment 2)
 *****************************************************************************/

//#include "stdafx.h"
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "bitmap.h"
/* Add code here to define variable   */
float cam_X(0), cam_Y(0), cam_Z(-250);
float cam_ViewX(0), cam_ViewY(0), cam_ViewZ(-500);
int movex = 0, movey = 0;
float winwidth, winheight;
GLubyte* sphereImage, *texpanel, *wheelimage, *w2image,*carimage,*sand;
GLuint sphereTex, wheel, w2,car;
int texpanel_w, texpanel_h, wheel_w, wheel_h, w2_w, w2_h,car_w, car_h,sand_w, sand_h;
GLubyte* fireImage[8];
GLuint fireTex[8];
int fireTex_w[8];
int fireTex_h[8];
int sphereTex_w;
int sphereTex_h;
int angle_x = 0;
int angle_y = 0;
int p = 0;
GLUquadricObj *disk;
int y = 0;
int i, j;
GLfloat ctrlpoints[4][4][3] = {
    {   {-0.5, 0.5, 3.0},
        {0.5, 0.5, 3.0},
        {1.5, 0.5, 3.0},
        {2.5, 0.5, 3.0}},
    {   {-0.5, 0.5, 2.0},
        {0.5, 1.5, 2.0},
        {1.5, 1.5, 2.0},
        {2.5, 0.5, 2.0}},
    {   {-0.5, 0.5, 1.0},
        {0.5, 1.5, 1.0},
        {1.5, 1.5, 1.0},
        {2.5, 0.5, 1.0}},
    {   {-0.5, 0.5, 0.0},
        {0.5, 0.5, 0.0},
        {1.5, 0.5, 0.0},
        {2.5, 0.5, 0.0}}
};
GLfloat texpts[2][2][2] = {
    {   {0.0, 0.0},
        {0.0, 1.0}},
    {   {1.0, 0.0},
        {1.0, 1.0}}};
GLfloat no_shininess[] = {0.0};
GLfloat high_shininess[] = {20.0};
GLfloat mat_shininess[] = {50.0};
GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat position[] = {0.0, 0.0, 2.0, 1.0};
GLfloat no_mat[] = {0.0, 0.0, 0.0, 1.0};
GLfloat mat_ambient[] = {0.7, 0.7, 0.7, 1.0};
GLfloat mat_ambient_color[] = {0.8, 0.8, 0.2, 1.0};
GLfloat mat_diffuse[] = {0.1, 0.2, 0.9, 1.0};
GLfloat mat_specular[] = {0.8, 0.8, 0.8, 1.0};
GLUquadricObj *sphere, *carpart;

#define sno 50//number of water drops
typedef struct {
float swx[sno];
float swy[sno];
float swz[sno];
float vx[sno];
float vy[sno];
float vz[sno];
float acc;
int life[sno];
} smoke;
smoke smo;
int x = 0;
float carx=0,cary=0,carz=0;
float ghostx=0,ghostz=0;
float carvelo=0,caracc=0,cargv=0;
float carvectorx=0,carvectory=0,carvectorz=0,carangley=0;
float displacement = 0;
float fground=0.0005,fsand=0.00415,ftion=fground;//firctions 
//Load the BMP file
GLubyte* TextureLoadBitmap(char *filename, int *w, int *h) /* I - Bitmap file to load */ {
    BITMAPINFO *info; /* Bitmap information */
    void *bits; /* Bitmap pixel bits */
    GLubyte *rgb; /* Bitmap RGB pixels */
    GLubyte err = '0';
    /*
     * Try loading the bitmap and converting it to RGB...
     */
    bits = LoadDIBitmap(filename, &info);
    if (bits == NULL)
        return (NULL);
    rgb = ConvertRGB(info, bits);
    if (rgb == NULL) {
        free(info);
        free(bits);
    };
    printf("%s: %d %d\n", filename, info->bmiHeader.biWidth, info->bmiHeader.biHeight);
    printf("read %s successfully\n", filename);
    *w = info->bmiHeader.biWidth;
    *h = info->bmiHeader.biHeight;
    /* Texture mapping parameters for filter and repeatance */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    /*
     * Free the bitmap and RGB images, then return 0 (no errors).
     */
    free(info);
    free(bits);
    return (rgb);
}

void initlights(void) {
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
	GLfloat light_amb[] =  {0.0, 0.0, 0.0, 1.0};  
	GLfloat light_dif[] = {1.0, 1.0, 1.0, 1.0};  
	GLfloat light_spc[] = {1.0, 1.0, 1.0, 1.0};  
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_spc);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void init() {
    // Clear Screen color
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0, 0, 0, 0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    // Texture mapping parameters for filter and repeatance
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // Texture mapping setting for Microsoft's OpenGL implementation
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	/* Add code here to initialize lighting, read bitmap images, initialize different GLU geometry
     * use glLight, TextureLoadBitmap, gluNewQuadric...etc
     */
    // init texture
    sphereImage = TextureLoadBitmap("ground.bmp", &sphereTex_w, &sphereTex_h);
    texpanel = TextureLoadBitmap("gr.bmp", &texpanel_w, &texpanel_h);
    wheelimage = TextureLoadBitmap("wheel.bmp", &wheel_w, &wheel_h);
    w2image = TextureLoadBitmap("w2.bmp", &w2_w, &w2_h);
	carimage = TextureLoadBitmap("car.bmp", &car_w, &car_h);
	sand = TextureLoadBitmap("sand.bmp",&sand_w,&sand_h);
	fireImage[0] = TextureLoadBitmap("p0.bmp",&fireTex_w[0],&fireTex_h[0]);
	glGenTextures(1, &fireTex[0]);
	glBindTexture(GL_TEXTURE_2D, fireTex[1]);
	fireImage[1] = TextureLoadBitmap("p1.bmp",&fireTex_w[1],&fireTex_h[1]);
	glGenTextures(1, &fireTex[1]);
	glBindTexture(GL_TEXTURE_2D, fireTex[1]);
	fireImage[2] = TextureLoadBitmap("p2.bmp",&fireTex_w[2],&fireTex_h[2]);
	glGenTextures(1, &fireTex[2]);
	glBindTexture(GL_TEXTURE_2D, fireTex[2]);
	fireImage[3] = TextureLoadBitmap("p3.bmp",&fireTex_w[3],&fireTex_h[3]);
	glGenTextures(1, &fireTex[3]);
	glBindTexture(GL_TEXTURE_2D, fireTex[3]);
	fireImage[4] = TextureLoadBitmap("p4.bmp",&fireTex_w[4],&fireTex_h[4]);
	glGenTextures(1, &fireTex[4]);
	glBindTexture(GL_TEXTURE_2D, fireTex[4]);
	fireImage[5] = TextureLoadBitmap("p5.bmp",&fireTex_w[5],&fireTex_h[5]);
	glGenTextures(1, &fireTex[5]);
	glBindTexture(GL_TEXTURE_2D, fireTex[5]);
	fireImage[6] = TextureLoadBitmap("p6.bmp",&fireTex_w[6],&fireTex_h[6]);
	glGenTextures(1, &fireTex[6]);
	glBindTexture(GL_TEXTURE_2D, fireTex[6]);
	fireImage[7] = TextureLoadBitmap("p7.bmp",&fireTex_w[7],&fireTex_h[7]);
	glGenTextures(1, &fireTex[7]);
	glBindTexture(GL_TEXTURE_2D, fireTex[7]);
    // init quadric object
    sphere = gluNewQuadric();
    gluQuadricTexture(sphere, GL_TRUE);
    carpart = gluNewQuadric();
    gluQuadricTexture(carpart, GL_TRUE);
	disk = gluNewQuadric();
	gluQuadricTexture(disk, GL_TRUE);
    // Texture mapping parameters for filter and repeatance
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_NORMALIZE);
    glFrontFace(GL_CCW); // Define Counter-Clockwise as front face
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_AUTO_NORMAL);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
    glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texpts[0][0][0]);
    glEnable(GL_MAP2_TEXTURE_COORD_2);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    initlights();
	srand(time(NULL));
	for(i=0; i<sno;i++){
			smo.life[i]=((rand() % 20));
			smo.swx[i]=((rand() % 10)-5 )/20.0;
			smo.swy[i]=((rand() % 10)-5 )/20.0;
			smo.swz[i]=((rand() % 10) )/20.0;
			smo.vx[i]=((rand() %5)-2)/20.0;
			smo.vy[i]=((rand() %5)-2)/20.0;
			smo.vz[i]=((rand() % 10))/20.0;
	}
	smo.acc=-0.1;
	ghostx=((rand() % 120)-60)/10.0;
	ghostz=((rand() % 120)-60)/10.0;
}

void drawwheel(float wheelx, float wheely, float wheelz, float wheelry, float wheelr) {//position xyz,rotationxy,radius
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);

    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w2_w, w2_h, 0, GL_RGB, GL_UNSIGNED_BYTE, w2image);
    glBindTexture(GL_TEXTURE_2D, w2);
    glTranslatef(wheelx, wheely, wheelz);  
    glTranslatef(0, wheelr, 0);	
	if(wheelry>0){
	    glTranslatef(0, 0, 0.01);	
	}
	glRotatef(wheelry*15, 0, 1, 0);
    glRotatef((displacement*10/wheelr), 0, 0, 1.0);
    gluCylinder(carpart, wheelr, wheelr, wheelr, 20, 8);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wheel_w, wheel_h, 0, GL_RGB, GL_UNSIGNED_BYTE, wheelimage);
    glBindTexture(GL_TEXTURE_2D, wheel);
    gluDisk(carpart, 0, wheelr, 20, 20);
    glTranslatef(0, 0, wheelr);
    gluDisk(carpart, 0, wheelr, 20, 20);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void display(void) {

    /* clear the window color before drawing is performed */
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
	/* Add code here to transform the car and the ground & draw them
     * use glTranslate, glRotate, glLoadIdentity, glPushMatrix, glPopMatrix, glMaterial, 
     * glBegin, gluSphere...etc
     *
     * Add code for Texture Mapping for the car and the ground
     * use glTexImage2D..
     */
	if(carz>0) {
		if(ftion==fsand)
			carvelo=carvelo*3;
		angle_x=0;
		ftion=fground;
	}else {
		if(ftion==fground)			
			carvelo=carvelo/8;
		ftion=fsand; 
	}
	
	if(cary>0&&cary-cargv>0){
		cargv-=0.001;
		cary+=cargv;
	}else {
		cary = 0;
		cargv = 0;
	}

	x=(x+1)%10;
	if(carvelo!=0&&cary==0&&carz<-0.5&&x==0){
		angle_x=((rand() % 3)-1 )*5;
	}
	if(carvelo<=(0.05-ftion)&&carvelo>=-(0.05-ftion))
		carvelo+=caracc;//speed up with limit
	else if(caracc>0){
		if(carvelo>0) 
			carvelo = (0.05-ftion);
		else
			carvelo = -(0.05-ftion);
	}   
	if(carvelo>0){ 
		if((carvelo-ftion)>=0)
			carvelo-=ftion;
		else carvelo=0;
	}	else  {
		if((carvelo+ftion)<=0)
			carvelo+=ftion;
		else carvelo=0;
	}	
	if(carx-carvelo*cos(carangley/180.0*3.1415926536)>=-5.25&&carx-carvelo*cos(carangley/180.0*3.1415926536)<=5.25) 	
			carx-=carvelo*cos(carangley/180.0*3.1415926536);
		else {
			if(carx>0) carx=5.245;
			else carx=-5.245;
		}

		if(carz+carvelo*sin(carangley/180.0*3.1415926536)>=-5.25&& carz+carvelo*sin(carangley/180.0*3.1415926536)<=5.25){	
			carz+=carvelo*sin(carangley/180.0*3.1415926536);
		}else {
			if(carz>0) carz=5.245;
			else carz=-5.245;
		}
	displacement += carvelo;
	gluLookAt(0.0+carx, 3.0, 3.0+carz, 0+carx, 0.0, 0+carz, 0.0, 1.0, 0.0);
	GLfloat spot_position[]  ={carx+0.15*sin(carangley/180.0*3.1415926536),cary, carz+0.15*cos(carangley/180.0*3.1415926536), 1};
	GLfloat spot_direction[] = {-cos(carangley/180.0*3.1415926536), 0, sin(carangley/180.0*3.1415926536)}; 
    glLightfv(GL_LIGHT1, GL_POSITION, spot_position);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	if(carvelo!=0)  
		glEnable(GL_LIGHT1);
	else 
		glDisable(GL_LIGHT1);
	float d =100;
	int a, b;
	float dw = 12.0 / d;
	float dh = 6.0 / d;
  //sand floor
	glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, sand_w, sand_h, 0, GL_RGB, GL_UNSIGNED_BYTE, sand);
  glNormal3f(cos(carangley/180.0*3.1415926536), -1, -sin(carangley/180.0*3.1415926536));
  for (b = 0; b < d; ++b) {
    glBegin(GL_QUADS);
    for (a = 0; a < d; ++a) {
	  glTexCoord2f(a/d, b/d);
    glVertex3f(dw * a-6.0,0,dh * b-6.0 );
    glTexCoord2f(a/d, (b+1)/d);
    glVertex3f(dw * a-6.0,0, dh * (b + 1)-6.0);
    glTexCoord2f((a+1)/d, (b+1)/d);
    glVertex3f(dw * (a+1)-6.0,0,dh * (b + 1)-6.0);
    glTexCoord2f((a+1)/d,b/d);
    glVertex3f(dw * (a+1)-6.0,0, dh * b -6.0);
    }
    glEnd();
  }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
	//flat floor
    glEnable(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texpanel_w, texpanel_h, 0, GL_RGB, GL_UNSIGNED_BYTE, texpanel);	
  glNormal3f(cos(carangley/180.0*3.1415926536), -1, -sin(carangley/180.0*3.1415926536));
  for (b = 0; b < d; ++b) {
    glBegin(GL_QUADS);
    for (a = 0; a < d; ++a) {
	glTexCoord2f(a/d, b/d);
    glVertex3f(dw * a-6.0,0,dh * b );
    glTexCoord2f(a/d, (b+1)/d);
    glVertex3f(dw * a-6.0,0, dh * (b + 1));
    glTexCoord2f((a+1)/d, (b+1)/d);
    glVertex3f(dw * (a+1)-6.0,0,dh * (b + 1));
    glTexCoord2f((a+1)/d,b/d);
    glVertex3f(dw * (a+1)-6.0,0, dh * b );
    }
    glEnd();
  }

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

	p=(p+1)%8;
	//fire
	for(i=-6;i<7;i++){
	glPushMatrix();	
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fireTex_w[p], fireTex_h[p], 0, GL_RGB, GL_UNSIGNED_BYTE, fireImage[p]);	
	glPushMatrix();	
		glTranslatef(i,0.5,-6.0);
		gluDisk(disk, 0.0, 0.5, 8,8);
	glPopMatrix();
		glPushMatrix();	
		glTranslatef(6.0,0.5,i/12.0*10.5+0.5);
		glRotatef(270, 0, 1, 0.0);
		gluDisk(disk, 0.0, 0.5, 8,8);
	glPopMatrix();
		glPushMatrix();	
		glTranslatef(-6.0,0.5,i/12.0*10.5+0.5);
		glRotatef(90, 0, 1, 0.0);
		gluDisk(disk, 0.0, 0.5, 8,8);
	glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	}	

	if(carvelo>0){
		if(carangley+angle_y>360)
			carangley=0;
		else if(carangley+angle_y<0)
			carangley=360;
		else carangley+=angle_y;
	} else
	if(carvelo<0){
		if(carangley-angle_y>360)
			carangley=0;
		else if(carangley-angle_y<0)
			carangley=360;
		else carangley-=angle_y;
	}  
	glPushMatrix();	
	//start whole car transfromation
	if(carvelo!=0&&angle_x!=0)
		glTranslatef(0,0.05,0);
	glTranslatef(carx,cary,carz);
	glRotatef(carangley, 0, 1, 0);
	glRotatef(angle_x, 0, 0, 1);
	//end of whole car transfromation
	//wheels
	glPushMatrix();
		drawwheel(0, 0 , -0.1,  angle_y, 0.1);
		drawwheel(0, 0 , 0.3,  angle_y, 0.1);
		drawwheel(0.5, 0 , -0.15,  0, 0.15);
		drawwheel(0.5, 0 , 0.3,  0, 0.15);
	glPopMatrix();
    glPushMatrix();
    glColor3f(0.0, 0.3, 0.0);
    glScalef(0.25, 0.4, 0.1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, car_w, car_h, 0, GL_RGB, GL_UNSIGNED_BYTE, carimage);
	glBindTexture(GL_TEXTURE_2D, car);
    glEnable(GL_TEXTURE_2D);
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);	 
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.25, 0.4, 0.1);
    glColor3f(0.0, 0.2, 0.0);
    for (j = 0; j <= 8; j++) {
        glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 30; i++) glEvalCoord2f((GLfloat) i / 30.0, (GLfloat) j / 8.0);
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 30; i++) glEvalCoord2f((GLfloat) j / 8.0, (GLfloat) i / 30.0);
        glEnd();
    }

	glPopMatrix();
	//car base
	glPushMatrix();
	glColor3f(0.0, 0.3, 0.0);
	glScalef(2.5, 0.3, 1.0);
	glTranslatef(0.1,0.5, 0.15);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, car_w, car_h, 0, GL_RGB, GL_UNSIGNED_BYTE, carimage);
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, car);
    glutSolidCube(0.3);
    glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_S); //disable texture coordinate generation	
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

	if(carvelo>0)
	for(i=0; i<sno;i++){
			if(smo.life[i]&&smo.swx[i]<0.75){
			smo.vx[i]+=smo.acc;
			smo.swx[i]-=(smo.vx[i]);
			smo.swy[i]-=(smo.vy[i]);
			smo.swz[i]+=(smo.vz[i]);
			smo.life[i]=(smo.life[i]-1)%20;	
			glPushMatrix();
				glColor3f(0.1+smo.swx[i]*0.6, 0.1+0.4*smo.swx[i], 0.4*smo.swx[i]);
				glTranslatef(smo.swx[i]+0.75,smo.swy[i],smo.swz[i]-0.05);
				glutSolidSphere(0.02, 6, 6);
			glPopMatrix();
			}else{
				smo.life[i]=((rand() % 20));
				smo.swx[i]=((rand() % 6)-3 )/20.0;
				smo.swy[i]=((rand() % 6)-3 )/20.0;
				smo.swz[i]=((rand() % 5) )/20.0;
				smo.vx[i]=((rand() %5)-2)/20.0;
				smo.vy[i]=((rand() %5)-2)/20.0;
				smo.vz[i]=((rand() % 5))/20.0;
			}

	    }

    glPopMatrix();//happy helloween
	if(ghostx*ghostx>32||ghostz*ghostz>32){
			ghostx=((rand() % 120)-60)/10.0;
			ghostz=((rand() % 120)-60)/10.0;
	}else{
			ghostx+=(0.1+carvelo)*sin(rand() % 90+0.0);
			ghostz+=(0.1+carvelo)*cos(rand() % 90+0.0);
	}
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0, 1.0,1.0,0.5);
	if(((carx-ghostx)*(carx-ghostx)+(carz-ghostz)*(carz-ghostz))>1)
		glTranslatef(ghostx,0.3, ghostz);
	else
		glTranslatef(ghostx,cary+0.7, ghostz);
	glRotatef(-90,1.0,0,0);
	glPushMatrix();
		glutSolidSphere(0.3,10,10);
	glPopMatrix();	
		glTranslatef(0,0, -0.3);
	glPushMatrix();
		glutSolidCone(0.3, 0.3, 10, 10);
	glPopMatrix();
	glDisable(GL_BLEND);	

    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h) {
    winwidth = w;
    winheight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (GLfloat) w / (GLfloat) h, 1.0, 40.0);
    glMatrixMode(GL_MODELVIEW);
}

/* keyboard up CALLBACK for handling special keyboard up event */
void keyspecialup(int key, int x, int y) {
    /* Add code here to control the animation interactively */
    switch (key) {
            // car control
        case GLUT_KEY_UP: // move front
			caracc=0;
            break;
        case GLUT_KEY_DOWN: // move back
			caracc=0;
            break;
        case GLUT_KEY_LEFT: // move left
            angle_y = 0; 
            break;
        case GLUT_KEY_RIGHT: // move right
            angle_y = 0; 
            break;
    }
}

/* keyboard CALLBACK for handling special keyboard event */
void keyspecial(int key, int x, int y) {
    /* Add code here to control the animation interactively */
    switch (key) {
        case GLUT_KEY_UP:
		if(carvelo<=0.4||carvelo>=-0.4)	caracc=0.00525; else {
			if(carvelo>0) carvelo=0.3;
			else carvelo=-0.3;
			caracc=0;
		}
		break;
        case GLUT_KEY_DOWN:
			if(carvelo<=0.4||carvelo>=-0.4) caracc=-0.00525; else
			{
			if(carvelo>0) carvelo=0.3;
			else carvelo=-0.3;
			caracc=0;
		}
		break;
        case GLUT_KEY_LEFT:
            angle_y = 1; 
            break;
        case GLUT_KEY_RIGHT:
            angle_y = -1;
            break;
    }
}

/* keyboard CALLBACK for handling keyboard event */
void keyboard(unsigned char key, int x, int y) {
    /* Add code here to control the animation interactively */
    switch (key) {
            // Reset all parameters
        case 'r': 
			angle_x = 0;
            angle_y = 0;
			carx=0;
			cary=0;
			carz=0;
			carvelo=0;
			caracc=0;
			displacement = 0;
			ftion=fground; 
			carangley=0;
			cargv=0;
			ghostx=((rand() % 120)-60)/10.0;
			ghostz=((rand() % 120)-60)/10.0;
            break;
		case 32: 
			if(cary<=0.1){
				cary = 0.8;
				cargv = 0;
			}
            break;
	}

}
void keyboardup(unsigned char key, int x, int y){

}
void mouse(int button, int state, int ix, int iy) {
}

void drag(int ix, int iy) {
}

void timer(int t) {
    /* Add code here to update the velocity, acceleration, position and rotation of car and wheels */
    // display after update and reset timer
    glutPostRedisplay();    // Refresh the frame
    glutTimerFunc(10, timer, 1);
}

void main(int argc, char** argv) {
    /*Initialization of GLUT Library */
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    /*Create a window with title specified */
    glutCreateWindow("Assignment 2");
    /*Register different CALLBACK function for GLUT to response 
    with different events, e.g. window sizing, mouse click or
    keyboard stroke */
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardup);
    glutSpecialFunc(keyspecial);
    glutSpecialUpFunc(keyspecialup);
    // call timer function every 10 ms
    glutTimerFunc(10, timer, 1);
    glutMouseFunc(mouse);
    glutMotionFunc(drag);
    init(); /*not GLUT call, initialize several parameters */
    /*Enter the GLUT event processing loop which never returns.
    it will call different registered CALLBACK according
    to different events. */
    glutMainLoop();
    gluDeleteQuadric(sphere);
    gluDeleteQuadric(carpart);	
	gluDeleteQuadric(disk);
}