#include <iostream>
using namespace std;
#include "dragon.h"

/*
void Dragon::draw()
{
        model *dragon = new model((const char *)"dragon.obj");
        glPushMatrix();
        dragon->draw_model(false,false);
        glPopMatrix();
}
*/

void Dragon::draw() 
{
        static int timer;
        model *dragon = new model((const char *)"dragon.obj");
        key_set *frame = new key_set(dragon);
        frame->load_skeleton("skeleton");
        frame->pre_compute_quat();
        glPushMatrix();
        frame->animate(timer,false);
        timer += 50;
        dragon->draw_model(false,false);
        glPopMatrix();
}
