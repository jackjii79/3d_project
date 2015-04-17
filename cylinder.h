#ifndef _CYLINDER_
#define _CYLINDER_

#include "renderable.h"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Cylinder : public Renderable
{
public:
	void draw();

private:
	void drawBody();
        void drawHead();
	//void drawElements();
	//void drawArrays();
};

#endif

