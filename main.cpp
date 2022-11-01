#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <bits/stdc++.h>
#include "BmpLoader.h"

double Txval=0,Tyval=0,Tzval=0;
double windowHeight=700, windowWidth=800;
GLfloat alpha = 0.0, theta = 0.0, axis_x=0.0, axis_y=0.0;
GLboolean bRotate = false, uRotate = false;

int wheel=0,angle=0;

unsigned int ID[100];


GLint l1=0,l2=0,l3=0;
GLint a1=0,a2=0,a3=0,d1=0,d2=0,d3=0,s1=0,s2=0,s3=0;


GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_ambient1[]  = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_diffuse1[]  = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_ambient2[]  = {1.0, 0.0, 0.0, 1.0};
GLfloat light_diffuse2[]  = {1.0, 0.0, 0.0, 1.0 };

GLfloat light_ambient3[]  = {0.5, 0.5, 0.5, 1.0};
GLfloat light_diffuse3[]  = { 1.0, 1.0, 1.0, 1.0 };



static GLfloat Cube[8][3] =
{
    {0.0, 0.0, 0.0},    //0
    {1.0, 0.0, 0.0},    //1
    {1.0, 1.0, 0.0},    //2
    {0.0, 1.0, 0.0},    //3
    {0.0, 0.0, 1.0},    //4
    {1.0, 0.0, 1.0},    //5
    {1.0, 1.0, 1.0},    //6
    {0.0, 1.0, 1.0}     //7
};

static GLubyte Cube_indeces[6][4] =
{
    {1,5,4,0},   //bottom clockwise
    {3,7,6,2},   //top
    {0,4,7,3},   //left clockwise
    {2,6,5,1},   //right
    {4,5,6,7},   //front
    {3,2,1,0}

};

