#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <dos.h>

using namespace std;

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define PI 3.1416

static GLfloat spin = 0.0;
//part 2 initialization
static float tx = 0.0, bx = 1.0, by = 1.0, isbaby = 0, dolnay = 0, glUp = 1, angle = 0, step = 0, stepx = 0, stepy = 0, cloudleftx = 0, cloudmiddlex = 0, cloudrightx = 0, flag = 0, temp = 0, cloudrightupx = 0, countdolna = 0, babyx = -6, manx = -1, many = 0, rFlag, x, y, z;

static float ty = 5.0;

static float cloudLeftMove = 0, hutMove = 70, hutx = 0;

static float xx = 10, yy = 13;
//End

typedef struct
{
    int x, y;
} point;

point* arrayCities = NULL; //to store the cities positions
int nCities;               //number of cities
int* position = NULL;      //order to visit the cities
double** distances;        //a pre-computed array of distances between cities (to speedup)
double initialDistance;    //a value to print at the end of the running
int choice = 0;

void land()
{
    glPushMatrix();
    glBegin(GL_POLYGON);

    glColor3ub(143, 221, 54);
    glVertex2d(310, 0);
    glVertex2d(315, 325);

    glVertex2d(0, 325);

    glColor3ub(95, 182, 12);
    glVertex2d(-305, 0);
    glVertex2d(-305, -305);

    glVertex2d(-415, 300);
    glVertex2d(415, -300);

    glColor3ub(111, 171, 54);
    glVertex2d(315, -30);
    glVertex2d(295, 0);

    glVertex2d(395, 200);
    glVertex2d(-195, 250);

    glColor3ub(9, 195, 16);
    glVertex2d(-195, -250);

    glVertex2d(-255, -200);
    glVertex2d(-300, -350);

    glVertex2d(300, -350);
    glVertex2d(-300, 350);

    glVertex2d(-50, 360);
    glVertex2d(350, -460);

    glVertex2d(390, -260);
    glEnd();
    glPopMatrix();
}

void display2()
{
    glClearColor(0.215, 0.835, 0.976, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    land();
    glColor3ub(0, 0, 0);
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    for (int k = 0; k < nCities; k++)
        glVertex2i(arrayCities[position[k]].x, arrayCities[position[k]].y);
    glEnd();

    glPointSize(3);
    glColor3ub(255, 5, 5);
    glPointSize(10);
    for (int k = 0; k < nCities; k++)
    {
        glBegin(GL_POINTS);
        glVertex2i(arrayCities[k].x, arrayCities[k].y);
        glEnd();
    }
    glutSwapBuffers();
}

double computeDistance()
{
    double distancePath = 0.0;
    for (int k = 0; k < nCities - 1; k++)
        distancePath += distances[position[k]][position[k + 1]];
    distancePath += distances[position[nCities - 1]][position[0]];
    return distancePath;
}

void Swap2Elements(int* e1, int* e2)
{
    do
    {
        *e1 = rand() % nCities;
        *e2 = rand() % nCities;
    } while (*e1 == *e2);
    int temp = position[*e1];
    position[*e1] = position[*e2];
    position[*e2] = temp;
}

void getShortestPath()
{
    int k = 0;
    int limit = 10000;
    while (k < 25000 && limit > 0)
    {
        int e1, e2;
        double d1 = computeDistance();
        Swap2Elements(&e1, &e2);
        double d2 = computeDistance();
        if (d2 > d1)
        {
            int temp = position[e1];
            position[e1] = position[e2];
            position[e2] = temp;
            limit--;
        }
        else
            printf("Distance iteration-%d : %lf\n", k, computeDistance());
        k++;
    }
}

void onKeyDown(unsigned char key, int x, int y)
{
    if (key == 8)
        exit(0);
    else if (key == 32)
    {
        getShortestPath();
        printf("Initial distance: %lf; Shortest path: %lf\n", initialDistance, computeDistance());
    }
    else if (key == 's')
    {
        int e1, e2;
        double d1 = computeDistance();
        Swap2Elements(&e1, &e2);
        double d2 = computeDistance();
        if (d2 > d1)
        {
            int temp = position[e1];
            position[e1] = position[e2];
            position[e2] = temp;
        }
        printf("Distance: %lf \n", computeDistance());
    }
    glutPostRedisplay();
}

void onResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

double distanceFinder(point a, point b)
{
    return sqrt(pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0));
}

