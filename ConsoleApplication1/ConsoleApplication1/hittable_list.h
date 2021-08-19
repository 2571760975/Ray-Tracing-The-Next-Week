#pragma once
#include "hittable.h"
#include<memory>
#include<vector>

using std::shared_ptr;//�����õ�������ָ�룬����ÿһ�ζ�std::shared_ptr
using std::make_shared;

//�����б�
class hittable_list : public hittable
{
	public:
		hittable_list() {}
		hittable_list(shared_ptr<hittable> object) { add(object); }

		void clear() { objects.clear(); }
		void add(shared_ptr<hittable> object) { objects.push_back(object); }

		virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
		virtual bool bounding_box(double t0, double t1, aabb& output_box) const override;
	public:
		std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;//�Ƿ��������ཻ
	auto closest_so_far = t_max;//����Ľ���

	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest_so_far, temp_rec)) {//����ÿһ�������Լ��Ļ����ж��������麯�����õ�����
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}

bool hittable_list::bounding_box(double t0, double t1, aabb& output_box)const {//�����Լ��İ�Χ��
	if (objects.empty()) return false;

	aabb temp_box;
	bool first_box = true;
	for (const auto& object : objects) {
		if (!object->bounding_box(t0, t1, temp_box)) return false;
		output_box = first_box ? temp_box: surrounding_box(output_box, temp_box);
		first_box = false;
	}
	return true;
}