static void getNormal3p
(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

void LoadTexture(const char*filename,int x)
{
    glGenTextures(1, &ID[x]);
    glBindTexture(GL_TEXTURE_2D, ID[x]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID[x]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}

void Material(GLfloat rCol, GLfloat gCol, GLfloat bCol)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { rCol*0.5, gCol*0.5, bCol*0.5, 1.0 };
    GLfloat mat_diffuse[] = { rCol, gCol, bCol, 1.0 };
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_shininess[] = { 100 };

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
}


void new_Cube()
{
    glBegin(GL_QUADS);

    GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(Cube[Cube_indeces[i][0]][0], Cube[Cube_indeces[i][0]][1], Cube[Cube_indeces[i][0]][2],
                    Cube[Cube_indeces[i][1]][0], Cube[Cube_indeces[i][1]][1], Cube[Cube_indeces[i][1]][2],
                    Cube[Cube_indeces[i][2]][0], Cube[Cube_indeces[i][2]][1], Cube[Cube_indeces[i][2]][2]);

        glVertex3fv(&Cube[Cube_indeces[i][0]][0]);
        glTexCoord2f(0,1);
        glVertex3fv(&Cube[Cube_indeces[i][1]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&Cube[Cube_indeces[i][2]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&Cube[Cube_indeces[i][3]][0]);
        glTexCoord2f(0,0);


    }

    glEnd();
}



static GLfloat p_vertex[5][3] =
{
    {0,0,0},
    {1,0,0},
    {0,0,1},
    {1,0,1},
    {0.5,1,0.5}
};


static GLubyte p_quadIndices[1][4] =
{
    {1,5,4,0}
};

static GLubyte p_triangleIndices[4][3] =
{
    {0,4,1}, //front
    {1,4,3}, //right
    {2,3,4}, //back
    {0,2,4}  //left
};

void draw_Pyramid()
{
    glBegin(GL_TRIANGLES);
    for (GLint i = 0; i <4; i++)
    {
        /*getNormal3p(p_vertex[p_triangleIndices[i][0]][0], p_vertex[p_triangleIndices[i][0]][1], p_vertex[p_triangleIndices[i][0]][2],
                    p_vertex[p_triangleIndices[i][1]][0], p_vertex[p_triangleIndices[i][1]][1], p_vertex[p_triangleIndices[i][1]][2],
                    p_vertex[p_triangleIndices[i][2]][0], p_vertex[p_triangleIndices[i][2]][1], p_vertex[p_triangleIndices[i][2]][2]);*/

        glVertex3fv(&p_vertex[p_triangleIndices[i][0]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&p_vertex[p_triangleIndices[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&p_vertex[p_triangleIndices[i][2]][0]);
        glTexCoord2f(1,1);
    }
    glEnd();

    glBegin(GL_QUADS);
    for (GLint i = 0; i <1; i++)
    {
        getNormal3p(p_vertex[p_quadIndices[i][0]][0], p_vertex[p_quadIndices[i][0]][1], p_vertex[p_quadIndices[i][0]][2],
                    p_vertex[p_quadIndices[i][1]][0], p_vertex[p_quadIndices[i][1]][1], p_vertex[p_quadIndices[i][1]][2],
                    p_vertex[p_quadIndices[i][2]][0], p_vertex[p_quadIndices[i][2]][1], p_vertex[p_quadIndices[i][2]][2]);

        glVertex3fv(&p_vertex[p_quadIndices[i][0]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&p_vertex[p_quadIndices[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&p_vertex[p_quadIndices[i][2]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&p_vertex[p_quadIndices[i][3]][0]);
        glTexCoord2f(0,1);
    }
    glEnd();
}







void Light_One()
{
    glPushMatrix();
    GLfloat light_position[] = { 4, 20, 10, 1.0 };
    glLightfv( GL_LIGHT0, GL_POSITION, light_position);
    glPopMatrix();
}

void Light_Two()
{
    glPushMatrix();
    GLfloat light_position[] = { 5.0, 30.0, 10.0, 1.0 };
    glLightfv( GL_LIGHT1, GL_POSITION, light_position);
    glPopMatrix();
}

void Light_Three()
{
    glPushMatrix();
    GLfloat light_position[] = { 5,10,0, 1.0 };
    glLightfv( GL_LIGHT2, GL_POSITION, light_position);
    GLfloat spot_direction[] = { 3, -3, 1 };
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, 25.0);
    glPopMatrix();
}



void draw_park_land()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    glPushMatrix();
    glScalef(20,0.3,20);
    glTranslatef(-0.2,-10,-0.5);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    glPushMatrix();
    glScalef(20,0.3,2);
    glTranslatef(-0.2,-10,-7.5);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

void draw_park_river()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glScalef(20,0.3,3);
    glTranslatef(-0.2,-10,-4.33);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(20,0.3,2);
    glTranslatef(-0.2,-10,5);
    new_Cube();
    //LoadTexture("F:\\kuet\\4th year\\4.2\\CSE 4108\\Project_1707051\\water.bmp",2);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}


void draw_park_sky()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex3f(-1000,475,-400);
    glTexCoord2f(1, 1);
    glVertex3f(-1000,475,1100);
    glTexCoord2f(1, 0);
    glVertex3f(1000,475,1100);
    glTexCoord2f(0, 0);
    glVertex3f(1000,475,-400);
    glEnd();

    //right anticlockwise
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex3f(1000,475,-400);
    glTexCoord2f(1, 1);
    glVertex3f(1000,475,1100);
    glTexCoord2f(1, 0);
    glVertex3f(1000,-125,1100);
    glTexCoord2f(0, 0);
    glVertex3f(1000,-125,-400);
    glEnd();

    //left clockwise
    glBegin(GL_QUADS);
    glTexCoord2f(1, 0);
    glVertex3f(-1000,-125,-400);
    glTexCoord2f(0, 0);
    glVertex3f(-1000,-125,1100);
    glTexCoord2f(0, 1);
    glVertex3f(-1000,475,1100);
    glTexCoord2f(1, 1);
    glVertex3f(-1000,475,-400);
    glEnd();

    //back clockwise
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex3f(-1000,475,-400);
    glTexCoord2f(1, 1);
    glVertex3f(1000,475,-400);
    glTexCoord2f(1, 0);
    glVertex3f(1000,-125,-400);
    glTexCoord2f(0, 0);
    glVertex3f(-1000,-125,-400);
    glEnd();

    //front anticlockwise
    glBegin(GL_QUADS);
    glTexCoord2f(1, 0);
    glVertex3f(-1000,-125,1100);
    glTexCoord2f(0, 0);
    glVertex3f(1000,-125,1100);
    glTexCoord2f(0, 1);
    glVertex3f(1000,475,1100);
    glTexCoord2f(1, 1);
    glVertex3f(-1000,475,1100);
    glEnd();

    //down clockwise
    /*glBegin(GL_QUADS);
    glTexCoord2f(1, 0); glVertex3f(1000,-125,-400);
    glTexCoord2f(0, 0); glVertex3f(1000,-125,1100);
    glTexCoord2f(0, 1); glVertex3f(-1000,-125,1100);
    glTexCoord2f(1, 1); glVertex3f(-1000,-125,-400);
    glEnd();*/
    glDisable(GL_TEXTURE_2D);
}


void draw_park_tree()
{
    GLUquadricObj *quad;
    quad = gluNewQuadric();
    //glScalef(0.3,0.5,0.3);
    glPushMatrix();
    Material(0.2,0.1,0.1);
    glRotatef(90,1,0,0);
    gluCylinder(quad, 0.5, 0.5, 5, 10, 10);
    glPopMatrix();

    float tx = 0, ty = 0, tz = 0, sx = 1, sy = 1, sz = 1;
    for(int i=0; i<10; i++)
    {
        glPushMatrix();
        glTranslatef(tx,ty,tz);
        Material(0, 0.5,0);
        glRotatef(-90,1,0,0);
        glScalef(sx, sy, sz);
        glutSolidCone(3-i,3,10,10);
        glPopMatrix();
        ty += 1.5;// sx -= 0.08;
    }
}

void tree()

{

    int tx=0;
    for(int i=0; i<6; i++)
    {
        glPushMatrix();
        glScalef(0.3,0.3,0.3);
        glTranslatef(-10+tx,-3.5,-45);
        draw_park_tree();
        glPopMatrix();
        tx+=5;
    }

    tx=0;
    for(int i=0; i<6; i++)
    {
        glPushMatrix();
        glScalef(0.3,0.3,0.3);
        glTranslatef(22+tx,-3.5,-45);
        draw_park_tree();
        glPopMatrix();
        tx+=5;
    }

    tx=0;
    for(int i=0; i<13; i++)
    {
        glPushMatrix();
        glScalef(0.3,0.3,0.3);
        glTranslatef(50,-3.5,-28+tx);
        draw_park_tree();
        glPopMatrix();
        tx+=5;
    }

    tx=0;
    for(int i=0; i<13; i++)
    {
        glPushMatrix();
        glScalef(0.3,0.3,0.3);
        glTranslatef(-10,-3.5,-28+tx);
        draw_park_tree();
        glPopMatrix();
        tx+=5;
    }

}

void draw_road()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glPushMatrix();
    glScalef(1,0.2,20);
    glTranslatef(5,-14,-0.5);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(20,0.2,1);
    glTranslatef(-0.2,-13,-0.7);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(1.5,0.2,3);
    glTranslatef(3.2,-14,3.2);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(20,0.3,2);
    glTranslatef(-0.2,-9,6);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void draw_gate()
{
    glPushMatrix();
    GLUquadricObj *quad;
    quad = gluNewQuadric();
    glRotatef(90,1,0,0);
    Material(0,0,0);
    gluCylinder(quad, 0.5, 0.5, 3, 10, 10);
    glPopMatrix();
}

void gate()
{
    glPushMatrix();
    glScalef(1,2,1);
    glTranslatef(3,1.5,13);
    draw_gate();
    glPopMatrix();

    glPushMatrix();
    glScalef(1,2,1);
    glTranslatef(7,1.5,13);
    draw_gate();
    glPopMatrix();
}

void draw_wall()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);

    glPushMatrix();
    glScalef(6,6,0.5);
    glTranslatef(-0.5,-0.5,25);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(6,6,0.5);
    glTranslatef(1.2,-0.5,25);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

GLfloat radius=2;

void drawWheel()
{
    //Torus
    glPushMatrix();
    Material(0.604, 0.804, 0.196);
    glutSolidTorus(0.1,radius,10,10);
    glPopMatrix();

    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    //line
    for (int theta = 0; theta <= 360; theta += 36)
    {
        glPushMatrix();
        Material(0.420, 0.557, 0.137);
        glRotatef(theta,0,0,1);
        glRotatef(90,0,1,0);
        gluCylinder(quadratic, 0.1, 0.1, radius, 10, 10);
        glPopMatrix();
    }

}

void wheelSeat()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    //flat disk
    glPushMatrix();
    Material(0, 0.5, 0.8);
    //glTranslatef(0,-10,0);
    glRotatef(90,1,0,0);
    gluDisk(quadratic,0,0.3,32,32);
    glPopMatrix();
    //basket
    glPushMatrix();
    Material(0, 0.5, 0.5);
    //glTranslatef(0,-10,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic, 0.3,0.3, 0.5, 32, 32);
    glPopMatrix();
    //seat head
    glPushMatrix();
    Material(0.416, 0.353, 0.804);
    glTranslatef(0,1,0);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic, 0.05,0.3, 0.3, 32, 32);
    glPopMatrix();
    //attached line
    glPushMatrix();
    Material(0.8,0.8,0.8);
    glTranslatef(0,1.5,0);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic, 0.05,0.05, 1.5, 32, 32);
    glPopMatrix();
}

void wheelStand()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glScalef(2,3,2);
    Material(1,1,1);
    glTranslatef(-0.5,-1,-0.5);
    draw_Pyramid();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void draw_wheel()
{
    glPushMatrix();
    glTranslatef(10,0,-7);
    glPushMatrix();
    glRotatef(angle,0,1,0);
    glPushMatrix();
    glRotatef(90,1,0,0);
    drawWheel();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-1.5,2);
    wheelSeat();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-1.5,-2);
    wheelSeat();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2,-1.5,0);
    wheelSeat();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2,-1.5,0);
    wheelSeat();
    glPopMatrix();
    glPopMatrix();

    wheelStand();
    glPopMatrix();
}

