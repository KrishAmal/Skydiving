
/*
---------MINIPROJECT ON-------------------------
---------GRAPHICAL VISUALIZATION OF SKYDIVING---

---------CREATED BY-----------------------------
---------AMAL KRISHNAN KP & UNAIS IB------------
*/



#include <GL\glew.h>
#include <GL\freeglut.h>

#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<random>
#include<string>
#include<iostream>


int flag = 0;
int ok=1;
float aero = 400.0;
int aero2 = -4500;
int high_score = 0;

int hit,miss = 3;
float repeatZ=0;
static int p[100] = {0};

#define PI 3.14159265
int rot = 0;
int limit;
int no_of_tor = 100;
int no_of_obj = 150;
int extra = 0;
int aero_flag1 = 0;
GLfloat random_z[150][3];
GLfloat random_m[60][3];

GLfloat random_tor[100][3];

//Var for bounding box of Skydiver
float topleft[3], topright[3], bottomleft[3], bottomright[3];

float wing[8][3] = { (5.0, 0.0, 0.0), (5.0f, 30.0f, -40.0f), (5.0f, 20.0f, -65.0f), (5.0f, 23.0f, -65.0f), (5.0f, 23.0f, -40.0f), (80.0, 23.0, -65.0), (80.0, 20.0, -65.0),
(80.0, 23.0, -160.0), (80.0, 20.0, -160.0) };

//VARIABLES DECLARATION

// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f, ly = 1.0f;
// XZ position of the camera
//float x = 0.0f, z = 50.0f, y = 210.0f;
float x = 0.0f, z =1100.0f, y = 825.0f;

//float sky_x = x, sky_y = y - 65, sky_z = z - 5;
float sky_x = 0, sky_y = -65, sky_z = -5;

float px = 0.5, py = 0.5, box_x = 0, box_y = 0;

//CLASS FOR THE BOUNDING BOX
class Objectbox{
public:
	float TopLeft[3];
	float TopRight[3];
	float BottomLeft[3];
	float BottomRight[3];
	bool collide;
	bool nopass;
	int flag = 0;

	Objectbox(){
		collide = false;
		nopass = true;
	}

	void put(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4){
		TopRight[0] = x1;
		TopRight[1] = y1;
		TopRight[2] = z1;
		BottomRight[0] = x2;
		BottomRight[1] = y2;
		BottomRight[2] = z2;
		BottomLeft[0] = x3;
		BottomLeft[1] = y3;
		BottomLeft[2] = z3;
		TopLeft[0] = x4;
		TopLeft[1] = y4;
		TopLeft[2] = z4;
	}

	int checkpass(){
		if (nopass == false)
			return 2;
		else if (nopass == true)
			return 1;
	}

	bool getCollide(){ return (collide); }
	

}skyd,tor[100];
int collide[100];

//TIMER
static void Timer(int value){
	//z -= 0.5;
	if (rot < 360)
		rot += 1;
	else
		rot = 1;
	if (ok == false)
		aero2 += 2;
	if (ok==3)
		aero +=1;
	glutPostRedisplay();
	// 100 milliseconds
	glutTimerFunc(10, Timer, 0);
}
int changevalue = 30.0;
void camera()
{
	z -= changevalue;

	if (changevalue == 30.0)
		extra = 30;
	else
		extra = 50;

	//y -= 0.05;
	//if (z > 0){
		//y -= 0.2;
		//x += 0.40;
		//z -= 2.0;
	//}

}

//RANDOM FUNCTION FOR
void random2(){
	for (int i = 0; i < no_of_obj; i++)
	{
		if (i % 2 == 0)
			random_z[i][0] = (rand() % 2000);
		else
			random_z[i][0] = -(rand() % 2000);
		
		random_z[i][1] = 5.0;

		//
		//if (i % 5 == 0)
	//		random_z[i][2] = (rand() % 10000);
		//else
			random_z[i][2] = -((rand() % 10000 )+100);

			//printf("%f  %f\n",random_z[i][0],random_z[i][2]);
	}
}

