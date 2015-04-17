#ifndef _CUBE_
#define _CUBE_

#include <vector>
#include "renderable.h"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Cube : public Renderable
{
public:
	void draw();

private:
	void drawImmediate(bool,bool);
	void drawElements();
	void drawArrays();
};

#endif

