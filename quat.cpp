#include<math.h>
#include"quat.h"

#define TOLERANCE 0.00001f
#define PIOVER180 0.017453f

quat::quat(){
}

quat::quat(double x,double y,double z, double w){
	this->w = w;//weight, default is 1.0
	this->x = x;
	this->y = y;
	this->z = z;
}

quat::quat(const quat& q){
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
}

quat::~quat(){
}

void quat::normalize(){
	float mag2 = w * w + x * x + y * y + z * z;
	if (  mag2!=0.f && (fabs(mag2 - 1.0f) > TOLERANCE)) {
		float mag = sqrt(mag2);
		w /= mag;
		x /= mag;
		y /= mag;
		z /= mag;
	}
}


quat quat::get_conjugate(){
	return quat(-x,-y,-z,w);
}

quat quat::operator* (const quat &rq) const{
	return quat(w * rq.x + x * rq.w + y * rq.z - z * rq.y,
               w * rq.y + y * rq.w + z * rq.x - x * rq.z,
               w * rq.z + z * rq.w + x * rq.y - y * rq.x,
	            w * rq.w - x * rq.x - y * rq.y - z * rq.z);
}

vec quat::operator* (const vec &v) const{
	vec t;
	t.x = v.x;
	t.y = v.y;
	t.z = v.z;

	//normalize t
/*	double len = sqrt(t.x*t.x + t.y*t.y + t.z*t.z);
	if(len == 0.0) len = 1.0;
	t.x /= len;
	t.y /= len;
	t.z /= len; */

	quat vec_quat, res_quat;
	vec_quat.x = t.x;
	vec_quat.y = t.y;
	vec_quat.z = t.z;
	vec_quat.w = 0.0f;

	quat conj;
	conj.x = -x;
	conj.y = -y;
	conj.z = -z;
	conj.w = w;

	res_quat = vec_quat * conj;
	res_quat = *this * res_quat;

	t.x = res_quat.x;
	t.y = res_quat.y;
	t.z = res_quat.z;
	return t;
}

quat quat::operator+ (const quat &q) const{
	return quat(x + q.x,
					y + q.y,
					z + q.z,
					w + q.w);
}

quat quat::operator* (const double &d) const{
	return quat(x * d,
					y * d,
					z * d,
					w * d);
}

//make sure that angle is degrees
void quat::from_axis(const vec &v, float angle)
{
	float sinAngle;
	//convert angle to radians
	angle = angle*PIOVER180;
	angle *= 0.5f;
	vec vn;
	vn.x = v.x;
	vn.y = v.y;
	vn.z = v.z;

	double len = sqrt(vn.x*vn.x + vn.y*vn.y + vn.z*vn.z);
	if(len == 0.0) len = 1.0;
	vn.x /= len;
	vn.y /= len;
	vn.z /= len;
 
	sinAngle = sin(angle);
 
	x = (vn.x * sinAngle);
	y = (vn.y * sinAngle);
	z = (vn.z * sinAngle);
	w = cos(angle);
}

//f needs to be between 1 and 0
quat quat::get_partial_rot(double f){
	quat q(*this);

	double angle = 2 * acos (q.w);
	double xval = q.x/sin(angle*2);
	double yval = q.y/sin(angle*2);
	double zval = q.z/sin(angle*2);

	angle = angle * f;

	quat b(cos(angle/2), sin(angle/2)*xval,sin(angle/2)*yval, sin(angle/2)*zval);

	b.normalize();
	return b;
}

void quat::slerp(quat & a, quat & b, double f){
	if(a.w == b.w &&
		a.x == b.x &&
		a.y == b.y &&
		a.z == b.z){
		this->w = a.w;
		this->x = a.x;
		this->y = a.y;
		this->z = a.z;
		return;
	}

	//find angle
	double val = a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;
	double angle = acos(val);
	
	//now do slerp
	quat q = a * (sin((1-f)*angle)/sin(angle)) + b * (sin(angle * f)/sin(angle));
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
}

void quat::from_euler (double xa, double ya, double za){

	vec x_ax;
	x_ax.x = 1.0;
	x_ax.y = 0.0;
	x_ax.z = 0.0;
	
	vec y_ax;
	y_ax.x = 0.0;
	y_ax.y = 1.0;
	y_ax.z = 0.0;

	vec z_ax;
	z_ax.x = 0.0;
	z_ax.y = 0.0;
	z_ax.z = 1.0;

	
	quat xquat;
	xquat.from_axis(x_ax,xa);
	quat yquat;
	yquat.from_axis(y_ax, ya);
	quat zquat;
	zquat.from_axis(z_ax, za);

	quat res =  xquat * (yquat * zquat);
	x = res.x;
	y = res.y;
	z = res.z;
	w = res.w;
 	
	normalize();
}