//RANDOM FUNCTION FOR TORUS
void rand_tor(){
	for (int i = 0; i < no_of_tor; i++)
	{
		if (i % 2 == 0)
			random_tor[i][0] = (rand() % 20);
		else
			random_tor[i][0] = -(rand() % 20);

		random_tor[i][1] = 800;

		random_tor[i][2] = -(100 + (600 * i));

		//printf("%f %f %f\n", random_tor[i][0], random_tor[i][1], random_tor[i][2] );
	}
}

//Random mountain points
void random_mountain(int x){

	float z = -1000.0f;
	float y_s = 10.0f, y_m = 50.0f, y_l = 75.0f;
	float x_s = 20.0f, x_l = 40.0f;

	float y_scales[3] = { y_s, y_m, y_l };
	float x_scales[2] = { x_s, x_l };

	random_m[0][0] = 0.0f;
	random_m[0][1] = 0.0f;
	random_m[0][2] = z;

	random_m[2][0] = x_l;
	random_m[2][1] = 0.0f;
	random_m[2][2] = z;


	random_m[1][0] = (random_m[0][0] + random_m[2][0]) / 2;
	random_m[1][1] = (rand() % 100) + 30.0f;
	random_m[1][2] = z;


	for (int i = 3; i < x / 2; i = i + 3){

		random_m[i][0] = random_m[i - 1][0];
		random_m[i][1] = 0.0f;
		random_m[i][2] = z;

		random_m[i + 2][0] = random_m[i - 1][0] + x_scales[1];
		random_m[i + 2][1] = 0.0f;
		random_m[i + 2][2] = z;

		random_m[i + 1][0] = (random_m[i][0] + random_m[i + 2][0]) / 2;
		random_m[i + 1][1] = (rand() % 100) + 30.0f;
		random_m[i + 1][2] = z;

	}
}

//MOUNTAINS
void draw_mountains(int x){

	for (int i = 0; i <x / 2; i = i + 3){
		glColor3ub(128, 0, 0);
		glBegin(GL_POLYGON);
		glVertex3f(random_m[i][0], random_m[i][1], random_m[i][2]);
		glVertex3f(random_m[i + 1][0], random_m[i + 1][1], random_m[i + 1][2]);
		glVertex3f(random_m[i + 2][0], random_m[i + 2][1], random_m[i + 2][2]);
	}
	glEnd();
}

//GROUND
void draw_ground(){
	glDisable(GL_LIGHTING);
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-10000.0f, 0.0f, -10000.0f+z);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(10000.0f, 0.0f, -10000.0f+z);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(10000.0f, 0.0f, 100.0f+z);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-10000.0f, 0.0f, 100.0f+z);


	glEnd();
	glEnable(GL_LIGHTING);
}

//SKY
void draw_sky(){
	glDisable(GL_LIGHTING);
	//glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	
	glColor3f(0.0, 0.6, 0.8);
	glVertex3f(-5000.0f, -1000.0f, -5500.0f+z);
	glVertex3f(-5000.0f, 3000.0f, -5500.0f+z);

	glColor3f(0.130, 0.280, 0.450);
	glVertex3f(5000.0f, 3000.0f, -5500.0f+z);
	glVertex3f(5000.0f, -1000.0f, -5500.0f+z);

	glEnd();
	glEnable(GL_LIGHTING);
	
}

