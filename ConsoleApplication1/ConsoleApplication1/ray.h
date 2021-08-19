#pragma once
#include "vec3.h"

//������
class ray
{	
	public:
		ray() {}
		ray(const vec3& origin,const vec3& direction,double time=0.0):orig(origin),dir(direction),tm(time){}

		vec3 origin() const { return orig; }
		vec3 direction() const { return dir; }
		double time() const { return tm; }

		vec3 at(double t) const { return orig + t * dir;}//�ٶ��Ĺ��߹�ʽ

	public:
		vec3 orig;
		vec3 dir;
		double tm;//�����洢�������ڵ�ʱ��

};

//����׷�����ĺ����Ǵ����ط������ߣ����Ҽ�����Щ���ߵõ�����ɫ����������²��裺
//�����ߴ��ӵ�ת��Ϊ��������
//��������Ƿ��볡���е������ཻ
//����У�����㽻�����ɫ