void Init(int size)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_POINT_SMOOTH);

    nCities = size;    // set the number of cities given by the user
    srand(time(NULL)); //to start each execution as different

    //dynamically create the arrays
    //arrayCities = new point[size];
    arrayCities = (point*)malloc(size * sizeof(point));
    position = (int*)malloc(size * sizeof(int));
    distances = (double**)malloc(size * sizeof(double*));

    // place the cities on the screen
    for (int k = 0; k < size; k++)
    {
        distances[k] = (double*)malloc(size * sizeof(double));
        int ri = rand() % (WINDOW_WIDTH + 1);
        ri -= WINDOW_WIDTH / 2;
        arrayCities[k].x = ri;
        ri = rand() % (WINDOW_HEIGHT + 1);
        ri -= WINDOW_HEIGHT / 2;
        arrayCities[k].y = ri;
        position[k] = k;
    }

    //computes the distance from all cities to all cities
    for (int y = 0; y < size; y++)
        for (int x = 0; x < size; x++)
            distances[x][y] = distanceFinder(arrayCities[x], arrayCities[y]); //from city y to each x

    initialDistance = computeDistance();
    printf("Initial distance: %lf\n", initialDistance);
}

//part 2 function

void circle(float radius_x, float radius_y)
{
    int i = 0;
    float angle1 = 0.0;

    glBegin(GL_POLYGON);

    for (i = 0; i < 100; i++)
    {
        angle1 = 2 * PI * i / 100;
        glVertex3f(cos(angle1) * radius_x, sin(angle1) * radius_y, 0);
    }

    glEnd();
}

void halfcircle(float radius_x, float radius_y)
{
    int i = 0;
    float angle1 = 0.0;

    glBegin(GL_POLYGON);

    for (i = 0; i < 51; i++)
    {
        angle1 = 2 * PI * i / 100;
        glVertex3f(cos(angle1) * radius_x, sin(angle1) * radius_y, 0);
    }

    glEnd();
}
void hillTree()
{
    glPushMatrix(); //Big tree
    glTranslated(-15, 6, 0);
    glScaled(0.5, 0.2, 0);
    glPushMatrix();
    glTranslated(-1, -10, 1);
    glColor3f(0.55, 0.27, 0.0745);
    glBegin(GL_QUADS);
    glVertex2d(0.0, 0.0);
    glVertex2d(0.0, 10.0);
    glVertex2d(1.0, 10.0);
    glVertex2d(1.0, 0.0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.5, 0, 0);
    glScaled(0.4, 1, 1);
    glPushMatrix();
    glColor3f(0.0, 0.39, 0);
    glTranslatef(0, 5, 0);
    circle(7, 6);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-5.5, 1.0, 1);
    glColor3f(0.0, 0.39, 0);
    glTranslatef(0, 5, 0);
    circle(3, 2);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-5.0, 4.0, 1);
    glColor3f(0.0, 0.39, 0);
    glTranslatef(0, 5, 0);
    circle(3, 2);
    glPopMatrix();

    glPushMatrix();
    glTranslated(5.5, -0.5, 1);
    glColor3f(0.0, 0.39, 0);
    glTranslatef(0, 5, 0);
    circle(4, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslated(5.0, 4.0, 1);
    glColor3f(0.0, 0.39, 0);
    glTranslatef(0, 5, 0);
    circle(3, 2);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0, 6.0, 1);
    glColor3f(0.0, 0.39, 0);
    glTranslatef(0, 5, 0);
    circle(4, 2);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-5.5, -3.0, 1);
    glColor3f(0.0, 0.39, 0);
    glTranslatef(0, 5, 0);
    circle(4, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslated(5.5, -3.5, 1);
    glColor3f(0.0, 0.39, 0);
    glTranslatef(0, 5, 0);
    circle(3, 2);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();
}

