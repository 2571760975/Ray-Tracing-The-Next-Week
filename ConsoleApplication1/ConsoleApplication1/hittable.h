#pragma once
#include "ray.h"
#include "rtweekend.h"
#include "aabb.h"
class material;

//这个类是检测相交的基类
struct hit_record //记录相交，同时也负责集体传参
{
	vec3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;//使用智能指针装载材质
	double t;//时间变量
	double u;//u,v信息
	double v;
	bool front_face;//光线的射入面


	inline void set_face_normal(const ray& r, const vec3& outward_normal) {//判断入射面是内侧还是外侧
		//这里是使用的法相永远与入射方向相反的做法
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable
{
	public:
		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const = 0;
		virtual bool bounding_box(double t0, double t1, aabb& output_box) const = 0;
		//返回值为BOOL表示并不是所有的图元都会有包围盒（eg.比如一个无限延申的平面）
		//t0,t1是接受物体的运动状态，包围盒需要在这个时间区间内运动的个物体完整的包围起来
};