//BOUNDING BOX
void bounding_box(float xx, float yy, float zz,int a){

	int add = a;

	glColor4f(1.0, 0.0, 0.0, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(xx + 0.0 + add, yy + 0.0 + add, zz + 0 );
	glVertex3f(xx + 0.0 + add, yy + 0.0 + -add, zz + 0 );
	glVertex3f(xx + 0.0 + -add, yy + 0.0 + -add, zz + 0 );
	glVertex3f(xx + 0.0 + -add, yy + 0.0 + add, zz + 0 );
	glEnd();

}

//TORUS
void torus(int multiplier, bool collide){

	if (collide == true){
		glColor3f(0.0, 1.0, 0.0);
	}
	else
		glColor3f(1.0, 0.0, 0.0);



	glPushMatrix();
	glutSolidTorus(0.4*multiplier, 1.8*multiplier, 10, 30);
	glPopMatrix();

	//glDisable(GL_LIGHTING);

}
void draw_torus()
{
	

	float xx, yy, zz;
	int add = 15;

	for (int i = 0; i < no_of_tor; i++){
		glPushMatrix();
		xx = random_tor[i][0], yy = random_tor[i][1], zz = random_tor[i][2];
		glTranslated(xx, yy, zz);
		torus(10, collide[i]);
		//printf("%d\n", collide[i]);
		//int a=tor[i].checkpass();
		//printf("Pass:%d\n", p[i]);
		glPopMatrix();
		bounding_box(xx, yy, zz, add);
		tor[i].put(xx + 0.0 + add, yy + 0.0 + add, zz + 0 - add, xx + 0.0 + add, yy + 0.0 + -add, zz + 0 - add, xx + 0.0 + -add, yy + 0.0 + -add, zz + 0 - add, xx + 0.0 + -add, yy + 0.0 + add, zz + 0 - add);
	}
	//printf("\n");
	
}


void draw_para(){
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0);

	//box_x = (x - px);
	glVertex3f(x - px, y + py, z - 10.0);
	glVertex3f(x - px, y + 2 * py, z - 10.0);
	glVertex3f(x + px, y + 2 * py, z - 10.0);
	glVertex3f(x + px, y + py, z - 10.0);

	glEnd();
}

//AEROPLANE
void aeroplane_wings(){

	glBegin(GL_QUADS);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(5.0, 20.0, -40.0);
	glVertex3f(5.0, 20.0, -60.0);
	glVertex3f(5.0, 23.0, -60.0);
	glVertex3f(5.0, 23.0, -40.0);

	glColor3f(0.75, 0.0, 0.0);
	glVertex3f(5.0, 23.0, -40.0);
	glVertex3f(50.0, 23.0, -55.0);
	glVertex3f(50.0, 20.0, -55.0);
	glVertex3f(5.0, 20.0, -40.0);

	glColor3f(0.50, 0.0, 0.0);
	glVertex3f(50.0, 23.0, -55.0);
	glVertex3f(50.0, 23.0, -60.0);
	glVertex3f(50.0, 20.0, -60.0);
	glVertex3f(50.0, 20.0, -55.0);

	glColor3f(0.25, 0.0, 0.0);
	glVertex3f(50.0, 20.0, -60.0);
	glVertex3f(5.0, 20.0, -60.0);
	glVertex3f(5.0, 23.0, -60.0);
	glVertex3f(50.0, 23.0, -60.0);

	glColor3f(0.25, 0.0, 1.0);
	glVertex3f(5.0, 23.0, -60.0);
	glVertex3f(5.0, 23.0, -40.0);
	glVertex3f(50.0, 23.0, -55.0);
	glVertex3f(50.0, 23.0, -60.0);

	glColor3f(0.25, 1.0, 0.0);
	glVertex3f(5.0, 20.0, -40.0);
	glVertex3f(5.0, 20.0, -60.0);
	glVertex3f(50.0, 20.0, -60.0);
	glVertex3f(50.0, 20.0, -55.0);

	glEnd();
}
void aeroplane(){

	glPushMatrix();
	glTranslatef(0.0, 20.0, -100.0);
	glColor3f(0.0, 0.0, 1.0);
	glutSolidCylinder(10, 80, 40, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 20.0, -20.0);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidCone(10, 30, 40, 20);
	glPopMatrix();

	glPushMatrix();
	aeroplane_wings();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.0, 23.0, -40.0);
	glRotatef(180, 0, 0, 1);
	glTranslatef(-5.0, -20.0, 40.0);
	aeroplane_wings();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(1.0, 30.0, -70.0);
	glScalef(1, 0.35, 1);
	glRotatef(90, 0, 0, 1);
	glTranslatef(-5.0, -20.0, 40.0);
	aeroplane_wings();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.0, 23.0, -75.0);
	glScalef(0.45, 1, 0.45);
	glTranslatef(-5.0, -20.0, 40.0);
	aeroplane_wings();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.0, 26.0, -75.0);
	glScalef(0.45, 1, 0.45);
	glRotatef(180, 0, 0, 1);
	glTranslatef(-5.0, -20.0, 40.0);
	aeroplane_wings();
	glPopMatrix();

}
void animate_aeroplane(float a){

	glPushMatrix();
	glTranslatef(0.0, 780.0, a);
	aeroplane();
	glPopMatrix();
}