void hillSide()
{
    glPushMatrix();
    glScaled(1, 0.6, 0);
    glPushMatrix(); //pahar
    glTranslated(-9.5, 0.0, 1);
    glColor3f(0.690, 0.921, 0.098);
    glTranslatef(0, 5, 0);
    halfcircle(16, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(8.5, 0.0, 1);
    glColor3f(0.603, 0.827, 0.011);
    glTranslatef(0, 5, 0);
    halfcircle(15, 7);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.403, 0.701, 0.019);
    glTranslated(-4.5, 0.0, 1);
    glTranslatef(0, 5, 0);
    halfcircle(6, 3);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-13.4, 5, 1);

    glBegin(GL_TRIANGLES);
    glColor3f(0.403, 0.701, 0.019);
    glVertex2f(1.0, 0.0);
    glVertex2f(4.0, 1.75);
    glVertex2f(5.0, 0.0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(4.2, 5, 1);
    glRotated(180, 0, 1, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(1.0, 0.0);
    glVertex2f(4.0, 1.78);
    glVertex2f(5.0, 0.0);
    glEnd();
    glPopMatrix();
    glPopMatrix();
}

void sky()
{
    glPushMatrix();
    glColor3f(0.411, 0.933, 0.972);

    glBegin(GL_QUADS);
    glVertex2d(-20, 5.0);
    glVertex2d(20, 5.0);
    glVertex2d(20, 10);
    glVertex2d(-20, 10);
    glEnd();
}

void river()
{

    glPushMatrix();
    glTranslatef(0, 20, 0);
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.87, 0);
    glVertex2d(-150.0, -16.4);
    glVertex2d(150.0, -16.4);
    glVertex2d(150.0, -16.5);
    glVertex2d(-150.0, -16.5);
    glEnd();
    glPopMatrix();

    glPushMatrix();

    glBegin(GL_QUADS);
    glColor3f(0, 0.565f, 0);
    glVertex2d(150.0, -16.5);
    glColor3f(0.0f, 1.0f, 0);
    glVertex2d(-150.0, -16.5);
    glColor3f(0.062f, 0.31f, 0.545f);
    glVertex2d(-150.0, -20);
    glColor3f(0.388f, 0.72f, 1.0f);
    glVertex2d(150.0, -20);
    glEnd();

    glPopMatrix();
    glPopMatrix();
}

void field()
{
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.196, 0.560, 0.015);
    glVertex2d(-20.0, 4);
    glVertex2d(20.0, 4);
    glVertex2d(20, -50);
    glVertex2d(-20.0, -50);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.250, 0.749, 0.003);
    glVertex2d(-20.0, 2);
    glVertex2d(20.0, 0);
    glVertex2d(20, -2);
    glVertex2d(-20.0, -4);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.250, 0.749, 0.003);
    glVertex2d(-20.0, -6);
    glVertex2d(20.0, -8);
    glVertex2d(20, -9);
    glVertex2d(-20.0, -12);
    glEnd();
    glPopMatrix();
}

