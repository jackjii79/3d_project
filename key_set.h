#ifndef KEY_SET_H
#define KEY_SET_H

#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include<stdint.h>
#include"model.h"
#include"quat.h"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#define MAX_BONECOUNT 80

typedef struct keyframe{
	double length;
									
	point begin;   //starting point of bone (should be preconputed)

	/*angles of rotation*/
	double anglex;
	double angley;
	double anglez;
	quat q;
	
	uint32_t time; //time that this keyframe happens
} keyframe;

typedef struct bone
 {
 	std::string name;				/* name of bone*/
	int id;                  //used for picking
	point begin;				//where the bone starts
	point start;              //location of bone relative to mesh
	double length;			   //length of bone
									/*angles of rotation*/
	double anglex;
	double angley;
	double anglez;

	quat o; //original quat

	//current angles 
	double curx;
	double cury;
	double curz;

	bool anim;

	double dist;         //the distance that the bone has influence over mesh points

	quat q;              //current quaternion for making smooth animations
	bool done;				//used for traversing the skeleton
 
	std::vector<struct point *>  points; /* points that the bone is connected to*/
	std::vector<int>  orig_points; /* the indexes of the original points values */
 	std::vector<struct bone *> child;	/* Pointers to children */
 	struct bone *parent;			/* Parent bone */
	std::vector<struct keyframe *> keyframes; //the keyframes of the bone
 } bone;

class key_set 
{
public:
	key_set(model *);
	~key_set();
	void draw_skeleton();//draw the tree recursively
	void change_bone();
	bone * add_bone(bone * parent, point begin, double length, double anglex,
		double angley, double anglez, std::string name);//add child bone to parent if parent is null then create a new root without having child
	void load_skeleton(const char * title);//load bone(include keyframes) from file
	void draw_keyframe();  
	void next_frame();	
	void animate(int t, bool draw);
	void set_cur_to_keyframe();
        void BoneListNames(bone *root,char names[MAX_BONECOUNT][20]);//store all bones in the order of travesal tree into array names
        void pre_compute_quat();

	//fields
	bone * cur;//current position
	bone * skeleton;//root point
	std::vector<uint32_t> keyframes; //array of time values for each keyframe

private:
        void pre_compute_quat_rec(quat r, point begin, bone * root, bool keyframe, int k);
	void traverse_skeleton(point begin, bone * root);
	bone * BoneFindByName(bone * root, std::string name);//return selected bone according to bone name
	bone * get_next_child(bone * cur);
	void animate_rec(quat last, point begin, bone * root, int prev, int next, double u, bool draw);
	void set_cur_to_keyframe_rec(bone * root);
        void move_points_anim(bone * root, quat & q, point & begin);

	//fields
	model * skin;//dragon mesh model
	int id_count;//sum of points including 3d mesh and skeleton vertex
	std::vector<point> orig_skin; //vector array of the original points
											//(used when morphing the skin)
	int cframe; //tells which keyframe to draw if using draw_keyframe();
	uint32_t frame_time; //keeps track of the time for each keyframe that is saved
};

#endif
