#pragma once

#include "rtweekend.h"
#include "texture.h"

//每一个材质类都应该封装两个功能：
//1.生成散射后的光线(或者说决定吸收了多少光线)
//2.如果发生散射，决定光线会变暗多少(attenuate)
class material
{
	public:
		virtual vec3 emitted(double u, double v, const vec3& p)const {
			return vec3(0, 0, 0);//让不是光源的物体默认返回黑色
		}

		virtual bool scatter(const ray& r_in,const hit_record& rec,vec3& attenuation,ray& scattered)const = 0;
};


class lambertian : public material {//漫反射材质模型
	public: 
		lambertian(shared_ptr<texture> a):albedo(a){}

		virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
			vec3 scatter_direction = rec.normal + random_unit_vector();
			scattered = ray(rec.p, scatter_direction,r_in.time());//确保散射光线和入射光线存在的时间点相同
			attenuation = albedo->value(rec.u,rec.v,rec.p);
			return true;
		}
	public:
		shared_ptr<texture> albedo;//颜色
};

class metal :public material {//金属材质
	public:
		metal(const vec3& a,double f):albedo(a),fuzz(f<1?f:1){}

		virtual bool scatter(
			const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const {
			vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
			scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere(),r_in.time());//给予反射方向一定的随机性
			attenuation = albedo;
			return (dot(scattered.direction(), rec.normal) > 0);
		}

	public:
		vec3 albedo;
		double fuzz;//反射向量的随机球

};

class dielectric : public material {
	public:
		dielectric(double ri):ref_idx(ri){}

		virtual bool scatter (const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const{
			attenuation = vec3(1.0, 1.0, 1.0);
			double etai_over_etat;//透射比
			if (rec.front_face) {//需要根据入射面来对应得到透射比
				etai_over_etat = 1.0 / ref_idx;
			}
			else {
				etai_over_etat = ref_idx;
			}
			vec3 unit_direction = unit_vector(r_in.direction());
			double cos_theta = ffmin(dot(-unit_direction, rec.normal), 1.0);
			double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
			if (etai_over_etat * sin_theta > 1.0) {
				vec3 reflected = reflect(unit_direction, rec.normal);
				scattered = ray(rec.p, reflected, r_in.time());
				return true;
			}

			double reflect_prob = schlick(cos_theta, etai_over_etat);//根据入射角模拟折射概率
			if (random_double() < reflect_prob)
			{
				vec3 reflected = reflect(unit_direction, rec.normal);
				scattered = ray(rec.p, reflected, r_in.time());
				return true;
			}
			vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
			scattered = ray(rec.p, refracted, r_in.time());
			return true;
		}
	public:
		double ref_idx;
};


class diffuse_light : public material {
	public:
		diffuse_light(shared_ptr<texture> a) : emit(a){}

		virtual bool scatter(
			const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
		) const override {
			return false;//投光物不反射光线
		}

		virtual vec3 emitted(double u, double v, const vec3& p)const override{
			return emit->value(u, v, p);
		}

	public:
		shared_ptr<texture> emit;


};

class isotropic : public material {
public:
	isotropic(shared_ptr<texture> a) : albedo(a) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
	) const {
		scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

public:
	shared_ptr<texture> albedo;
};