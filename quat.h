#ifndef __QUAT_H__
#define __QUAT_H__

typedef struct {
	double x;
	double y;
	double z;
} vec;

class quat{
public:
	quat();
	quat(const quat& q);
	quat(double,double,double,double);
	~quat();
	void normalize();
	quat get_conjugate();
	quat operator* (const quat &rq) const;
	quat operator* (const double &d) const;
	vec operator* (const vec &v) const;
	quat operator+ (const quat &q) const;
	void from_axis(const vec &v, float angle);
	void from_euler (double x, double y, double z);
	quat get_partial_rot(double f);
	void slerp(quat & a, quat & b, double f);

	double w;
	double x;
	double y;
	double z;
private:

};

#endif