//CLOUDS
void clouds3(){
	glPushMatrix();
	glColor3ub(224, 224, 224);
	glTranslatef(0, 20, 0);
	glutSolidSphere(200, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(224, 224, 224);
	glTranslatef(100, 100,100 );
	glutSolidSphere(100, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(224, 224, 224);
	glTranslatef(-100, -100, -50);
	glutSolidSphere(200, 30, 30);
	glPopMatrix();
}
void clouds2(){
	glPushMatrix();
	//glTranslatef(-20,200,-200);
	glColor3ub(224, 224, 224);
	glTranslatef(0, 20, -40);
	glutSolidSphere(50, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(224, 224, 224);
	glTranslatef(0, 20, 0);
	glutSolidSphere(20, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(224, 224, 224);
	glTranslatef(10, 15, 10);
	glutSolidSphere(30, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(224, 224, 224);
	glTranslatef(10, 45,-30);
	glutSolidSphere(30, 20, 20);
	glPopMatrix();
}
void clouds()
{

	glPushMatrix();
	//glTranslatef(-20,200,-200);
	glColor3ub(224, 224, 224);
	glTranslatef(0, 20, -40);
	glutSolidSphere(10, 20, 20);
	glPopMatrix();
	
	glPushMatrix();
	glColor3ub(224, 224, 224);
	glTranslatef(0, 20, -30);
	glutSolidSphere(5, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(224, 224, 224);
	glTranslatef(-10, 30, -30);
	glutSolidSphere(10, 30, 20);
	glPopMatrix();
	
	glPushMatrix();
	glColor3ub(224, 224, 224);
	glTranslatef(0, 25, -30);
	glutSolidSphere(10, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(224, 224, 224);
	glTranslatef(0, 10, -30);
	glutSolidSphere(10, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(224, 224, 224);
	glTranslatef(0, 20, -50);
	glutSolidSphere(10, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(224, 224, 224);
	glTranslatef(-20, 20, -50);
	glutSolidSphere(20, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(224, 224, 224);
	glTranslatef(-15, 20, -10);
	glutSolidSphere(20, 20, 20);
	glPopMatrix();
	
}
void draw_clouds(float rZ){
	float mcolor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat qaWhite[] = { 1.0, 1.0, 1.0, 1.0 }; //White Color
	GLfloat qaRed[] = { 1.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_LEFT, GL_AMBIENT_AND_DIFFUSE, mcolor);
	glMaterialfv(GL_LEFT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_LEFT, GL_SHININESS, 20);

	for (int i = 0; i < no_of_obj; i++){
		glPushMatrix();
		
		glTranslatef(random_z[i][0]+50,1100,random_z[i][2]+100+rZ);
		
		clouds();
		glPopMatrix();
	}

	
}

//TREES
void trees()
{

	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.0, 20.0, -40.0);
	glRotatef(90, 1, 0, 0);
	glutSolidCylinder(3, 40, 40, 20);
	glTranslatef(0.0, -20.0, 40.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.8, 0.0);
	glTranslatef(0.0, 20.0, -40.0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(7, 10, 40, 20);
	glTranslatef(0.0, -20.0, 40.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.7, 0.0);
	glTranslatef(0.0, 13.0, -40.0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(10, 10, 40, 20);
	glTranslatef(0.0, -20.0, 40.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.6, 0.0);
	glTranslatef(0.0, 4.0, -40.0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(15, 12, 40, 20);
	glTranslatef(0.0, -20.0, 40.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.6, 0.0);
	glTranslatef(0.0, -5.0, -40.0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(20, 16, 5, 5);
	glTranslatef(0.0, -20.0, 40.0);
	glPopMatrix();


}
void draw_tree(float rZ){
	for (int i = 0; i < no_of_obj; i++){
		glPushMatrix();

		glTranslatef(random_z[i][0] + 50, 10, random_z[i][2] +100+rZ);
		glScaled(3, 3, 3);
		trees();
		glPopMatrix();
	}

}

//MOUNTAINS
void mountains(float base, float height){
	GLUquadricObj *p = gluNewQuadric();
	gluQuadricDrawStyle(p, GL_LINES);

	glPushMatrix();
	glColor3ub(136, 0, 21);
	glTranslatef(0, 0, -50);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(-5.0, -20.0, 40.0);
	gluCylinder(p, base, 50, height, 20, 20);
	glPopMatrix();
}
void draw_mountain(){
	float mountainY=-15.0;

	glPushMatrix();
	glTranslatef(0, mountainY, -5000);
	mountains(100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(200, mountainY, -5000);
	mountains(150, 200);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-200, mountainY, -5000);
	mountains(175, 300);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-400, mountainY, -5000);
	mountains(100, 150);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-700, mountainY, -5000);
	mountains(200, 400);
	glPopMatrix();

}

//TEXT RENDERING
void stroke_text(int tx,int ty,int tz,float sx,float sy,float sz,char *str){
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);

	glTranslatef(tx,ty,tz);
	glScalef(sx,sy,sz);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)str);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

//RENDER INSTRUCTION TEXT
void render_instruct(){

	char instruct_title[] = " INSTRUCTIONS";
	char welcome[] = "Welcome , Read the Instructions before playing";
	char i1[] = "To Move the skydiver press UP,DOWN,LEFT,RIGHT";
	char i2[] = "Pass through the Rings to progress the game";
	char i3[] = "If you miss three consecutive rings -> GAME OVER";
	char i4[] = " ENjoy";
	char press[] = "Press Space to Continue";

	glDisable(GL_LIGHTING);
	glColor3b(0.0, 0.0, 0.0);
	stroke_text(-16, 414, -3000, 0.015, 0.015, 0.025, instruct_title);
	stroke_text(-30, 410, -3000, 0.015, 0.015, 0.025, welcome);
	stroke_text(-30, 406, -3000, 0.015, 0.015, 0.025, i1);
	stroke_text(-30, 402, -3000, 0.015, 0.015, 0.025, i2);
	stroke_text(-30, 398, -3000, 0.015, 0.015, 0.025, i3);
	stroke_text(-30, 394, -3000, 0.015, 0.015, 0.025, i4);
	stroke_text(-30, 390, -3000, 0.015, 0.015, 0.025, press);
	glEnable(GL_LIGHTING); 

}

//RENDER INTRODUTION TEXT
void render_text(){

	char title[] = "GRAPHICAL VISUALIZATION OF SKYDIVING";
	char clgName[] = "Srinivas School of Engineering";
	char dept[] = "Department of Computer Science";
	char year[] = "2016";

	char amal[] = "AMAL KRISHNAN KP";
	char amal_usn[] = "4ES13CS006";

	char unais[] = "UNAIS IB";
	char unais_usn[] = "4ES12CS077";

	char press[] = "Press Enter to Start";

	glDisable(GL_LIGHTING);
	glColor3b(0.0, 0.0, 0.0);
	stroke_text(-20, 417, -3000, 0.020, 0.020, 0.020,clgName);
	stroke_text(-16, 414, -3000, 0.015, 0.015, 0.025, dept);
	stroke_text(-3, 412, -3000, 0.010, 0.010, 0.025, year);
	stroke_text(-35, 400, -3000, 0.025, 0.025, 0.025, title);
	stroke_text(-35, 395, -3000, 0.010, 0.010, 0.010, amal);
	stroke_text(-35, 393, -3000, 0.010, 0.010, 0.010, amal_usn);
	stroke_text(25, 395, -3000, 0.010, 0.010, 0.010, unais);
	stroke_text(25, 393, -3000, 0.010, 0.010, 0.010, unais_usn);
	stroke_text(-10, 387, -3000, 0.015, 0.015, 0.015, press);

	glEnable(GL_LIGHTING);
	
}

//RENDER GAME OVER TEXT
void render_over(){

	char game_over[] = "GAME OVER";

	glDisable(GL_LIGHTING);
	glColor3b(0.0, 0.0, 0.0);
	stroke_text(-13, 400, -3000, 0.025, 0.025, 0.025, game_over);
	glEnable(GL_LIGHTING);
}

//CHECK COLLISION
static int checkCollision(Objectbox a, Objectbox b, int num){
	//x-axis
	int Q = 15;
	if (p[num] == 0){
		if (a.TopLeft[0] > b.TopLeft[0] && a.TopRight[0] < b.TopRight[0])
		{
			//printf("x=%f x=%f x=%f x=%f \n", a.TopLeft[0], b.TopLeft[0], a.TopRight[0], b.TopRight[0]);
			if (a.TopLeft[1] < b.TopLeft[1] && a.BottomLeft[1] > b.BottomLeft[1])
			{
				//printf("y=%f y=%f y=%f y=%f \n", a.TopLeft[1], b.TopLeft[1], a.BottomLeft[1], b.BottomLeft[1]);
				if (a.TopLeft[2] <= b.TopLeft[2])
				{
					//b.putCollide(true);
					collide[num] = true;
					if (b.checkpass() == 1)
						return 101;
					p[num] = 1;
					//printf("\n \n COLLISIONNNN !!!! \n \n");
					//printf("%d\n", b.collide);
				}
				//printf("%f\t%f\n", a.TopLeft[2], b.TopLeft[2]);
				if (a.TopLeft[2] == b.TopLeft[2] + Q){
					hit++;
					//miss = 3;
				}
			}
			else
			{

				if (a.TopLeft[2] == b.TopLeft[2] + Q)
				{
					miss--;
				}

			}
		}
		else
		{
			if (a.TopLeft[2] == b.TopLeft[2] + Q)
			{
				miss--;
			}
		}


		if (miss ==0)
			ok = 4;

	}
	else if (p[num] == 1)
	{
		collide[num] = true;
		if (b.checkpass() == 1)
			return 101;
	}
	else if (p[num] == 2){
		p[num] = 2;
	}
}

//SKYDIVER
void skydiver_jump(){

	int add = 10;
		glPushMatrix();
		glTranslatef(0.0, 20.0, -100.0);
		glScaled(0.65, 0.25, 1.5);
		glColor3f(0.0, 0.0, 1.0);
		glutSolidCube(10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0, 20.0, -110.0);
		glScaled(0.40, 0.40, 0.40);
		glColor3f(1.0, 1.0, 0.0);
		glutSolidSphere(5, 20, 20);
		glPopMatrix();

		



		glPushMatrix();
		glTranslatef(-2.4, 20.0, -90.0);
		glScaled(0.20, 0.25, 1);
		glColor3f(1.0, 1.0, 0.0);
		glutSolidCube(10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(2.4, 20.0, -90.0);
		glScaled(0.20, 0.25, 1);
		glColor3f(1.0, 1.0, 0.0);
		glutSolidCube(10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-3, 28.0, -81.0);
		glRotatef(90, 1, 0, 0);
		glScaled(0.20, 0.25, 0.60);
		glTranslatef(2.4, -20.0, 10.0);
		glColor3f(1.0, 0.5, 0.0);
		glutSolidCube(10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(2.0, 28.0, -81.0);
		glRotatef(90, 1, 0, 0);
		glScaled(0.20, 0.25, 0.60);
		glTranslatef(2.4, -20.0, 10.0);
		glColor3f(1.0, 0.5, 0.0);
		glutSolidCube(10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-2.4, 24.0, -85.0);
		glScaled(0.20, 0.25, 0.5);
		glColor3f(1.0, 0.0, 0.0);
		glutSolidCube(10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(2.4, 24.0, -85.0);
		glScaled(0.20, 0.25, 0.5);
		glColor3f(1.0, 0.0, 0.0);
		glutSolidCube(10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.60, 25.0, -105.0);
		glRotatef(90, 0, 1, 0);
		glScaled(0.20, 0.25, 0.50);
		glTranslatef(2.4, -20.0, 10.0);
		glColor3f(1.0, 1.0, 0.0);
		glutSolidCube(10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-10, 25.0, -105.0);
		glRotatef(90, 0, 1, 0);
		glScaled(0.20, 0.25, 0.50);
		glTranslatef(2.4, -20.0, 10.0);
		glColor3f(1.0, 1.0, 0.0);
		glutSolidCube(10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(7, 20.0, -108.0);
		glScaled(0.20, 0.25, 0.6);
		glColor3f(1.0, 0.0, 0.0);
		glutSolidCube(10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-7, 20.0, -108.0);
		glScaled(0.20, 0.25, 0.6);
		glColor3f(1.0, 0.0, 0.0);
		glutSolidCube(10);
		glPopMatrix();
}
void draw_skydiver_jump(){

	float add = 5.0f;
	//bool collide;

	glPushMatrix();
	glTranslated(x + sky_x, y + sky_y, z + sky_z);
	skydiver_jump();
	


	glPopMatrix();

	glDisable(GL_LIGHTING);
	glColor4f(0.0, 0.0, 1.0, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(x + sky_x + 0.0 + add, y + sky_y + 20.0 + add, z + sky_z - 110 - add);
	//printf("%f %f %f \n", x + sky_x + 0.0 + add, y + sky_y + 20.0 + add, z + sky_z - 110 - add);
	glVertex3f(x + sky_x + 0.0 + add, y + sky_y + 20.0 + -add, z + sky_z - 110 - add);
	glVertex3f(x + sky_x + 0.0 + -add, y + sky_y + 20.0 + -add, z + sky_z - 110 - add);
	glVertex3f(x + sky_x + 0.0 + -add, y + sky_y + 20.0 + add, z + sky_z - 110 - add);
	glEnable(GL_LIGHTING);
	glEnd();

	skyd.put(x + sky_x + 0.0 + add, y + sky_y + 20.0 + add, z + sky_z - 110 - add,
		x + sky_x + 0.0 + add, y + sky_y + 20.0 + -add, z + sky_z - 110 - add,
		x + sky_x + 0.0 + -add, y + sky_y + 20.0 + -add, z + sky_z - 110 - add,
		x + sky_x + 0.0 + -add, y + sky_y + 20.0 + add, z + sky_z - 110 - add);

	//printf("%f %f %f %f %f %f \n", skyd.TopRight[0], skyd.TopRight[1], skyd.TopRight[2], skyd.BottomRight[0], skyd.BottomRight[1], skyd.BottomRight[2]);

	for (int i = 0; i < no_of_tor; i++){
		checkCollision(skyd, tor[i], i);
	}
}

void cam(){
	int tempx = 50;

	if (aero > -500 &&tempx>0)
		tempx -= 2;
	gluLookAt(x, y, z, x + lx, y - 0.15, z + lz, 0.0f, 1.0f, 0.0f);
}

//SUN
void draw_sun(){
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glTranslatef(300, 700, -4000+z);
	glutSolidSphere(50, 30, 20);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

//HEADS UP DISPLAY OF THE GAME
void HUD(){

	char score[] = "Score";
	char rings[] = "Rings";
	char inst[] = "                                              q-Quit";
	char lives[] = "Lives";


	glDisable(GL_LIGHTING);
	glColor3b(0.0, 0.0, 0.0);

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);

	if (z<200)
		high_score = (-(z - 200));

	std::string str = std::to_string(high_score);
	const char* temp;
	temp = str.c_str();

	std::string str2 = std::to_string(hit);
	const char* hits;
	hits = str2.c_str();

	std::string str3 = std::to_string(miss);
	const char* misses;
	misses = str3.c_str();

	
	//Score Value
	glTranslatef(x - 43, y + 10, z - extra);
	glScalef(0.015, 0.015, 0.025);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)temp);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	//Display "Score"

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);

	glTranslatef(x - 43, y + 12.5, z - extra);
	glScalef(0.015, 0.015, 0.025);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)score);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)inst);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	//Display "Rings"
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);

	glTranslatef(x - 43, y + 7.5, z - extra);
	glScalef(0.015, 0.015, 0.025);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)rings);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	//Rings value
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);

	glTranslatef(x - 43, y + 5.0, z - extra);
	glScalef(0.015, 0.015, 0.025);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)hits);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	//Lives
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);

	glTranslatef(x + 35, y + 8, z -extra);
	glScalef(0.015, 0.015, 0.025);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)lives);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	//Lives value
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);

	glTranslatef(x + 35, y + 6, z - extra);
	glScalef(0.015, 0.015, 0.025);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)misses);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_LIGHTING);
}

