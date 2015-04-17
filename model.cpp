#include "math.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#include "model.h"

model::model():num_points(0), num_faces(0)
{
}

model::model(const char * file_name): num_points(0),  num_faces(0){
        load_model(file_name);
}

model::~model(){
}

void model::load_model(const char * file_name){
	ifstream file(file_name);
	string word;
	int face_num=0; //current face index in faces array

	if (file.is_open())
  	{
		while(file.good() )
		{
			file >> word;//skip the space by default
			//cout << "word is " << word << endl;
			if (word == "v"){
				//we know its a vertex so load the three(x, y, z) values
				point * new_point = new point;
				file >> new_point->x;
				file >> new_point->y;
				file >> new_point->z;
				new_point->num_faces=0;
				points.push_back(*new_point);
				num_points++;
				//load a blank value into the vertex_normals
				point * new_norm = new point;
				new_norm->x = 0;
				new_norm->y = 0;
				new_norm->z = 0;
				vert_normals.push_back(*new_norm);
			}
			
			if(word == "f"){
				//it's a face so keep loading values until not a number
				face * new_face = new face;
				new_face->num_points = 0;
				string input;
				int point_num;
				bool same_face = true;
				while(same_face){
					file >> input;
					const char * n = input.c_str();
					if(!file.good()) break;
					point_num = atoi(n);
					if(point_num){ //if its a number enter it into the current face
						new_face->points.push_back(point_num);//point_num corresponds to the index number as previously defined in points vector,different is points starting from 0,but f index starting from 1
						new_face->num_points++;
						points[point_num-1].faces.push_back(face_num);//face_num is the current index for face NO.
						points[point_num-1].num_faces++;
					}else{ //otherwise stop...
						file.seekg(-1, ios::cur); //set seek pos back one
						same_face = false;
					}
				}
				face_num++;
				faces.push_back(*new_face);
				num_faces++;
			}
			
		}
		file.close();
	}

	get_normals(); // calculate normals;
}

void model::get_normals(){
	 point a, b;

        //first get face normals
    for(int i=0;i<num_faces;i++){
        a.x = points[faces[i].points[0]-1].x - points[faces[i].points[1]-1].x;
        a.y = points[faces[i].points[0]-1].y - points[faces[i].points[1]-1].y;
        a.z = points[faces[i].points[0]-1].z - points[faces[i].points[1]-1].z;

        b.x = points[faces[i].points[1]-1].x - points[faces[i].points[2]-1].x;
        b.y = points[faces[i].points[1]-1].y - points[faces[i].points[2]-1].y;
        b.z = points[faces[i].points[1]-1].z - points[faces[i].points[2]-1].z;

		  point * new_point = new point;
        new_point->x = (a.y*b.z) - (a.z*b.y);
        new_point->y = (a.z*b.x) - (a.x*b.z);
        new_point->z = (a.x*b.y) - (a.y*b.x);

//we doing the unit normalize which divide x,y,z value by the length which is sqrt(x2+y2+z2).
//such normal vector will remain normalize as long as rotation and transformation, but if scaling involved then better trigger glEnable(GL_NORMALIZE) to let opengl do normalize work automatically

        float len = (float)(sqrt((new_point->x * new_point->x)+
                 (new_point->y * new_point->y) +
                 (new_point->z * new_point->z)));

         if (len == 0.0f)
             len = 1.0f;

         new_point->x /= len;
         new_point->y /= len;
         new_point->z /= len;
		  face_normals.push_back(*new_point);
    }

    //normalize();

	//now do vertex normals
	for(int i=0;i<num_points;i++){
		int b = points[i].num_faces;

             //we will sum all neighbor face's normal value for each sharing vertex normal
             //because if a flat surface,then all vertexs which reside on the same surface whose normal share the same value as surface normal
		for(int a=0;a<b;a++){
			vert_normals[i].x += face_normals[points[i].faces[a]].x;
			vert_normals[i].y += face_normals[points[i].faces[a]].y;
			vert_normals[i].z += face_normals[points[i].faces[a]].z;
		}
	}

	for(int i=0;i<num_points;i++){
   	float len = (float)(sqrt((vert_normals[i].x * vert_normals[i].x)+
              (vert_normals[i].y * vert_normals[i].y) +
              (vert_normals[i].z * vert_normals[i].z)));

         if (len == 0.0f)
             len = 1.0f;

         vert_normals[i].x /= len;
         vert_normals[i].y /= len;
         vert_normals[i].z /= len;
    }

}

void model::draw_model(bool normals_on, bool smooth){
	double xavrg = 0;//only viable for drawing the normal line
	double yavrg = 0;//only viable for drawing the normal line
	double zavrg = 0;//only viable for drawing the normal line
	int num = 0; //number of points for each face
	for(int i=0;i<num_faces;i++){
		if(!smooth) glNormal3f(face_normals[i].x,face_normals[i].y,face_normals[i].z);
		glBegin(GL_POLYGON);
		for(int j=0;j<faces[i].num_points;j++){
			glColor3f(1.0f, 0.0f, 0.0f);
			if(smooth)
			glNormal3f(vert_normals[faces[i].points[j]-1].x,
							vert_normals[faces[i].points[j]-1].y,
							vert_normals[faces[i].points[j]-1].z);
			glVertex3f(
					points[faces[i].points[j]-1].x,
					points[faces[i].points[j]-1].y,
					points[faces[i].points[j]-1].z
					);
			xavrg += points[faces[i].points[j]-1].x;
			yavrg += points[faces[i].points[j]-1].y;
			zavrg += points[faces[i].points[j]-1].z;
			num++;
		}
		glEnd();
	
		//draw normal for each face,which is a line represent the direction of each normal per face
		if(normals_on){
			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_LINES);
				glVertex3f(xavrg/num,yavrg/num,zavrg/num);
				glVertex3f(xavrg/num+face_normals[i].x,
							yavrg/num+face_normals[i].y,
							face_normals[i].z+zavrg/num);
			glEnd();
		}

		xavrg = 0;
		yavrg = 0;
		zavrg = 0;
		num = 0;
	}
}
