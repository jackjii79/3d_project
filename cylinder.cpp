#ifndef _CYLINDER_CPP_
#define _CYLINDER_CPP_
#include <iostream>
using namespace std;
#include "cylinder.h"
#include "math.h"
GLint num_vertex = 40;
GLint num_vertex_head = 20;
#define PI 3.1416

/*
          Z
          |
    7 +-------+ 6
     /|      /|
    / |     / |   ---> Y
 4 +-------+5 |
   | 3+----|--+ 2
   | /     | /
   |/      |/
 0 +-------+ 1
      /
     X         
*/


void Cylinder::draw()
{
	glPushMatrix();

        glTranslatef(0.0,0.0,0.0);
        glRotatef(180.0,1.0,0.0,0.0);
        glScalef(0.5,0.5,1.0);

        glPushMatrix();
	// draw body (center cube)
	drawBody();

	glPopMatrix();
	glPushMatrix();
        glTranslatef(-0.25,0.5,1.2);
        glRotatef(150,1.0,0.0,0.0);
        glScalef(0.3,0.25,0.5);
        //glTranslatef(0.5,0.0,0.0);
        drawHead();
        glPopMatrix();
}

//==================== 1. Immediate method ===================================
// immediate definition of individual vertex properties

void Cylinder::drawBody()
{
        GLfloat vertex1[4];
        GLfloat vertex2[4];
        GLfloat vertex3[3];
        GLfloat vertex4[3];
        //GLfloat normal[3]; used for normal of each polygon

        const GLfloat delta_angle = 2.0 * PI/num_vertex;
        glEnable(GL_NORMALIZE);

        glBegin(GL_TRIANGLE_FAN);

        for(int i = 0;i < num_vertex;i++)
        {
            vertex1[0] = cos(delta_angle*i);
            vertex1[1] = sin(delta_angle*i);
            vertex1[2] = 1.0;
            vertex1[3] = 1.0;
            glNormal3f(vertex1[0],vertex1[1],vertex1[2]+0.5);
            glVertex4fv(vertex1);
        }

        glEnd();

        for(int i = 0;i < num_vertex;i++)
        {
            glBegin(GL_POLYGON);

            vertex3[0] = cos(delta_angle*(i%num_vertex));
            vertex3[1] = sin(delta_angle*(i%num_vertex));
            vertex3[2] = 1.0;
            vertex4[0] = cos(delta_angle*((i+1)%num_vertex));
            vertex4[1] = sin(delta_angle*((i+1)%num_vertex));
            vertex4[2] = 1.0;

/* normal to the whole polygon
            if (vertex3[0] < vertex4[0])
             normal[0] = vertex3[0]*2 + (vertex4[0]*2 - vertex3[0]*2) / 2;
            else
             normal[0] = vertex4[0]*2 + (vertex3[0]*2 - vertex4[0]*2) / 2;
  
            if (vertex3[1] < vertex4[1])
             normal[1] = vertex3[1]*2 + (vertex4[1]*2 - vertex3[1]*2) / 2;
            else
             normal[1] = vertex4[1]*2 + (vertex3[1]*2 - vertex4[0]*2) / 2;

            normal[2] = 0.0;
            glNormal3fv(normal);
*/
            glNormal3fv(vertex3);
            glVertex3fv(vertex3);
            vertex3[2] = - 1.0;
            glNormal3fv(vertex3);
            glVertex3fv(vertex3);

            glNormal3fv(vertex4);
            glVertex3fv(vertex4);
            vertex4[2] = - 1.0;
            glNormal3fv(vertex4);
            glVertex3fv(vertex4);

            glEnd();
        }

        glBegin(GL_TRIANGLE_FAN);

        for(int i = 0;i < num_vertex;i++)
        {
            vertex2[0] = cos(delta_angle*i);
            vertex2[1] = sin(delta_angle*i);
            vertex2[2] = - 1.0;
            vertex2[3] = 1.0;
            glNormal3f(vertex2[0],vertex2[1],vertex2[2] - 0.5);
            glVertex4fv(vertex2);
        }
        glEnd();

        for(int i = 0;i < num_vertex;i++)
        {
            glBegin(GL_POLYGON);

            vertex3[0] = cos(delta_angle*(i%num_vertex));
            vertex3[1] = sin(delta_angle*(i%num_vertex));
            vertex3[2] = - 1.0;
            vertex4[0] = cos(delta_angle*((i+1)%num_vertex));
            vertex4[1] = sin(delta_angle*((i+1)%num_vertex));
            vertex4[2] = - 1.0;

/* normal to the whole polygon
            if (vertex3[0] < vertex4[0])
             normal[0] = vertex3[0]*2 + (vertex4[0]*2 - vertex3[0]*2) / 2;
            else
             normal[0] = vertex4[0]*2 + (vertex3[0]*2 - vertex4[0]*2) / 2;
  
            if (vertex3[1] < vertex4[1])
             normal[1] = vertex3[1]*2 + (vertex4[1]*2 - vertex3[1]*2) / 2;
            else
             normal[1] = vertex4[1]*2 + (vertex3[1]*2 - vertex4[0]*2) / 2;

            normal[2] = 0.0;
            glNormal3fv(normal);
*/

            glNormal3fv(vertex3);
            glVertex3fv(vertex3);
            vertex3[2] = 1.0;
            glNormal3fv(vertex3);
            glVertex3fv(vertex3);

            glNormal3fv(vertex4);
            glVertex3fv(vertex4);
            vertex4[2] = 1.0;
            glNormal3fv(vertex4);
            glVertex3fv(vertex4);
            
            glEnd();
        }
}

