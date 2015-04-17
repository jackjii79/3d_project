#include<vector>
#include<iostream>
#include<fstream>
#include <string.h>
#include<string>
#include <GL/glut.h>
#include <math.h>
#include "key_set.h"
#include "quat.h"
#include "model.h"

#define BONE_DIST .75    //distance from bone that points are influenced by bone's movement

using namespace std;

/*
void key_set::animate() {
 model *dragon = new model((const char *)"dragon.obj");
 key_set *frame = new key_set(dragon);
 frame->load_skeleton("skeleton");
 frame->pre_compute_quat();
 //for(int i = 0;i<=3000;i+=50) {
 glPushMatrix();
 frame->animate(timer,false);
 timer += 50;
 skin->draw_model(false,false);
 glPopMatrix();
 //}
}
*/

key_set::key_set(model * m):cur(NULL), cframe(0), skeleton(NULL), frame_time(0), skin(m)
{
	for(unsigned i = 0;i<m->points.size();i++){
		point * p = new point;
		p->x = m->points[i].x;
		p->y = m->points[i].y;
		p->z = m->points[i].z;
		orig_skin.push_back(*p);
	}
}

//add bone: if parent is NULL, a new skeleton is added to the
//keyframe vector
bone * key_set::add_bone(bone * parent, point begin, double length, double anglex,
		double angley, double anglez, string name){
	bone * t;
	
	//if there is no parent then create a new root
	if(parent == NULL){
		if(!(parent = new bone)) return NULL;
		skeleton = parent;
		parent->parent = NULL;
		cur = parent;
	}else{ 
		if(!(t = new bone)) return NULL;
		t->parent = parent;
		parent->child.push_back(t);
		parent = t;
	}

	parent->begin.x = begin.x;
	parent->begin.y = begin.y;
	parent->begin.z = begin.z;
	parent->anglex = anglex;
	parent->angley = angley;
	parent->anglez = anglez;
	parent->curx = anglex;
	parent->cury = angley;
	parent->curz = anglez;
	parent->length = length;
	parent->anim = false;
	parent->done = true;
	parent->dist = BONE_DIST;
	parent->points.clear();//clear content of vector points
	parent->orig_points.clear();

	parent->name = name;
	
	return parent;
}

void key_set::BoneListNames(bone *root,char names[MAX_BONECOUNT][20]) {
 int i,present;
 if (!root) return;
 //check if this name is already in the list
 present = 0;
 for (i=0;(i < MAX_BONECOUNT) && (names[i][0] != '\0');i++)
  if (! root->name.compare((const char*) names[i])) {
   present = 1;
   break;
  }
 //if not present and if there is space in list
 if (! present && (i < MAX_BONECOUNT)) {
  strcpy(names[i],root->name.c_str());
  //names[i].copy(root->name,root->name.size());
  if(i + 1 < MAX_BONECOUNT)
   names[i+1][0] = '\0';
 }
 //fill the list with subtree's names
 for (i=0;i<root->child.size();i++)
  BoneListNames(root->child[i],names);
}

void key_set::draw_skeleton(){
	//diable lighting for skeleton
	glDisable(GL_LIGHTING);
	//set id's to starting num, skin->points.size() so it doesnt interfer with spheres
	id_count = skin->points.size();//sum of points in model mesh
	traverse_skeleton(skeleton->begin, skeleton);
	glEnable(GL_LIGHTING);
}

void key_set::traverse_skeleton(point begin, bone * root){ //recursive code to draw skeleton
	//calculate end position
	vec end;
	end.x = 0.0;
	end.y = root->length;
	end.z = 0.0;

	end = root->q * end; //do rotation based on quaternions

	end.x += begin.x;
	end.y += begin.y;
	end.z += begin.z;

	if(root == cur) glColor3f(1.0,0.0,0.0);
	else glColor3f(1.0,1.0,1.0);

	root->id = id_count;

	glBegin(GL_LINES);
		glVertex3f(begin.x,begin.y,begin.z);
		glVertex3f(end.x,end.y,end.z);
	glEnd();
	
	glPushMatrix();
		glTranslatef(end.x,end.y,end.z);
		glutSolidSphere(0.1, 5, 5);
	glPopMatrix(); 

	point pend; //convert from vec to point
	pend.x = end.x;
	pend.y = end.y;
	pend.z = end.z;

	id_count++;

	for(unsigned int i=0;i<root->child.size();i++)
		traverse_skeleton(pend, root->child[i]);
}

