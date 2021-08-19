#pragma once

#include "hittable.h"
#include "vec3.h"
#include "surface_texture.h"

//����
void get_sphere_uv(const vec3& p, double& u, double& v) {//�õ������uv����
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
		vec3 center;//����
		double radius;//�뾶
		shared_ptr<material> mat_ptr;//��ŵĲ���ָ��
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
		if (temp < t_max && temp > t_min) {//������Ľ������
			rec.t = temp;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-half_b + root) / a;
		if (temp<t_max && temp > t_min) {//������Ľ������
			rec.t = temp;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);//���������������UV����
			rec.mat_ptr = mat_ptr;//����ʱ�������ָ��
			return true;
		}
	}
	return false;

	//ԭ��ʹ�õ������ʽ
	//vec3 oc = r.origin() - center;
	//auto a = dot(r.direction(), r.direction());
	//auto b = 2.0*dot(oc, r.direction());
	//auto c = dot(oc, oc) - radius * radius;
	//auto discriminant = b * b - 4 * a*c;
	//if (discriminant < 0) {
	//	return -1.0;
	//}
	//else {
	//	return(-b - sqrt(discriminant)) / (2.0*a);//�����ʽ����Ҫ�õ�����
	//}
}

bool sphere::bounding_box(double t0, double t1, aabb& output_box)const {
	output_box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return true;
}