void Cylinder::drawHead()
{
        GLfloat vertex1[4];
        GLfloat vertex2[4];
        GLfloat vertex3[3];
        GLfloat vertex4[3];
        //GLfloat normal[3]; used for normal of each polygon

        const GLfloat delta_angle = 2.0 * PI/num_vertex_head;
        glEnable(GL_NORMALIZE);

        glBegin(GL_TRIANGLE_FAN);

        for(int i = 0;i < num_vertex_head;i++)
        {
            vertex1[0] = cos(delta_angle*i);
            vertex1[1] = sin(delta_angle*i);
            vertex3[2] = 1.0;
            vertex1[3] = 1.0;
            glNormal3f(vertex1[0],vertex1[1],vertex1[2]+0.5);
            glVertex4fv(vertex1);
        }

        glEnd();

        for(int i = 0;i < num_vertex_head;i++)
        {
            glBegin(GL_POLYGON);

            vertex3[0] = cos(delta_angle*(i%num_vertex_head));
            vertex3[1] = sin(delta_angle*(i%num_vertex_head));
            vertex3[2] = 1.0;
            vertex4[0] = cos(delta_angle*((i+1)%num_vertex_head));
            vertex4[1] = sin(delta_angle*((i+1)%num_vertex_head));
            vertex4[2] = 1.0;

            glNormal3fv(vertex3);
            glVertex3fv(vertex3);
            vertex3[2] = - 1.0;
            glNormal3fv(vertex3);
            glVertex3fv(vertex3);

            glNormal3fv(vertex4);
            glVertex3fv(vertex4);
            vertex4[2] = - 1.0;
            glNormal3fv(vertex4);
            glVertex3fv(vertex4);

            glEnd();
        }

        glBegin(GL_TRIANGLE_FAN);

        for(int i = 0;i < num_vertex_head;i++)
        {
            vertex2[0] = cos(delta_angle*i);
            vertex2[1] = sin(delta_angle*i);
            vertex2[2] = - 1.0;
            vertex2[3] = 1.0;
            glNormal3f(vertex2[0],vertex2[1],vertex2[2] - 0.5);
            glVertex4fv(vertex2);
        }
        glEnd();

        for(int i = 0;i < num_vertex_head;i++)
        {
            glBegin(GL_POLYGON);

            vertex3[0] = cos(delta_angle*(i%num_vertex_head));
            vertex3[1] = sin(delta_angle*(i%num_vertex_head));
            vertex3[2] = - 1.0;
            vertex4[0] = cos(delta_angle*((i+1)%num_vertex_head));
            vertex4[1] = sin(delta_angle*((i+1)%num_vertex_head));
            vertex4[2] = - 1.0;


            glNormal3fv(vertex3);
            glVertex3fv(vertex3);
            vertex3[2] = 1.0;
            glNormal3fv(vertex3);
            glVertex3fv(vertex3);

            glNormal3fv(vertex4);
            glVertex3fv(vertex4);
            vertex4[2] = 1.0;
            glNormal3fv(vertex4);
            glVertex3fv(vertex4);
            
            glEnd();
        }
}
#endif