//recursively move to the next bone in tree order
void key_set::change_bone(){

	if(cur->child.size()>0){
		cur = cur->child[0];
		cur->done = true;
	}else{
		cur = get_next_child(cur);
	}

	//debug:

	if(cur == NULL) cur = skeleton;
	cout << "bone: " << cur->name << endl;
        cout << "x: " << cur->start.x << endl;
        cout << "y: " << cur->start.y << endl;
        cout << "z: " << cur->start.z << endl;
	if (cur->parent == NULL) cout << "	parent = NULL" << endl;
	else cout << "	parent = " << cur->parent->name << endl;
	cout << "	children: " << endl;
	for(unsigned int i=0;i<cur->child.size();i++){
		cout << "	child " << i << " " << cur->child[i]->name << endl;
	}
}

//find neighbor child on the same level which next to cur
bone * key_set::get_next_child(bone * cur){
	if(cur->done == true){
		bone * tmp = cur->parent;
		for(unsigned int i=0;i<tmp->child.size();i++){
			if(tmp->child[i] == cur){
				if(i == tmp->child.size()-1) //if last child
					cur = get_next_child(cur->parent);	
				else{
					cur = tmp->child[i+1];
					cur->done = true;
				}
				break;
			}
		}		
	}
	return cur;
}

bone * key_set::BoneFindByName(bone * root, string name){
	
	if(root == NULL) return NULL;

	if(root->name.compare(name) == 0){ //if names are the same
		return root;
	}else{
		int count = root->child.size();
		bone * tmp;
		for(int i = 0;i<count;i++){
			tmp = BoneFindByName(root->child[i], name);
			if(tmp!=NULL) return tmp;
		}
		return NULL;
	}
}

//loads a skeleton
void key_set::load_skeleton(const char * title){
        double anglex, angley, anglez, len;
        uint32_t time;
	ifstream file (title);
	if (file.is_open()){
   	 while (file.good()){
		string parent;
		bone tmp;
      	        file >> tmp.name;
			file >> tmp.begin.x;
			file >> tmp.begin.y;
			file >> tmp.begin.z;
			file >> tmp.anglex;
			file >> tmp.angley;
			file >> tmp.anglez;
			file >> tmp.length;
			if(!file.good()) break;
			file >> parent;
			tmp.parent = BoneFindByName(skeleton, parent);

			bone * b = add_bone(tmp.parent, tmp.begin, tmp.length, tmp.anglex, tmp.angley, 
				tmp.anglez, tmp.name);

			//now load in bone points
			int p;
			file >> p;
			for(int i=0;i<p;i++){
				int t;
				file >> t;
				b->points.push_back(&skin->points[t]);
				b->orig_points.push_back(t);
			}
                        for(int j=0;j<4;j++){
                        //load keyframe data recording each bone from 0 to 3000 4 frames in total per bone
                                file >> anglex;
                                file >> angley;
                                file >> anglez;
                                file >> len;
                                file >> time;
                                //update keyframes array
                                if(keyframes.size() == 0 || time > keyframes.back())//.back() return the last element
                                        keyframes.push_back(time);
                                keyframe * k = new keyframe;
                                k->anglex = anglex;
                                k->angley = angley;
                                k->anglez = anglez;
                                k->length = len;
                                k->time = time;
                                b->keyframes.push_back(k);
                        }
    	}
    	file.close();
	}
	else cout << "unable to open file" << endl;
	skeleton->done = false;
}

void key_set::next_frame(){
	cframe++;
	if (cframe >= keyframes.size()) cframe = 0;
	cout << "keyframe switched to " << cframe << endl;
}

void key_set::animate(int t, bool draw){
	//compute which keyframes to interpolate between
	glDisable(GL_LIGHTING);
	int prev, next;
	double u;

	if(keyframes.size() < 2) {
		cerr << "need at least two keyframes to animate" << endl;
		return;
	}

	//cout << "t = " << t << "keyframes.back() = " << keyframes.back() << endl;
	if (t > keyframes.back()) t = (t%keyframes.back()); //repeat animation if it gets to end

	for(unsigned i=1;i<keyframes.size();i++){
		if(t <= keyframes[i]) {
			prev = i-1;
			next = i;
			break;
		}
	}
	 
	u = t-keyframes[prev];
	u = u/(keyframes[next]-keyframes[prev]);

	//create a identity quaternion to start with
	quat q(0,0,0,1);
	animate_rec(q, skeleton->begin, skeleton, prev, next, u, draw);

	glEnable(GL_LIGHTING);
}

void key_set::set_cur_to_keyframe(){
	set_cur_to_keyframe_rec(skeleton);
	pre_compute_quat_rec(quat(0,0,0,1), skeleton->begin, skeleton, false, 0);
}

