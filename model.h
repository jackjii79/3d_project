#ifndef MODEL_H
#define MODEL_H

#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

typedef struct point {
	double x;
	double y;
	double z;
	//these next two fields are useless for skeleton points - oh well
	std::vector<int> faces; //array of faces that point is connected to
	int num_faces; //number of faces the point is a part of
} point;

typedef struct face {
    std::vector<int> points; //array of the points in the face
    int num_points;
} face;

class model 
{
public:
        model();
        model(const char * file);
        ~model();
	void load_model(const char * file);
        void get_normals();
	std::vector<point> points; //array of all the points in the model
	void draw_model(bool normals_on, bool smooth);
private:
	//model functions and fields
	std::vector<face> faces;	//array of all the faces/polygons in the model
	std::vector<point> face_normals;
	std::vector<point> vert_normals;
	int num_points;
	int num_faces;

	//allow key_set access to fields
	friend class key_set;
	friend class select_sphere;
};

#endif // MODEL_H
