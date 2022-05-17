//
//  main.cpp
//  Graphics_Project
//
//  Created by Mohamed Bahgat Elrakaiby on 2016-12-06.
//  Copyright © 2016 Mohamed Bahgat Elrakaiby. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

double object_transform[4] = { 0, 0, 0, 0 };
double target_transform[4] = { 0, 0, -1, 0 };
double road_pos = -1;
double road_len = 1000;
double max_x = 1;
double max_distance = 15;
double speed_factor_target = 1;
double speed_factor_object = 0;
double gem_span_min = -1;
bool status = false;

void drawObject(){

	glColor3d(1, 1, 1);
	glPushMatrix();
	glColor3d(0, 0, 1);
	glTranslated(0, 0.5, 0);
	glTranslated(object_transform[0], object_transform[1], object_transform[2]);
	glRotated(0, 1, 0, object_transform[3]);
	glPushMatrix();
	glutSolidSphere(0.5, 100, 100);
	glPopMatrix();
	glPopMatrix();
}

void drawTarget()
{
	glColor3d(1, 1, 1);
	glPushMatrix();
	glColor3d(0, 1, 1);
	glTranslated(0, 0.5, 0);
	glTranslated(target_transform[0], target_transform[1], target_transform[2]);
	glRotated(0, 1, 0, target_transform[3]);
	glPushMatrix();
	glutSolidSphere(0.5, 100, 100);
	glPopMatrix();
	glPopMatrix();
}

void drawRoad(double r){
	glPushMatrix();
	glColor3d(0.5, 0.5, 0.5);
	glTranslated(0, 0, road_pos + 1);
	glRotated(0, 1, 0, r);
	glPushMatrix();
	glBegin(GL_QUADS);

	glVertex3d(-1.5, 0, road_len);
	glVertex3d(1.5, 0, road_len);
	glVertex3d(1.5, 0, -road_len);
	glVertex3d(-1.5, 0, -road_len);

	glEnd();
	glPopMatrix();
	glPushMatrix();
	glBegin(GL_QUADS);

	glVertex3d(1.5, 0, road_len);
	glVertex3d(1.5, 1, road_len);
	glVertex3d(1.5, 1, -road_len);
	glVertex3d(1.5, 0, -road_len);

	glEnd();
	glPopMatrix();
	glPushMatrix();
	glBegin(GL_QUADS);

	glVertex3d(-1.5, 0, road_len);
	glVertex3d(-1.5, 1, road_len);
	glVertex3d(-1.5, 1, -road_len);
	glVertex3d(-1.5, 0, -road_len);

	glEnd();
	glPopMatrix();
	glPopMatrix();

	glFlush();
}

void drawObstacle(){
	srand((unsigned)time(nullptr));
	double ran = rand() % 300;
	ran /= 100;
	ran -= 1.6;

	glColor3d(1, 1, 0.5);
	glPushMatrix();
	glTranslated(ran, 0.1, gem_span_min + 0.5);
	glPushMatrix();
	glutSolidCube(0.2);
	glPopMatrix();
	glPopMatrix();

	gem_span_min--;

}

void detect_reaching(){
	if (target_transform[2] - object_transform[2] >= max_distance)
	{
		status = false;
	}
	else if (target_transform[2] < object_transform[2])
	{
		status = true;
	}
}


void setupCamera(double x, double y, double z, double x1, double y1, double z1) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 300 / 300, 0.1f, 300.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, x1, y1, z1, 0.0f, 1.0f, 0.0f);
}


void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawObject();
	drawRoad(0);
	drawTarget();
	drawObstacle();
	glFlush();
}

void Anim(){
	speed_factor_target += 0.003;
	target_transform[2] -= 0.001 * speed_factor_target;
	target_transform[3] += 0.001 * speed_factor_target;
	if (!(target_transform[2] - 2 > road_pos)){

		road_pos -= 5;
	}
	setupCamera(0, object_transform[1] + 3, object_transform[2] + 3, 0, object_transform[1], object_transform[2]);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
	switch (key) {
	case 'd':
		object_transform[0] = (object_transform[0] < max_x) ? object_transform[0] + 0.2 : object_transform[0];

		setupCamera(0, object_transform[1] + 3, object_transform[2] + 3, 0, object_transform[1], object_transform[2]);
		glutPostRedisplay();
		break;

	case 'a':
		object_transform[0] = (object_transform[0] > -max_x) ? object_transform[0] - 0.2 : object_transform[0];

		setupCamera(0, object_transform[1] + 3, object_transform[2] + 3, 0, object_transform[1], object_transform[2]);
		glutPostRedisplay();
		break;

	case 'w':
		speed_factor_object += 0.3;
		object_transform[2] -= 0.01 * speed_factor_object;
		glutPostRedisplay();
		break;

	case 's':
		speed_factor_object -= 0.7;
		object_transform[2] -= 0.01 * speed_factor_object;
		object_transform[2] = (object_transform[2] <= 0) ? 0 : object_transform[2];
		glutPostRedisplay();
		break;

	}
}



int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(150, 150);

	glutCreateWindow("Graphics Project");
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glutDisplayFunc(Display);
	glutIdleFunc(Anim);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	glutKeyboardFunc(keyboard);
	//glutPassiveMotionFunc(MousePassiveMotion);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_COLOR_MATERIAL);

	setupCamera(0, object_transform[1] + 3, object_transform[2] + 3, object_transform[0], object_transform[1], object_transform[2]);

	glutMainLoop();
}