//DISPLAY FUNCTION
void display(void) {
	
	if (ok == 1){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
	
		gluLookAt(0.0, 400.0, -2950.0, 0.0,400.0, -3000.0, 0.0f, 1.0f, 0.0f);
		draw_sky();
		animate_aeroplane(aero2);

		//gluLookAt(0.0, 400.0, 10.0, 0.0, 400.0, -100.0, 0.0f, 1.0f, 0.0f);
	    render_text();

		glutSwapBuffers();
		
	}
	if (ok == 2){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		gluLookAt(0.0, 400.0, -2950.0, 0.0, 400.0, -3000.0, 0.0f, 1.0f, 0.0f);
		draw_sky();
		render_instruct();
		glutSwapBuffers();
	}
	if (ok == 3){

		// Clear Color and Depth Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Reset transformations
		glLoadIdentity();

		// Set the camera
		//cam();
		gluLookAt(x, y, z, x + lx, y - 0.15, z + lz, 0.0f, 1.0f, 0.0f);

		camera();
		// Draw ground
		draw_ground();
		
		//Draw torus
		draw_torus();
		//draw_mountains(50);
		draw_mountain();
		draw_sky();

		if (z>200)
			animate_aeroplane(aero);

		if (z < -6200)
		{
			draw_clouds(-6000);
			draw_tree(-6000);
		}
		else{
			draw_clouds(0);
			draw_tree(0);
		}

		if (z < 450)
			draw_skydiver_jump();

		draw_sun();
		HUD();

		glutSwapBuffers();
		Sleep(1);
		
		//glFlush();
		//printf("Z : %f",z);

	}
	
	if (ok == 4){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		gluLookAt(0.0, 400.0, -2950.0, 0.0, 400.0, -3000.0, 0.0f, 1.0f, 0.0f);
		
		glDisable(GL_LIGHTING);

		glBegin(GL_QUADS);

		glColor3f(0.0, 0.6, 0.8);
		glVertex3f(-5000.0f, -1000.0f, -5500.0f);
		glVertex3f(-5000.0f, 3000.0f, -5500.0f);

		glColor3f(0.130, 0.280, 0.450);
		glVertex3f(5000.0f, 3000.0f, -5500.0f);
		glVertex3f(5000.0f, -1000.0f, -5500.0f);

		glEnd();
		glEnable(GL_LIGHTING);

		render_over();

		glutSwapBuffers();
	}
	
}