int angle2=0,d=0,dir=1;

void dolna()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    //Material(0.5,0.5,0.5);
    glScalef(0.3,5,0.3);
    glTranslatef(0,-0.6,0);
    new_Cube();
    glPopMatrix();


    glPushMatrix();
    //Material(0.5,0.5,0.5);
    glScalef(0.3,5,0.3);
    glTranslatef(10,-0.6,0);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    //Material(0.5,0.5,0.5);
    glScalef(3.3,0.3,0.3);
    glTranslatef(0,6,0);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glPushMatrix();
    glRotatef(angle2,1,0,0);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glScalef(2,0.3,1);
    glTranslatef(0.3,-4,0);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glPushMatrix();
    //Material(0.5,0.5,0.5);
    glScalef(0.1,3,0.1);
    glTranslatef(5.2,-.35,0);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    //Material(0.5,0.5,0.5);
    glScalef(0.1,3,0.1);
    glTranslatef(25,-.35,0);
    new_Cube();
    glPopMatrix();
    glPopMatrix();
}

void draw_dolna()
{
    glPushMatrix();
    glTranslatef(-3,0,-17);
    dolna();
    glPopMatrix();
}

GLfloat angle3=0,s=0,ts=30,sd=1;

void draw_ride1()
{
    //stand
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    Material(1,1,1);
    glScalef(1,2,2);
    glTranslatef(0,-1.5,-0.25);
    draw_Pyramid();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //rotating part
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    Material(1,1,1);
    glPushMatrix();
    glRotatef(angle3, 0,0,1);
    //seat
    glPushMatrix();
    glScalef(4,0.3,1);
    glTranslatef(-0.35,-4,0);
    new_Cube();
    glPopMatrix();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void stand()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,7);
    glPushMatrix();
    glScalef(0.5,9,0.5);
    glTranslatef(0,-0.33,0);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.5,9,0.5);
    glTranslatef(6,-0.33,0);
    new_Cube();
    glPopMatrix();



    int ty=0;
    for(int i=0; i<11; i++)
    {
        glPushMatrix();
        glScalef(3.5,0.3,0.3);
        glTranslatef(0,-10+ty,0);
        new_Cube();
        glPopMatrix();
        ty+=3;
    }
    glDisable(GL_TEXTURE_2D);

}

