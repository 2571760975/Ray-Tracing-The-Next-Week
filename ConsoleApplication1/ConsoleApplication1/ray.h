#pragma once
#include "vec3.h"

//光线类
class ray
{	
	public:
		ray() {}
		ray(const vec3& origin,const vec3& direction,double time=0.0):orig(origin),dir(direction),tm(time){}

		vec3 origin() const { return orig; }
		vec3 direction() const { return dir; }
		double time() const { return tm; }

		vec3 at(double t) const { return orig + t * dir;}//假定的光线公式

	public:
		vec3 orig;
		vec3 dir;
		double tm;//用来存储光线所在的时刻

};

//光线追踪器的核心是从像素发射射线，并且计算这些射线得到的颜色。这包括如下步骤：
//将射线从视点转换为像素坐标
//计算光线是否与场景中的物体相交
//如果有，则计算交点的颜色

