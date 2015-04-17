#ifndef _DRAGON_
#define _DRAGON_

#include <vector>
#include "renderable.h"
#include "key_set.h"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Dragon : public Renderable
{
public:
        void draw();
};

#endif