GLfloat ty=0,up_speed=1,u=0;
GLint ucon=0;

void seat()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);

    glPushMatrix();
    glScalef(3.5,0.2,1.5);
    glTranslatef(0,-12,0.3);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.5,1,1.5);
    glTranslatef(0,-2.5,0.3);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.5,1,1.5);
    glTranslatef(6,-2.5,0.3);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(3.5,1,0.2);
    glTranslatef(-0,-2.5,2 );
    new_Cube();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

}


void draw_ride2()
{
    glPushMatrix();
    glTranslatef(1.4,0,-18);
    glPushMatrix();
    stand();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,ty,0);
    seat();
    glPopMatrix();
    glPopMatrix();
}


void plot()
{
    glPushMatrix();
    glScalef(2,0.2,2);
    glTranslatef(-1,3,0.5);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,3.5,0.2);
    glTranslatef(-1,-.8,5);
    new_Cube();
    glPopMatrix();


    glPushMatrix();
    glScalef(0.2,3.5,0.2);
    glTranslatef(-10,-.8,5);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,3.5,0.2);
    glTranslatef(-10,-.8,14);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,3.5,0.2);
    glTranslatef(-1,-.8,14);
    new_Cube();
    glPopMatrix();
}


void move_stand()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glScalef(0.2,6,0.2);
    glTranslatef(-20,-0.5,-5);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glScalef(0.2,6,0.2);
    glTranslatef(-20,-0.5,-90);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,9);
    glPushMatrix();
    glScalef(0.2,0.2,17);
    glTranslatef(-20,15,-1.05);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void move_seat()
{
    glPushMatrix();
    //glScalef(0.5,0.5,0.5);
    glTranslatef(-4,-2.5,-17);
    wheelSeat();
   glPopMatrix();
}