void cloudMiddle()
{
    cloudmiddlex = cloudmiddlex - 0.0005;
    if (cloudmiddlex < -20)
    {
        cloudmiddlex = 20;
    }
    //printf("%f\n",cloudmiddlex);
    glPushMatrix();
    glTranslated(-9.5 + cloudmiddlex, 16, 1);
    glScaled(0.5, 0.2, 1);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslated(0, -38, 1),
        circle(1.5, 2.5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(1, -35, 1),
        circle(2, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslated(3, -35, 1),
        circle(2, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslated(4.5, -37.5, 1),
        circle(1.5, 2.5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(2.5, -38.5, 1),
        circle(1.5, 2.5);
    glPopMatrix();
    glPopMatrix();
}

void cloudRight()
{

    cloudrightx -= 0.0005;
    if (cloudrightx < -35)
    {
        cloudrightx = 20;
    }
    //printf("%f\n",cloudrightx);

    glPushMatrix();
    glTranslated(-1.5 + cloudrightx, 16, 1);
    glScaled(0.5, 0.2, 1);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslated(0, -38, 1),
        circle(1.5, 2.5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(1, -35, 1),
        circle(2, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslated(3, -35, 1),
        circle(2, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslated(4.5, -37.5, 1),
        circle(1.5, 2.5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(2.5, -38.5, 1),
        circle(1.5, 2.5);
    glPopMatrix();
    glPopMatrix();
}

void sun()
{
    glPushMatrix();
    glTranslated(-4, 8, 0);
    glScaled(0.3, 0.3, 0);
    glColor3f(1, 1, 0);
    circle(3.5, 3.5);
    glPopMatrix();
}

void optimalmove()
{
    if (flag == 0)
    {
        manx += 0.002;
        many -= 0.0003;
        if (manx > 10 || many < -2)
        {
            manx = 9.999;
            many = -1.9998;
            flag = 1;
        }
    }
    if (flag == 1)
    {
        manx += 0.005;
        if (manx > 10)
        {
            manx = 9.999;
            flag = 2;
        }
    }

    if (flag == 2)
    {
        manx += 0.001;
        many -= 0.001;
        if (many < -8 || manx > 16)
        {
            many = -7.999;
            manx = 15.999;

            flag = 3;
        }
    }

    if (flag == 3)
    {
        manx -= 0.003;
        if (manx < -15)
        {
            manx = -14.999;
            flag = 4;
        }
    }
    if (flag == 4)
    {
        manx += 0.001;
        many += 0.001;
        if (manx > -8)
        {
            manx = -1;
            many = 0;
            flag = 5;
        }
    }
    glTranslated(-0.5 + manx, 0.5 + many, 1);
}

void unoptimizedmove()
{
    if (temp == 0)
    {
        manx -= 0.002;
        many -= 0.0002;
        if (manx < -10 || many < -2)
        {
            manx = -9.999;
            many = -1.9998;
            temp = 1;
        }
    }
    if (temp == 1)
    {
        manx += 0.002;
        many -= 0.0003;
        if (manx > 16 || many < -8)
        {
            manx = 15.999;
            many = -7.9998;
            temp = 2;
        }
    }

    if (temp == 2)
    {
        manx -= 0.0008;
        many += 0.0008;
        if (many > -2.004 || manx < 10)
        {
            manx = 10;
            many = -2.04;
            temp = 3;
        }
    }

    if (temp == 3)
    {
        manx -= 0.0008;
        many -= 0.0008;
        if (many < -8 || manx < 5)
        {
            many = -8;
            manx = 5;

            temp = 4;
        }
    }

    if (temp == 4)
    {
        manx -= 0.002;
        if (manx < -15)
        {
            manx = -14.999;
            temp = 5;
        }
    }
    if (temp == 5)
    {
        manx += 0.001;
        many += 0.001;
        if (manx > -8)
        {
            manx = -1;
            many = 0;
            temp = 6;
        }
    }

    glTranslated(-0.5 + manx, 0.5 + many, 1);
}

void pond()
{
    glPushMatrix();
    glScaled(0.5, 0.2, 0);
    glBegin(GL_POLYGON);
    glColor3f(0.337, 0.745, 0.882);
    glVertex2d(-50, 2);
    glVertex2d(-80, 6);
    glColor3f(0.337, 0.745, 0.882);
    glVertex2d(-25, -5);
    glVertex2d(-20, -1);
    glVertex2d(-20, 7);

    glColor3f(0.337, 0.745, 0.882);
    glVertex2d(-62, 3);
    glVertex2d(-25, -5);
    glColor3f(0.392, 0.843, 0.890);
    glVertex2d(-38, 15);
    glVertex2d(-22, 15);
    glColor3f(0.482, 0.819, 0.976);
    glVertex2d(-45, 15);
    glVertex2d(-45, -5);
    glColor3f(0.168, 0.650, 0.913);
    //glVertex2d(-22, -5);

    glVertex2d(-18, 10);
    glVertex2d(-25, 12);
    glColor3f(0.482, 0.819, 0.976);
    glVertex2d(-25, 15);
    glVertex2d(-35, 6);
    glColor3f(0.392, 0.678, 0.890);
    glVertex2d(-29, 8);
    glVertex2d(-45, 4);
    glColor3f(0.482, 0.819, 0.976);
    glVertex2d(-40, 5);
    glVertex2d(-65, 3);
    glVertex2d(-25, -5);

    glEnd();
    glPopMatrix();
}

void man()
{
    glPushMatrix();

    if (rFlag == 1)
    {

        optimalmove();
    }
    else if (rFlag == 2)
    {

        unoptimizedmove();
    }
    glColor3f(0.333, 0.129, 0.027);
    glBegin(GL_POLYGON);
    for (int t = 0; t <= 360; t += 1)
    {
        x = 0 + 0.5 * (cos(t));
        y = 0.8 + 0.5 * (sin(t));
        z = 0;
        glVertex3f(x, y, z);
    }
    glEnd();
    glColor3f(0.411, 0.058, 0.8);
    glBegin(GL_QUADS);
    glVertex2d(-0.5, 0.5);
    glVertex2d(0.5, 0.5);
    glVertex2d(0.5, -0.5);
    glVertex2d(-0.5, -0.5);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.980, 0.819, 0.741);
    glVertex2d(-0.25, 0.5);
    glVertex2d(0.25, 0.5);
    glVertex2d(0, 0);
    glEnd();

    //neck

    //head

    glColor3f(0.980, 0.819, 0.741);
    glBegin(GL_POLYGON);
    for (int t = 0; t <= 360; t += 1)
    {
        x = 0 + 0.3 * (cos(t));
        y = 0.8 + 0.4 * (sin(t));
        z = 0;
        glVertex3f(x, y, z);
    }
    glEnd();
    glColor3f(0.980, 0.819, 0.741);
    glBegin(GL_QUADS);
    glVertex2d(-0.25, 0.5);
    glVertex2d(-0.15, 0.6);
    glVertex2d(0.15, 0.6);
    glVertex2d(0.25, 0.5);
    glEnd();

    glColor3f(0, 0, 0);
    glPointSize(3.0);
    glBegin(GL_POINTS);
    glVertex2f(0.1, 0.9);
    glVertex2f(-0.15, 0.9);
    glEnd();

    glBegin(GL_LINES);
    glLineWidth(3);
    glVertex2f(0.1, 0.7);
    glVertex2f(-0.15, 0.7);
    glEnd();

    //legs
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2d(-0.5, -0.5);
    glVertex2d(-0.15, -0.5);
    glVertex2d(-0.15, -1.5);
    glVertex2d(-0.5, -1.5);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2d(0.5, -0.5);
    glVertex2d(0.15, -0.5);
    glVertex2d(0.15, -1.5);
    glVertex2d(0.5, -1.5);
    glEnd();

    glColor3f(0.980, 0.819, 0.741);
    glBegin(GL_TRIANGLES);
    glVertex2d(-0.25, 0.5);
    glVertex2d(0.25, 0.5);
    glVertex2d(0, 0.3);
    glEnd();

    //hands
    glColor3f(0.980, 0.819, 0.741);
    glBegin(GL_QUADS);
    glVertex2d(-0.5, 0.5);
    glVertex2d(-0.5, 0.3);
    glVertex2d(-1.0, -0.2);
    glVertex2d(-1.0, -0.5);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2d(0.5, 0.5);
    glVertex2d(0.5, 0.3);
    glVertex2d(1.0, -0.2);
    glVertex2d(1.0, -0.5);
    glEnd();
    //
    glColor3f(0, 0, 0);

    glBegin(GL_LINES);

    glVertex2d(1, -0.5);
    glVertex2d(1, 1.7);

    glVertex2d(1, -0.5);
    glVertex2d(2, 1.6);

    glVertex2d(1, -0.5);
    glVertex2d(1.5, 1.4);

    glVertex2d(1, -0.5);
    glVertex2d(2.5, 1.8);

    glVertex2d(1, -0.5);
    glVertex2d(1.7, 2.0);
    glEnd();
    glColor3f(0.968, 0, 0.968);
    glBegin(GL_POLYGON);
    for (int t = 0; t <= 360; t += 1)
    {
        x = 1 + 0.3 * (cos(t));
        y = 1.7 + 0.4 * (sin(t));
        z = 0;
        glVertex3f(x, y, z);
    }
    glEnd();
    glColor3f(0, 0.270, 0.760);
    glBegin(GL_POLYGON);
    for (int t = 0; t <= 360; t += 1)
    {
        x = 2 + 0.3 * (cos(t));
        y = 1.6 + 0.4 * (sin(t));
        z = 0;
        glVertex3f(x, y, z);
    }
    glEnd();

    glColor3f(0.964, 0.337, 0);
    glBegin(GL_POLYGON);
    for (int t = 0; t <= 360; t += 1)
    {
        x = 1.5 + 0.3 * (cos(t));
        y = 1.4 + 0.4 * (sin(t));
        z = 0;
        glVertex3f(x, y, z);
    }
    glEnd();
    glColor3f(0.968, 0.901, 0.215);
    glBegin(GL_POLYGON);
    for (int t = 0; t <= 360; t += 1)
    {
        x = 2.5 + 0.3 * (cos(t));
        y = 1.8 + 0.4 * (sin(t));
        z = 0;
        glVertex3f(x, y, z);
    }
    glEnd();
    glColor3f(0.666, 0.152, 0.949);
    glBegin(GL_POLYGON);
    for (int t = 0; t <= 360; t += 1)
    {
        x = 1.7 + 0.3 * (cos(t));
        y = 2 + 0.4 * (sin(t));
        z = 0;
        glVertex3f(x, y, z);
    }
    glEnd();
    glPopMatrix();
}

void hut()
{
    glPushMatrix();
    glScaled(0.5, 0.2, 0);
    glBegin(GL_POLYGON);
    glColor3f(0.980, 0.627, 0.019); //blue box front
    glVertex2d(-23.0f, -17.0f);
    glVertex2d(-23.0f, -11.0f);
    glVertex2d(-25.0f, -9.0f);
    glVertex2d(-27.0f, -11.0f);
    glVertex2d(-27.0f, -17.0f);
    glEnd();
    glBegin(GL_QUADS);

    glColor3f(0.360, 0.137, 0); //yellow box front
    glVertex2d(-22.0f, -17.0f);
    glVertex2d(-22.0f, -19.0f);
    glVertex2d(-28.0f, -19.0f);
    glVertex2d(-28.0f, -17.0f);

    glColor3f(0.537, 0.321, 0.141); //red stand front
    glVertex2d(-24.0f, -19.0f);
    glVertex2d(-24.0f, -15.0f);
    glVertex2d(-26.0f, -15.0f);
    glVertex2d(-26.0f, -19.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.980, 0.627, 0.019); //blue box front
    glVertex2d(23.0f, -17.0f);
    glVertex2d(23.0f, -11.0f);
    glVertex2d(25.0f, -9.0f);
    glVertex2d(27.0f, -11.0f);
    glVertex2d(27.0f, -17.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.360, 0.137, 0); //yellow box front
    glVertex2d(22.0f, -17.0f);
    glVertex2d(22.0f, -19.0f);
    glVertex2d(28.0f, -19.0f);
    glVertex2d(28.0f, -17.0f);

    glColor3f(0.537, 0.321, 0.141); //red stand front
    glVertex2d(24.0f, -19.0f);
    glVertex2d(24.0f, -15.0f);
    glVertex2d(26.0f, -15.0f);
    glVertex2d(26.0f, -19.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.980, 0.627, 0.019); //blue box front
    glVertex2d(3.0f, -37.0f);
    glVertex2d(3.0f, -31.0f);
    glVertex2d(5.0f, -28.0f);
    glVertex2d(7.0f, -31.0f);
    glVertex2d(7.0f, -37.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.360, 0.137, 0); //yellow box front
    glVertex2d(2.0f, -37.0f);
    glVertex2d(2.0f, -39.0f);
    glVertex2d(8.0f, -39.0f);
    glVertex2d(8.0f, -37.0f);

    glColor3f(0.537, 0.321, 0.141); //red stand front
    glVertex2d(4.0f, -39.0f);
    glVertex2d(4.0f, -35.0f);
    glVertex2d(6.0f, -35.0f);
    glVertex2d(6.0f, -39.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.980, 0.627, 0.019); //blue box front
    glVertex2d(33.0f, -37.0f);
    glVertex2d(33.0f, -31.0f);
    glVertex2d(35.0f, -28.0f);
    glVertex2d(37.0f, -31.0f);
    glVertex2d(37.0f, -37.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.360, 0.137, 0); //yellow box front
    glVertex2d(32.0f, -37.0f);
    glVertex2d(32.0f, -39.0f);
    glVertex2d(38.0f, -39.0f);
    glVertex2d(38.0f, -37.0f);

    glColor3f(0.537, 0.321, 0.141); //red stand front
    glVertex2d(34.0f, -39.0f);
    glVertex2d(34.0f, -35.0f);
    glVertex2d(36.0f, -35.0f);
    glVertex2d(36.0f, -39.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.980, 0.627, 0.019); //blue box front
    glVertex2d(-33.0f, -37.0f);
    glVertex2d(-33.0f, -31.0f);
    glVertex2d(-35.0f, -28.0f);
    glVertex2d(-37.0f, -31.0f);
    glVertex2d(-37.0f, -37.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.360, 0.137, 0); //yellow box front
    glVertex2d(-32.0f, -37.0f);
    glVertex2d(-32.0f, -39.0f);
    glVertex2d(-38.0f, -39.0f);
    glVertex2d(-38.0f, -37.0f);

    glColor3f(0.537, 0.321, 0.141); //red stand front
    glVertex2d(-34.0f, -39.0f);
    glVertex2d(-34.0f, -35.0f);
    glVertex2d(-36.0f, -35.0f);
    glVertex2d(-36.0f, -39.0f);
    glEnd();

    glPopMatrix();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    hillTree();
    sky();
    sun();
    field();
    cloudMiddle();
    cloudRight();
    hillSide();
    hut();
    pond();
    man();

    glutPostRedisplay();
    glFlush();
}

void init(void)
{
    glClearColor(1, 0, 0, 0.1);
    glOrtho(-20.0, 20.0, -10.0, 10.0, -15.0, 15.0);
}

void rotateMenu(int option)
{
    if (option == 1)
        rFlag = 1;
    if (option == 2)
        rFlag = 2;
    if (option == 3)
        exit(0);
}

int main(int argc, char* argv[])
{
    int op;
    cout << "\t\tMenu\n\n1 : Visualization for travelling saleman Problem\n\n2 : Working of Travelling Salesman Algorithm\n";
    cin >> op;
    glutInitWindowSize(1300, 900);
    switch (op)
    {

    case 1:
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

        glutInitWindowPosition(0, 0);
        glutCreateWindow("Travelling SalesMan Problem.");

        init();
        glutDisplayFunc(display);
        glutCreateMenu(rotateMenu);
        glutAddMenuEntry("Travel on Optimal Path", 1);
        glutAddMenuEntry("Travel on Unoptimized Path", 2);
        glutAddMenuEntry("Exit", 3);
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        break;
    case 2:

        int n;
        printf("Please insert the number of cities [5,100]: ");
        scanf("%d", &n);
        printf("\n");
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
        //glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
        glutCreateWindow("TSP Algorithm");
        glutDisplayFunc(display2);
        glutIdleFunc(display2);
        glutReshapeFunc(onResize);
        glutKeyboardFunc(onKeyDown);
        Init(n);
        break;
    }
    glutMainLoop();
    return 0;
}
