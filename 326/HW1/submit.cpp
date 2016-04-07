/*****************************************************************************
        FILE : submit.cpp (csci3260 2015-2016 Assignment 1)
        NOTE : you have to implement functions in this file
*****************************************************************************/

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <gl\glut.h>
#include <math.h>
#include <time.h>
#define TREENO 15//number of trees
int count = 0;
int winWidth = 600;
int winHeight = 600;
int fishone=0,fishtwo=20;
float fishoy,fishtx,fishty;
int ranX[TREENO],ranY[TREENO],wind[TREENO];
int movex=0, movey=0;
float cam_X(0), cam_Y(0), cam_Z(-250);
float cam_ViewX(0), cam_ViewY(0), cam_ViewZ(-500);
int ballN = 0;
int ballx[10],bally[10];
bool fishon=1,ani=1,lightsp=1;
int drawT=TREENO;
#define fno 500//number of water drops
typedef struct {
int fwx[fno];
int fwy[fno];
int fwz[fno];
int vx[fno];
int vy[fno];
int vz[fno];
int acc;
int life[fno];
} fountain;
fountain founta;
GLUquadricObj *quadratic;
void init(void) // All Setup For OpenGL Goes Here
{
	// Light 0 Settings
	static GLfloat light0pos[] = {200.f, 100.f, 400.f, 0.f};
	static GLfloat light0_mat1[] = {0.8, 0.8, 0.8, 1.f};
	static GLfloat light0_diff1[] = {0.9, 0.9, 0.9, 1.f};
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_mat1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff1);


	glEnable(GL_LIGHT0);
	// Light 1 Settings
	static GLfloat light1_mat1[] = {0.5, 0.5, 0.5, 1.0};
	static GLfloat light1_diff1[] = {1.0, 1.0, 1.0, 1.0};
	static GLfloat light1_specular1[] = {1.0, 1.0, 1.0, 1.0};
	static GLfloat spot_direction1[] = {-1.0, -1.0, 0.0};
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_mat1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diff1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular1);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION,0.5);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION,1.5);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION,0.2);
	glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,45.0);
	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,2.0);
	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,spot_direction1);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);		// Cull all the back face
	glFrontFace(GL_CCW);		// Define Counter-Clockwise as front face
	glEnable(GL_COLOR_MATERIAL); 
	glShadeModel(GL_SMOOTH);
	int i;
	srand(time(NULL));
	for(i=0;i<TREENO;i++){//random positions and movement for trees
		ranX[i]=((rand() % 20) -15)*10;
		ranY[i]=((rand() % 20) -15)*10;
		wind[i]=((rand() % 5) +6);
	}
	for(i=0; i<fno;i++){
			founta.life[i]=((rand() % 20));
			founta.fwx[i]=((rand() % 10)-5 );
			founta.fwy[i]=((rand() % 10)-5 );
			founta.fwz[i]=((rand() % 10) );
			founta.vx[i]=((rand() %5)-2);
			founta.vy[i]=((rand() %5)-2);
			founta.vz[i]=((rand() % 10));
	}
	founta.acc=-1;
}
static int spin;
void display(void) // Here's Where We Do All The Drawing
{
	if(lightsp) spin = (spin + 1) % 360;
	// Light 1 positon changing
	GLfloat light1pos[] = {300.f, 300.f, 200.f, 0.f};
	glClearColor(0.0, 0.0, 0.0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(cam_X+movex,cam_Y+movey,cam_Z, cam_ViewX+movex, cam_ViewY+movey, cam_ViewZ, 0, 1, 0);
	glPushMatrix();
		glRotated((GLdouble)spin,1.0,1.0,0.0);
		glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
	glPopMatrix();
	glTranslatef(0, 0, -550);
		GLfloat no_mat[] = {0.0,0.0,0.0,1.0};
		GLfloat mat_ambient[] = {0.7,0.7,0.7,1.0};
		GLfloat mat_ambient_color[] = {0.8,0.8,0.2,1.0};
		GLfloat mat_diffuse[] = {0.1,0.2,0.9,1.0};
		GLfloat mat_specular[] = {0.8,0.8,0.8,1.0};
		GLfloat no_shininess[] = {0.0};
		GLfloat high_shininess[] = {20.0};
		glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
		glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
		glMaterialfv(GL_FRONT,GL_SHININESS,high_shininess);
		glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


	// TODO:
	// Draw grounds and objects here

	//Start of Ground
	glColor4f(0.0, 0.4, 0.0, 1.0);
	glPushMatrix(); 
	glRotatef(80.0,1.0,0,0);
	glPushMatrix();
	glTranslatef(0,-110,0);
	glScalef(100.0,0.4,100.0);
	glutSolidCube(5);
	glPopMatrix();
    //End of Ground
		int i;
	double offset =0.0;
	quadratic = gluNewQuadric();

	if(fishon){
	fishone = (fishone+1)%360;
	fishtwo = (fishtwo+2)%360;
	}
	if(fishon||ani) count += 1;
	if(count == 10){
		count = 0;	
	}
	//Start of trees
	for(i=0;i<drawT;i++){
		if (count ==9) {
			wind[i] = (wind[i]*2)%7;
			if (wind[i]==0) wind[i]=1;
		}
		glPushMatrix();
		glRotatef(90,1,0,0);
	    glTranslatef(ranX[i],ranY[i],80);
		glColor4f(0.3,0.15, 0.15,1.0);
		gluCylinder(quadratic,8.0,10.0,30.0,32,32);
		glColor4f(0.0,0.2, 0.0,1.0);
		//trees do not reflect light
			glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
			glMaterialfv(GL_FRONT,GL_SPECULAR,no_mat);
			glMaterialfv(GL_FRONT,GL_SHININESS,no_shininess);
			glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glutSolidSphere(13, 20, 20);
		glPushMatrix();
		if(ani) glTranslatef(0+wind[i],15,-5);
		   else glTranslatef(0,15,-5);
		glutSolidSphere(8, 20, 20);
		glPopMatrix();
		if(ani) glTranslatef(-6+wind[i]/2,7,8);
		 else glTranslatef(-6,7,8);
		glutSolidSphere(8, 20, 10);
		glTranslatef(-6,-17,8);
		glutSolidSphere(8, 20, 10);
		glPopMatrix();
	}
	//end of trees
			glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
			glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
			glMaterialfv(GL_FRONT,GL_SHININESS,high_shininess);
			glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//start of house
	    glPushMatrix();
		glColor4f(0.3,0.2, 0.0,1.0);
		glTranslatef(150,-80,-70);
		glScalef(1.5,1.0,1.0);
	    glutSolidCube(50);

		glTranslatef(0,25,0);

		glScalef(36,20,36);

		glRotatef(-45,0,90,0);

		glColor4f(0.3,0.2,0.2,1.0);
		glutSolidOctahedron();
		glRotatef(135,0,-90,0);
		glScalef(1/36.0,1/20.0,1/36.0);
		glBegin(GL_QUADS);//window 1
		glColor4f(0.5,0.0, 0.3,1.0);
		glVertex3f(0.0-20, -25.0,0.0-26);
		glColor4f(0.3,0.7, 0.7,1.0);
		glVertex3f(0.0-20, -5.0,0.0-26);
		glVertex3f(10.0-20, -5.0,0.0-26);
		glColor4f(0.5,0.0, 0.3,1.0);
		glVertex3f(10.0-20, -25.0,0.0-26);
	    glEnd();
		glBegin(GL_QUADS);//window 2
		glColor4f(0.5,0.0, 0.3,1.0);
		glVertex3f(0.0-20+30, -25.0,0.0-26);
		glColor4f(0.3,0.7, 0.7,1.0);
		glVertex3f(0.0-20+30, -5.0,0.0-26);
		glVertex3f(10.0-20+30, -5.0,0.0-26);
		glColor4f(0.5,0.0, 0.3,1.0);
		glVertex3f(10.0-20+30, -25.0,0.0-26);
	    glEnd();
		glBegin(GL_QUADS);//door
		glColor4f(0.0,0.0, 0.0,1.0);
		glVertex3f(0.0-20+15, -48.0,0.0-25.5);
		glColor4f(0.0,0.7, 0.5,1.0);
		glVertex3f(0.0-20+15, -15.0,0.0-25.5);
		glVertex3f(10.0-20+15, -15.0,0.0-25.5);
		glColor4f(0.0,0.0, 0.0,1.0);
		glVertex3f(10.0-20+15, -48.0,0.0-25.5);
	    glEnd();
		glPopMatrix();
	//end of house

	//start of pool
	    glPushMatrix();
		glColor4f(0.0,0.0, 0.3,1.0);
		glRotatef(-90,1,0,0);
		glTranslatef(130,-100,-100);
		
		glPushMatrix();
		glScalef(2,1.5,0.05);
		glutSolidSphere(50, 30, 20);
		glPopMatrix();


		//fishes
		if(fishon) {
			fishoy=-wind[6]*7;
			fishtx=60*cos(3.1415926536*fishone/45.0)+wind[0]*2;
			fishty=60*sin(3.1415926536*fishone/45.0)-wind[2]*2;
		}
		glPushMatrix();
		glRotatef(fishone,0,0,-1);
		glColor4f(0.3,0.5, 0.3,1.0);
		glScalef(1,0.5,0.5);
		glTranslatef(10,fishoy,5);
		glutSolidSphere(10, 20, 10);
		glBegin(GL_TRIANGLES);
		glColor4f(0.0,0.3, 0.5,1.0);
		glVertex3f(10.0, 0.0,0.0);
		glColor4f(0.3,0.7, 0.3,1.0);
		int tail1=0, tail2=0;
		if(fishon){//waving tails
			tail1=wind[count%10];
		    tail2=wind[count%10+5];
		}
		glVertex3f(15.0+tail1, -15,0.0);
		glColor4f(0.3,0.3, 0.7,1.0);
		glVertex3f(15.0+tail2, 15.0,0.0);
	    glEnd();
		glPopMatrix();
		glScalef(-1,-0.5,0.5);
		glTranslatef(fishtx,fishty,5);
		glRotatef(fishtwo,0,0,-1);
	    glutSolidSphere(10, 20, 10);
		glBegin(GL_TRIANGLES);
		glColor4f(0.5,0.0, 0.3,1.0);
		glVertex3f(10.0, 0.0,0.0);
     	glColor4f(0.3,0.7, 0.3,1.0);
		glVertex3f(20.0+tail1, -15,0.0);
		glColor4f(0.3,0.3, 0.7,1.0);
		glVertex3f(20.0+tail2, 15.0,0.0);
	    glEnd();
//end of fishes
		glPopMatrix();
	//end of pool
    glPushMatrix();	
	//start of fountain
	glRotatef(-90.0,1.0,0,0);
	glTranslatef(150,170,-100);
	glutSolidTorus(5, 50, 10, 50);
	glPushMatrix();	
	glColor4f(0.0,0.1, 0.5,1.0);
	glScalef(1,1,5);
	glTranslatef(0,0,5);
	glutSolidTorus(5, 5, 10, 50);
	glPopMatrix();	
	glScalef(1,1,0.1);
	glColor4f(0.0,0.4, 0.5,1.0);
	glutSolidSphere(50, 20, 10);
	glPopMatrix();
	glPushMatrix();
		glRotatef(-90.0,1.0,0,0);
		glColor4f(0.0,0.4, 0.5,1.0);
	    glTranslatef(150,170,-50);
		for(i=0; i<fno;i++){
			if(founta.life[i]){
			founta.fwx[i]-=(founta.vx[i]);
			founta.fwy[i]-=(founta.vy[i]);
			founta.vz[i]+=founta.acc;
			founta.fwz[i]+=(founta.vz[i]);
			founta.life[i]=(founta.life[i]-1)%20;
			glPushMatrix();
			glTranslatef(founta.fwx[i],founta.fwy[i],founta.fwz[i]);
			if(ani&&abs(founta.fwx[i])<50&&abs(founta.fwz[i])<50&&abs(founta.fwy[i]<50))glutSolidSphere(0.7, 8, 8);
			glPopMatrix();
			}else{
				founta.life[i]=((rand() % 20));
				founta.fwx[i]=((rand() % 10)-5 );
				founta.fwy[i]=((rand() % 10)-5 );
				founta.fwz[i]=((rand() % 10) );
				founta.vx[i]=((rand() %5)-2);
				founta.vy[i]=((rand() %5)-2);
				founta.vz[i]=((rand() % 10));
			}
	    }//end of fountain
    glPopMatrix();
	glPopMatrix();

   
	glPopMatrix();
	glRectf(-0.5,-0.5,0.5,0.5);
	glutSwapBuffers();
	glFlush();	


	// Refresh the frame
	glutPostRedisplay();
}


void reshape(int w, int h) // Resize the GL Window. w=width, h=height
{
	winWidth = w; winHeight = h;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45, (float)w/(float)h, 250, 900); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


void special(int key, int x, int y) // Handle special keys
{	//use keyboard to change view point

	switch (key) 
	{	
		case GLUT_KEY_LEFT:
			movex-=10;
			break;
		case GLUT_KEY_UP:
			movey+=10;
			break;
		case GLUT_KEY_RIGHT:
			movex+=10;
			break;
		case GLUT_KEY_DOWN:
			movey-=10;
			break;
	
	}
}

void keyboard(unsigned char key, int x, int y) // Handle the keyboard events here
{	
	switch (key) 
	{//on/off animation

		case '\033'://press 'esc' to quit
		   exit(0);
		   break;
		case '1':
			ani=1;
			lightsp=1;
			fishon=1;//enable all animation
			break;
		case '2':
			ani=0;
			lightsp=0;
			fishon=0;//stop all animation
			break;
		case '3':
			ani=1;//wind for trees
			break;
		case '4':
			ani=0;//no wind for trees
			break;
		case '5':
			fishon=1;//enable fish to move
			break;
		case '6':
			fishon=0;//stop fish to move
			break;
		case '7':
			lightsp=1;//enable light1 to move
			break;
		case '8':
			lightsp=0;//stop light1 to move
			break;
	}
}

void mouseclick(int button, int state, int x, int y) // Handle the mouse click events here
{
	printf("Button %d, State %d, Position %d, %d \n", button, state, x, y);
	if(button){
		if(state&&drawT<TREENO) drawT++;	
	}else{
		if(state&&drawT>1) drawT--;
	}
	//botton0=>left,1=middle,2=right
}

void mousemove(int x, int y) //Handle the mouse momve events here 
{
	printf("Mouse moved to position %d %d \n", x, y);	
	//mouse moves to edges change view point
	if (x<100){movex-=10;}
	if (x>500){movex+=10;}
	if (y<100){movey+=10;}
	if (y>500){movey-=10;}
}

void idle()
{
	Sleep(10);
}


int main(int argc, char** argv)
{

	/*Initialization of GLUT Library */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

	/*Create a window with title specified */
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment 1");

	init();	/*not GLUT call, initialize several parameters */

	/*Register different CALLBACK function for GLUT to response 
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutPassiveMotionFunc(mousemove);
	glutMouseFunc(mouseclick);
	glutIdleFunc(idle);
	
	
	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();
	return 0;
}