GLfloat tz1=0,ty1=0,k1=0,upcon=0,k2=0,scon=0;

void draw_ride3()
{
    glPushMatrix();
    glTranslatef(0,ty1,tz1);
    move_seat();
    glPopMatrix();
    move_stand();
}

void disk()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    //flat disk
    glPushMatrix();
    Material(1, 1, 1);
    glTranslatef(0,-2.5,0);
    glRotatef(90,1,0,0);
    //glTranslatef(0,-6,0);
    gluDisk(quadratic,0,2,32,32);
    glPopMatrix();
}

void chair()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    //flat disk
    glPushMatrix();
    Material(0, 0.5, 0.8);
    //glTranslatef(0,-10,0);
    glRotatef(90,1,0,0);
    gluDisk(quadratic,0,0.3,32,32);
    glPopMatrix();
    //basket
    glPushMatrix();
    Material(0, 0.5, 0.5);
    //glTranslatef(0,-10,0);
    glRotatef(-90,1,0,0);
    gluCylinder(quadratic, 0.3,0.3, 0.5, 32, 32);
    glPopMatrix();
}

void draw_chair()
{
    glPushMatrix();
    glTranslatef(0,-2.5,-1.5);
    chair();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-2.5,1.5);
    chair();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5,-2.5,0);
    chair();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5,-2.5,0);
    chair();
    glPopMatrix();

}

void head()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    glPushMatrix();
    Material(0.416, 0.353, 0.804);
    glTranslatef(0,2,0);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic, 0.05,0.3, 5, 32, 32);
    glPopMatrix();
    //attached line
    glPushMatrix();
    Material(0.8,0.8,0.8);
    glTranslatef(0,3,0);
    glRotatef(90,1,0,0);
    gluCylinder(quadratic, 0.01,3, 3, 32, 32);
    glPopMatrix();
}

