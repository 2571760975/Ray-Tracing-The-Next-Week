#pragma once

#include "hittable.h"
#include "vec3.h"
#include "surface_texture.h"

//球体
void get_sphere_uv(const vec3& p, double& u, double& v) {//得到球体的uv坐标
	auto phi = atan2(p.z(), p.x());
	auto theta = asin(p.y());
	u = 1 - (phi + pi) / (2 * pi);
	v = (theta + pi / 2) / pi;
}

class sphere : public hittable
{
	public:
		sphere() {}
		sphere(vec3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {}

		virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
		virtual bool bounding_box(double t0, double t1, aabb& output_box) const override;

	public:
		vec3 center;//球心
		double radius;//半径
		shared_ptr<material> mat_ptr;//存放的材质指针
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;

	if (discriminant > 0) {
		auto root = sqrt(discriminant);
		auto temp = (-half_b - root) / a;
		if (temp < t_max && temp > t_min) {//在最近的交点计算
			rec.t = temp;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-half_b + root) / a;
		if (temp<t_max && temp > t_min) {//在最近的交点计算
			rec.t = temp;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);//根据球面坐标求得UV坐标
			rec.mat_ptr = mat_ptr;//击中时计算材质指针
			return true;
		}
	}
	return false;

	//原本使用的求根公式
	//vec3 oc = r.origin() - center;
	//auto a = dot(r.direction(), r.direction());
	//auto b = 2.0*dot(oc, r.direction());
	//auto c = dot(oc, oc) - radius * radius;
	//auto discriminant = b * b - 4 * a*c;
	//if (discriminant < 0) {
	//	return -1.0;
	//}
	//else {
	//	return(-b - sqrt(discriminant)) / (2.0*a);//求根公式，需要得到交点
	//}
}

bool sphere::bounding_box(double t0, double t1, aabb& output_box)const {
	output_box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return true;
}

