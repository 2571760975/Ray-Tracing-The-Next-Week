#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include"rtweekend.h"

#include "hittable_list.h"
#include"sphere.h"
#include"camera.h"
#include"material.h"
#include"moving_sphere.h"
#include"bvh.h"
#include "surface_texture.h"
#include "xy_rect.h"
#include "box.h"
#include "Control.h"
#include "constant_medium.h"


#include<iostream>

#pragma region "random_scene"
hittable_list random_scene() {//建立一个随机场景
	hittable_list world;

	auto checker = make_shared<checker_texture>(
		make_shared<constant_texture>(vec3(0.2, 0.3, 0.1)),
		make_shared<constant_texture>(vec3(0.9, 0.9, 0.9))
		);

	world.add(make_shared<sphere>(
		vec3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = vec3::random() * vec3::random();
					world.add(make_shared<moving_sphere>(
						center, center + vec3(0, random_double(0,0.5), 0), 0.0, 1.0, 0.2,
						make_shared<lambertian>(make_shared<constant_texture>(vec3(0.5, 0.5, 0.5)))));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = vec3::random(.5, 1);
					auto fuzz = random_double(0, .5);
					world.add(
						make_shared<sphere>(center, 0.2, make_shared<metal>(albedo, fuzz)));
				}
				else {
					// glass
					world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
				}
			}
		}
	}

	world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));

	world.add(
		make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(make_shared<constant_texture>(vec3(0.4, 0.2, 0.1)))));

	world.add(
		make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));

	return static_cast<hittable_list>(make_shared<bvh_node>(world,0,1));
}//随机场景1 //随机场景1

hittable_list two_spheres() {
	hittable_list objects;


	auto checker = make_shared<checker_texture>(
		make_shared<constant_texture>(vec3(0.2, 0.3, 0.1)),
		make_shared<constant_texture>(vec3(0.9, 0.9, 0.9))
		);

	objects.add(make_shared<sphere>(vec3(0, -10, 0), 10, make_shared<lambertian>(checker)));
	objects.add(make_shared<sphere>(vec3(0, 10, 0), 10, make_shared<lambertian>(checker)));

	return objects;
}

hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(vec3(0,-1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(vec3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return objects;
} 

hittable_list earth() {
	int nx, ny, nn;
	unsigned char* texture_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);

	auto earth_surface =
		make_shared<lambertian>(make_shared<image_texture>(texture_data, nx, ny));
	auto globe = make_shared<sphere>(vec3(0, 0, 0), 2, earth_surface);

	return hittable_list(globe);
}

hittable_list simple_light() {
	hittable_list objects;

	auto pertext = make_shared<noise_texture>(4);
	objects.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	objects.add(make_shared<sphere>(vec3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	auto difflight = make_shared<diffuse_light>(make_shared<constant_texture>(vec3(4, 4, 4)));
	objects.add(make_shared<sphere>(vec3(0, 7, 0), 2, difflight));
	objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

	return objects;
}

hittable_list cornell_box() {
	hittable_list objects;

	auto red = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.65, 0.05, 0.05)));
	auto white = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.73, 0.73, 0.73)));
	auto green = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.12, 0.45, 0.15)));
	auto light = make_shared<diffuse_light>(make_shared<constant_texture>(vec3(15, 15, 15)));



	objects.add(make_shared<flip_face>(make_shared<yz_rect>(0, 555, 0, 555, 555, green)));
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
	objects.add(make_shared<flip_face>(make_shared<xz_rect>(0, 555, 0, 555, 555, white)));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.add(make_shared<flip_face>(make_shared<xy_rect>(0, 555, 0, 555, 555, white)));

	shared_ptr<hittable> box1 = make_shared<box>(vec3(0, 0, 0), vec3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	objects.add(box1);

	shared_ptr<hittable> box2 = make_shared<box>(vec3(0, 0, 0), vec3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));
	objects.add(box2);


	return objects;
}

hittable_list cornell_smoke() {
	hittable_list objects;

	auto red = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.65, 0.05, 0.05)));
	auto white = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.73, 0.73, 0.73)));
	auto green = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.12, 0.45, 0.15)));
	auto light = make_shared<diffuse_light>(make_shared<constant_texture>(vec3(7, 7, 7)));

	objects.add(make_shared<flip_face>(make_shared<yz_rect>(0, 555, 0, 555, 555, green)));
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
	objects.add(make_shared<flip_face>(make_shared<xz_rect>(0, 555, 0, 555, 555, white)));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.add(make_shared<flip_face>(make_shared<xy_rect>(0, 555, 0, 555, 555, white)));

	shared_ptr<hittable> box1 = make_shared<box>(vec3(0, 0, 0), vec3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));

	shared_ptr<hittable> box2 = make_shared<box>(vec3(0, 0, 0), vec3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));

	objects.add(
		make_shared<constant_medium>(box1, 0.01, make_shared<constant_texture>(vec3(0, 0, 0))));
	objects.add(
		make_shared<constant_medium>(box2, 0.01, make_shared<constant_texture>(vec3(1, 1, 1))));

	return objects;
}