GLfloat angle4=0,r4=0;

void draw_ride4()
{

    glPushMatrix();
    glTranslatef(9,0,-15);
    glPushMatrix();
    glRotatef(angle4,0,1,0);
    disk();
    draw_chair();
    glPopMatrix();
    head();
    glPopMatrix();
}


void wheel5()
{
    glPushMatrix();
    //glTranslatef(10,0,-15);
    //glPushMatrix();
    glScalef(1,1,1);
    glTranslatef(0.2,1,0);
    glRotatef(angle,0,1,0);

   // glRotatef(90,1,0,0);
    drawWheel();
    glPopMatrix();
}

void stand5()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glScalef(0.2,4,0.2);
    glTranslatef(0,-0.75,0);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glScalef(0.2,4,0.2);
    glTranslatef(0,-0.75,-10);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glScalef(0.2,0.2,2);
    glTranslatef(0,4,-1);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

GLfloat r5=0,angle5=0;

void draw_ride5()
{
    glPushMatrix();
    glPushMatrix();
    glScalef(1,1,1);
    glTranslatef(0.2,1,-1);
    glRotatef(-angle5,0,0,1);

    //glRotatef(angle,0,1,0);
    drawWheel();
    glPopMatrix();
    stand5();
    glPopMatrix();
}


void table()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    glPushMatrix();
    glScalef(3,0.2,3);
    glTranslatef(0,-5,0);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,2,0.2);
    glTranslatef(0,-1.5,0);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,2,0.2);
    glTranslatef(0,-1.5,14);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,2,0.2);
    glTranslatef(14,-1.5,0);
    new_Cube();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,2,0.2);
    glTranslatef(14,-1.5,14);
    new_Cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

void table_seat()
{

    glPushMatrix();
    glTranslatef(1,0,0);
    head();
    glPopMatrix();
}

void draw_table()
{
    glPushMatrix();
    glTranslatef(-2,0,-7);
    table();
    table_seat();
    glPopMatrix();
}





int eye[3]= {2,3,13};
int re[3]= {4,0,0};
int up[3]= {0,1,0};
int k=-5,c1=0,c2=0;
int rr =10;

void changeSize(int w, int h)
{
    if (h == 0)
        h = 1;
    float r = (GLfloat)w / (GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);
    gluPerspective(60, r, 1, 200);
    glMatrixMode(GL_MODELVIEW);
}
//60,1,1,10000