void changeSize(int w, int h)
{
	
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 6000.0f);


	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

//KEYBOARD 
void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.5f;
	
	switch (key) {
		
	case GLUT_KEY_LEFT:
		sky_x -=2.0f;
		break;
	case GLUT_KEY_RIGHT:
		sky_x += 2.0f;
		break;
	case GLUT_KEY_UP:
		sky_y += 2.0f;
		break;
	case GLUT_KEY_DOWN:
		sky_y -= 2.0f;
		break;
		
		
	/*case GLUT_KEY_LEFT:
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx * fraction;
		z += lz * fraction;
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fraction;
		z -= lz * fraction;
		break;
	*/	




	}
}
void processNormalKeys(unsigned char key, int xx, int yy)
{
	float fraction = 0.1f;

	switch (key) {
	case 113:exit(0);
	case 65://y -= ly * fraction;
		y -= 5;
			break;
	case 97:y += ly * fraction;
			//z += lz * fraction;
			break;
	case 13: ok = 2;
			break;
	case 32: ok = 3;
			break;
	}

}

//MENU
void menu(int op) {

	switch (op) {
	case 0:glutFullScreen();
		break;
	case 1:
	case 'Q':
	case 'q':
		exit(0);
	}
}

//LIGHTING
void lighting(){

	glEnable(GL_COLOR_MATERIAL);
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glShadeModel(GL_SMOOTH);

	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv) {
	limit = 50;
	
	rand_tor();
	random2();
	random_mountain(50);
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1366, 768);

	glutCreateWindow("Amal");

	// register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);
	//glutIdleFunc(display);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	int subMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Full Screen", 0);
	glutAddMenuEntry("Exit", 'q');

	glutCreateMenu(menu);
	glutAddSubMenu("Sub Menu", subMenu);
	glutAddMenuEntry("Quit", 'q');
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// OpenGL init
	
	glEnable(GL_DEPTH_TEST);
	
	Timer(0);
	// enter GLUT event processing cycle
	lighting();
	glutMainLoop();

	return 1;
}