hittable_list final_scene() {
	hittable_list boxes1;
	auto ground =
		make_shared<lambertian>(make_shared<constant_texture>(vec3(0.48, 0.83, 0.53)));

	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = random_double(1, 101);
			auto z1 = z0 + w;

			boxes1.add(make_shared<box>(vec3(x0, y0, z0), vec3(x1, y1, z1), ground));
		}
	}

	hittable_list objects;

	objects.add(make_shared<bvh_node>(boxes1, 0, 1));

	auto light = make_shared<diffuse_light>(make_shared<constant_texture>(vec3(7, 7, 7)));
	objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

	auto center1 = vec3(400, 400, 200);
	auto center2 = center1 + vec3(30, 0, 0);
	auto moving_sphere_material =
		make_shared<lambertian>(make_shared<constant_texture>(vec3(0.7, 0.3, 0.1)));
	objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

	objects.add(make_shared<sphere>(vec3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
	objects.add(make_shared<sphere>(
		vec3(0, 150, 145), 50, make_shared<metal>(vec3(0.8, 0.8, 0.9), 10.0)
		));

	auto boundary = make_shared<sphere>(vec3(360, 150, 145), 70, make_shared<dielectric>(1.5));
	objects.add(boundary);
	objects.add(make_shared<constant_medium>(
		boundary, 0.2, make_shared<constant_texture>(vec3(0.2, 0.4, 0.9))
		));
	boundary = make_shared<sphere>(vec3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
	objects.add(make_shared<constant_medium>(
		boundary, .0001, make_shared<constant_texture>(vec3(1, 1, 1))));

	int nx, ny, nn;
	auto tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	auto emat = make_shared<lambertian>(make_shared<image_texture>(tex_data, nx, ny));
	objects.add(make_shared<sphere>(vec3(400, 200, 400), 100, emat));
	auto pertext = make_shared<noise_texture>(0.1);
	objects.add(make_shared<sphere>(vec3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

	hittable_list boxes2;
	auto white = make_shared<lambertian>(make_shared<constant_texture>(vec3(0.73, 0.73, 0.73)));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.add(make_shared<sphere>(vec3::random(0, 165), 10, white));
	}

	objects.add(make_shared<translate>(
		make_shared<rotate_y>(
			make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
		vec3(-100, 270, 395)
		)
	);

	return objects;
}

#pragma endregion

vec3 ray_color(const ray& r,const vec3& background,const hittable& world,int depth) {//使用层数来限制递归次数，避免保栈
	hit_record rec;

	if (depth <= 0)
		return vec3(0, 0, 0);

	if (!world.hit(r, 0.001, infinity, rec))//如果没有打中物体，就返回背景色
		return background;

	ray scattered;
	vec3 attenuation;
	vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
	if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		return emitted;

	return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
	
	//vec3 unit_direction = unit_vector(r.direction());//标准化向量
	//auto t = 0.5*(unit_direction.y() + 1.0);//计算y轴上的像素位置
	//return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);//蓝白线性插值
}



int main() {
	//定义图片大小，像素多少
	const int image_width = 200;
	const int image_height = 200;
	const int samples_per_pixel = 100;//添加采样数
	const int max_depth = 50;//递归次数
	const auto aspect_ratio = double(image_width) / image_height;
	const vec3 background(0, 0, 0);//背景颜色
	const vec3 vup(0, 1, 0);
	vec3 lookfrom(278, 278, -800);//摄像机平面
	vec3 lookat(278, 278, 0);//看向方向
	auto dist_to_focus = 10.0;//距离
	auto aperture = 0.0;//光圈大小
	auto vfov = 40;//视角宽度

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	auto R = cos(pi / 4);
	hittable_list world = final_scene();//得到场景

	camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus,0.0,1.0);//加入摄像机

	for (int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining" << j << ' ' << std::flush;//使用错误输出流+刷新
		for (int i = 0; i < image_width; ++i) {
			vec3 color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {//这里的做法是在像素内随机区域继续打出光线
				auto u = (i + random_double()) / image_width;
				auto v = (j + random_double()) / image_height;
				ray r = cam.get_ray(u, v);
				color += ray_color(r,background,world,max_depth);
			}
			color.write_color(std::cout, samples_per_pixel);//在输出流中做处理
		}
	}
	std::cerr << "\nDone.\n";
}



//光追的整个渲染流程
//使用255.999的原因是因为颜色值取不到256
//关于ray类
//ppm格式的文件使用极速看图打开
//线性插值的一般形式
//使用3D的计算方式，放入2D的结果
//球体求交公式推导
//求根公式系数消去？为什么
//为什么需要t_max和t_min判断相交是否有效,为了计算第一个点的颜色
//智能指针Share_ptr
//采样点抗锯齿...光追居然是这种方法实现的吗
//大球表现为绿色原因估计是因为法向量和采样射线的交角差不多，所以都是绿的，看小球的球顶可以看出来
//漫反射是根据切线建立球体选择随机点来进行反射模拟的
//声明成inline的函数可以跨文件使用吗？ double_random()
//控制漫反射光线的否定法
//伽马矫正
//shadow ance
//lambertian分布律 cos系数矫正
//为什么使用半球矫正法不需要加上法向量？
//反射向量的计算方法
//材质scatter的具体做法
//使用反射随机球半径来模拟模糊程度，半径越大，整体也就越模糊
//别忘了if里面的语句也是会运算的，albeo赋值给了attunation成为了颜色
//绝缘体材质的折射与反射策略
//Snell定律计算，拆解成平行光和垂直光的叠加，公式要记住
//全内反射的发生情况，（从高折射率介质射入低折射率介质，玻璃到空气会形成全内反射，Snell法则会无解）
//摄像机的旋向计算
//散焦模糊，即模仿现实相机透镜
//图片纹理映射