GLfloat zoomx=0,zoomz=0,zoomy=0;
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //glFrustum(-5, 5, k, 5, 4, 50);
    gluPerspective(60,1,1,10000);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(eye[0],eye[1],eye[2],re[0],re[1],re[2],up[0],up[1],up[2]);

    //glViewport(0, 0, windowHeight, windowWidth);

    glPushMatrix();
    glTranslatef(zoomx,zoomy,zoomz);

    glPushMatrix();
    glTranslatef(-2,0,-10);
    glPushMatrix();
    draw_park_land();
    draw_park_river();
    draw_park_sky();
    tree();
    gate();

    draw_road();
    draw_wall();

    glPopMatrix();
    glPopMatrix();

    draw_wheel();

    draw_dolna();

    glPushMatrix();
    glTranslatef(5,0,-4);
    glRotatef(90,0,1,0);
    draw_ride1();
    glPopMatrix();

    draw_ride2();

    draw_ride3();

    draw_ride4();

    glPushMatrix();
    glTranslatef(10,0,-1);
    //glRotatef(90,0,1,0);
    draw_ride5();
    glPopMatrix();

    draw_table();

    glPopMatrix();



    glFlush();
    glutSwapBuffers();

    glEnable(GL_LIGHTING);

    Light_One();
    Light_Two();
    Light_Three();
    //glDisable(GL_LIGHTING);
}
int r=0;

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {

    case 'f':
        //f = 1;
        break;

    case '1':
        if(l1==0)
        {
            l1=1;
            a1=1;
            s1=1;
            d1=1;
            glEnable(GL_LIGHT0);
        }
        else
        {
            l1=0;
            a1=0;
            s1=0;
            d1=0;
            glDisable(GL_LIGHT0);
        }
        break;

    case '2':
        if(l2==0)
        {
            l2=1;
            a2=1;
            s2=1;
            d2=1;
            glEnable(GL_LIGHT1);
        }
        else
        {
            l2=0;
            a2=0;
            s2=0;
            d2=0;
            glDisable(GL_LIGHT1);
        }
        break;

    case '3':
        if(l3==0)
        {
            l3=1;
            a3=1;
            s3=1;
            d3=1;
            glEnable(GL_LIGHT2);
        }
        else
        {
            l3=0;
            a3=0;
            s3=0;
            d3=0;
            glDisable(GL_LIGHT2);
        }
        break;
    case 'a':
        if(a1==0 || a2==0 || a3==0)
        {
            a1=1;
            a2=1;
            a3=1;
        }
        else
        {
            a1=0;
            a2=0;
            a3=0;
        }
        break;

    case 's':
        if(s1==0 || s2==0 || s3==0)
        {
            s1=1;
            s2=1;
            s3=1;
        }
        else
        {
            s1=0;
            s2=0;
            s3=0;
        }
        break;

    case 'd':
        if(d1==0 || d2==0 || d3==0)
        {
            d1=1;
            d2=1;
            d3=1;
        }
        else
        {
            d1=0;
            d2=0;
            d3=0;
        }
        break;

    case 'i':
        //eye[2]-=2;
        zoomz++;
        break;

    case 'o':
        //eye[2]+=2;
        zoomz--;
        break;

    case 'y':
        //eye[2]-=2;
        zoomx++;
        break;

    case 'u':
        //eye[2]+=2;
        zoomx--;
        break;

    case 'j':
        //eye[2]-=2;
        zoomy++;
        break;

    case 'k':
        //eye[2]+=2;
        zoomy--;
        break;


    case 'm':
        eye[1]--;
        break;
    case 'n':
        eye[1]++;
        break;
    case 'p':
        re[0]--;
        break;

    case 'q':
        re[0]++;
        break;
    case 'x':
        re[1]--;
        break;

    case 'z':
        re[1]++;
        break;
    case 'w':
        if(wheel==0)
            wheel=1;
        else
            wheel=0;
        break;
    case '0':
        if(d==0)
            d=1;
        else
            d=0;
        break;
    case '9':
        if(s==0)
            s=1;
        else
            s=0;
        break;

    case '8':
        if(u==0)
            u=1;
        else
            u=0;
        break;

    case '7':
        if(k1==0)
            k1=1;
        else
            k1=0;
        break;

    case '6':
        if(k2==0)
            k2=1;
        else
            k2=0;
        break;

    case 'e':
        if(r4==0)
            r4=1;
        else
            r4=0;
        break;

    case '4':
        if(r5==0)
            r5=1;
        else
            r5=0;
        break;

    case 27:	// Escape key
        exit(1);
    }
}

int speed=1,tangle=30;