//interpolate keyframes into angle x,y,z recursively
void key_set::set_cur_to_keyframe_rec(bone * root){

	root->curx = root->keyframes[cframe]->anglex;
	root->cury = root->keyframes[cframe]->angley;
	root->curz = root->keyframes[cframe]->anglez;

	for(unsigned i=0;i<root->child.size();i++){
		set_cur_to_keyframe_rec(root->child[i]);
	}
}

void key_set::animate_rec(    //recursive code to animate skeleton //calculate end position
	quat last, 
	point begin, 
	bone * root, 
	int prev, 
	int next, 
	double u,
	bool draw)
{ 
       if(root != NULL) {
	vec end;
	end.x = 0.0;
	end.y = root->length;
	end.z = 0.0;

	quat r;
	//do linear interp
	//r = (root->keyframes[prev]->q * (1-u)) + (root->keyframes[next]->q * u);
 	r.slerp(root->keyframes[prev]->q, root->keyframes[next]->q, u);
	end = r * end; //do rotation based on final quaternion

	end.x += begin.x;
	end.y += begin.y;
	end.z += begin.z;

	if(draw){
	if(root == cur) glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINES);
		glVertex3f(begin.x,begin.y,begin.z);
		glVertex3f(end.x,end.y,end.z);
	glEnd();

	glPushMatrix();
		glTranslatef(end.x,end.y,end.z);
		glutSolidSphere(0.1, 5, 5);
	glPopMatrix(); 
	glColor3f(1.0,1.0,1.0);
	}

	point pend; //convert vec struct back to point struct
	pend.x = end.x;
	pend.y = end.y;
	pend.z = end.z;

	move_points_anim(root, r, begin);

	for(unsigned int i=0;i<root->child.size();i++){
		animate_rec(r, pend, root->child[i], prev, next, u, draw);
	}
       }
}

void key_set::move_points_anim(bone * root, quat & q, point & begin){

	for(int i=0;i<root->points.size();i++){

		vec v;
		v.x = orig_skin[root->orig_points[i]].x - root->start.x;
		v.y = orig_skin[root->orig_points[i]].y - root->start.y;
		v.z = orig_skin[root->orig_points[i]].z - root->start.z;

		v = root->o.get_conjugate() * v;

		v = q * v; //do rotation based on quaternions
		
		root->points[i]->x = v.x + begin.x;
		root->points[i]->y = v.y + begin.y;
		root->points[i]->z = v.z + begin.z;
	}
}

//compute quaternions before running animation to save time
void key_set::pre_compute_quat(){
        //computer for base skeleton

        quat q(0,0,0,1);
        pre_compute_quat_rec(q, skeleton->begin, skeleton, false, 0);

        //compute for each keyframe
        for(int i=0;i<keyframes.size();i++){
                pre_compute_quat_rec(quat(0,0,0,1), skeleton->begin, skeleton, true, i);
        }
}

//recurisve function for pre_compute_quat
//if keyframe, will computer quaternions for keyframe # k
void key_set::pre_compute_quat_rec(quat r, point begin, bone * root, bool keyframe, int k){
        if(k > root->keyframes.size() - 1) {
                cerr << "error: pre_compute_quat_rec: k not in bounds" << endl;
                exit(1);
        }

        // assign starting position of bone
        if(keyframe){
                root->keyframes[k]->begin.x = begin.x;
                root->keyframes[k]->begin.y = begin.y;
                root->keyframes[k]->begin.z = begin.z;
        }else{
                root->begin.x = begin.x;
                root->begin.y = begin.y;
                root->begin.z = begin.z;
        }
        root->start.x = begin.x;
        root->start.y = begin.y;
        root->start.z = begin.z;
        quat q;
        if(keyframe){
                q.from_euler(root->keyframes[k]->anglex,root->keyframes[k]->angley,
                                        root->keyframes[k]->anglez);
                q = r * q;
        root->keyframes[k]->q = q;
        }else{
                q.from_euler(root->curx,root->cury,root->curz);
                q = r * q;
                root->q = q;
                root->o = q;
        }

        vec end;
        end.x = 0.0;
        end.y = root->length;
        end.z = 0.0;

        end = q * end; //do rotation based on quaternions

        end.x += begin.x;
        end.y += begin.y;
        end.z += begin.z;

        point pend; //convert vec struct back to point struct
        pend.x = end.x;
        pend.y = end.y;
        pend.z = end.z;

        for(unsigned int i=0;i<root->child.size();i++){
                pre_compute_quat_rec(q, pend, root->child[i], keyframe, k);
        }
}