void animate()
{
    if (bRotate == true)
    {
        theta += 0.2;
        if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0);
    }

    if (uRotate == true)
    {
        alpha += 0.2;
        if(alpha > 360.0)
            alpha -= 360.0*floor(alpha/360.0);
    }

    if(wheel==1)
    {
        angle += speed;
        if(angle > 360)
            angle -=360;
    }

    if(d == 1)
    {
//        if(tangle == 0) {d = 0;}
//        if(angle2 >= tangle && dir == 1) { tangle--; dir = -1; speed *= 0.90;}
//        if(angle2 <= -tangle && dir == -1) { tangle--; dir = 1; speed*= 0.90;}
//        angle2 += dir * 1;

        if(angle2 >= 20 || angle2 <= -20 )
        {
            dir *= -1;
        }
        angle2 += dir * 0.01;
    }


    if(s == 1)
    {
        if(angle3 >= 20 || angle3 <= -20 )
        {
            sd *= -1;
        }
        angle3 += sd * 0.1;
    }

    if(u==1)
    {
        if(ucon == 0){
            ty+=0.01;
            if(ty>= 8) ucon = 1;
        }

        else{
            ty-=0.01;
            if(ty<= 0) ucon = 0;
        }

    }


    if(k1==1)
    {
        if(upcon==0)
        {
            ty1+=0.01;
            if(ty1>=4)
            {
                upcon=1;
                k1=0;
            }
        }
        else
        {
            ty1-=0.01;
            if(ty1<=0)
            {
                upcon=0;
                k1=0;
            }
        }
    }


    if(k2==1)
    {
        if(scon==0)
        {
            tz1+=0.01;
            if(tz1>=15)
            {
                scon=1;
            }
        }
        else
        {
            tz1-=0.01;
            if(tz1<=0)
            {
                scon=0;
                k2=0;
            }
        }
    }

    if(r4==1)
    {
        angle4 += 1;
        if(angle4 > 360)
            angle4 -=360;
    }
    if(r5==1)
    {
        angle5 += 1;
        if(angle5 > 360)
            angle5 -=360;
    }


//For Light1
    if(a1 == 1)
    {
        glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient1);
    }
    else if(a1 == 0)
    {
        glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);
    }
    if(d1 == 1)
    {
        glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse1);
    }
    else if(d1 == 0)
    {
        glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);
    }
    if(s1 == 1)
    {
        glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    }
    else if(s1 == 0)
    {
        glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);
    }

//For Light2
    if(a2 == 1)
    {
        glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient2);
    }
    else if(a2 == 0)
    {
        glLightfv( GL_LIGHT1, GL_AMBIENT, no_light);
    }
    if(d2 == 1)
    {
        glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
    }
    else if(d2 == 0)
    {
        glLightfv( GL_LIGHT1, GL_DIFFUSE, no_light);
    }
    if(s2 == 1)
    {
        glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular);
    }
    else if(s2 == 0)
    {
        glLightfv( GL_LIGHT1, GL_SPECULAR, no_light);
    }

//For Spotlight
    if(a3 == 1)
    {
        glLightfv( GL_LIGHT2, GL_AMBIENT, light_ambient3);
    }
    else if(a3 == 0)
    {
        glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);
    }
    if(d3 == 1)
    {
        glLightfv( GL_LIGHT2, GL_DIFFUSE, light_diffuse3);
    }
    else if(d3 == 0)
    {
        glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);
    }
    if(s3 == 1)
    {
        glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular);
    }
    else if(s3 == 0)
    {
        glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);
    }


    glutPostRedisplay();

}


int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(100,100);
    glutInitWindowSize(windowHeight, windowWidth);
    glutCreateWindow("Traingle-Demo");


    LoadTexture("F:\\kuet\\4th year\\4.2\\CSE 4108\\Project_1707051\\grass3.bmp",1);
    LoadTexture("F:\\kuet\\4th year\\4.2\\CSE 4108\\Project_1707051\\water.bmp",2);
    LoadTexture("F:\\kuet\\4th year\\4.2\\CSE 4108\\Project_1707051\\skyup.bmp",3);
    LoadTexture("F:\\kuet\\4th year\\4.2\\CSE 4108\\Project_1707051\\road.bmp",4);
    LoadTexture("F:\\kuet\\4th year\\4.2\\CSE 4108\\Project_1707051\\brick.bmp",5);
    LoadTexture("F:\\kuet\\4th year\\4.2\\CSE 4108\\Project_1707051\\wood.bmp",6);
    LoadTexture("F:\\kuet\\4th year\\4.2\\CSE 4108\\Project_1707051\\steel.bmp",7);
    LoadTexture("F:\\kuet\\4th year\\4.2\\CSE 4108\\Project_1707051\\steel2.bmp",8);
    LoadTexture("F:\\kuet\\4th year\\4.2\\CSE 4108\\Project_1707051\\rope.bmp",9);



    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    glutIdleFunc(animate);
    glutMainLoop();

    return 0;
}

