#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

// Mesh variables
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255,   0,   0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const int width = 800;
const int height = 800;
Model *model = new Model("obj/head.obj");

// Lighting variables
Vec3f light_dir(0, 0, -1);


// Line drawing method using Bresenham's line algorithm
void line(Vec2i v0, Vec2i v1, TGAImage &image, TGAColor color) {

	bool steep = false;

	int x0 = v0.x, y0 = v0.y, x1 = v1.x, y1 = v1.y;

	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror = std::abs(dy * 2);
	int error = 0;
	int y = y0;

	for (int x = x0; x <= x1; x++) {
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}

		error += derror;

		if (error > dx) {
			y += (y1 > y0 ? 1 : -1);
			error -= dx * 2;
		}
	}

}


// Triangle drawing method
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
	// Don't bother with triangles that are just a line
	if (t0.y == t1.y && t0.y == t2.y) return;

	// Sort vertices by y-coordinates (low to high)
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);

	// Draw triangles
	int total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; i++) {
		bool upper_half = i > t1.y - t0.y || t1.y == t0.y;					   // Indicates filling upper half of triangle (or entire if bottom is horizontal)
		int segment_height = upper_half ? t2.y - t1.y : t1.y - t0.y + 1;       // Add 1 pixel to avoid division by zero
		float alpha = (float)i / total_height;
		float beta = (float)(i - (upper_half ? t1.y - t0.y : 0)) / segment_height;
	    
		Vec2i A = t0 + (t2 - t0) * alpha;
		Vec2i B = upper_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;

		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) {
			image.set(j, t0.y + i, color);
		}
	}
}


void triangle(Vec3f *points, float *zbuffer, TGAImage &image, TGAColor color) {

}

// Rasterization method
void rasterize(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color, int ybuffer[]) {
	if (p0.x > p1.x) {
		std::swap(p0, p1);
	}
	for (int x = p0.x; x < p1.x; x++) {
		float t = (x - p0.x) / (float)(p1.x - p0.x);
		int y = p0.y * (1. - t) + p1.y * t;
		if (ybuffer[x] < y) {
			ybuffer[x] = y;
			image.set(x, 0, color);
		}
	}
}

// Main method
int main(int argc, char** argv) {

	TGAImage scene(width, height, TGAImage::RGB);
	/*
	// scene "2d mesh"
	line(Vec2i(20, 34), Vec2i(744, 400), scene, red);
	line(Vec2i(120, 434), Vec2i(444, 400), scene, green);
	line(Vec2i(330, 463), Vec2i(594, 200), scene, blue);

	// screen line
	line(Vec2i(10, 10), Vec2i(790, 10), scene, white);
	*/
	
	float *zbuffer = new float[width * height];
	
	// Draw mesh 
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		Vec2i screen_coords[3];
		Vec3f world_coords[3];
		for (int j = 0; j < 3; j++) {
			Vec3f v = model->vert(face[j]);
			screen_coords[j] = Vec2i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2.);
			world_coords[j] = v;
		}
		Vec3f norm = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
		norm.normalize();
		float intensity = norm * light_dir;
		if (intensity > 0) {
			triangle(screen_coords, zbuffer, scene, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
		}
	}
	
	/*int *zbuffer = new int[width * height];

	TGAImage render(width, 16, TGAImage::RGB);
	int ybuffer[width];
	for (int i = 0; i < width; i++) {
		ybuffer[i] = std::numeric_limits<int>::min();    // Initialize y-buffer to -infinity
	}

	rasterize(Vec2i(20, 34), Vec2i(744, 400), render, red, ybuffer);
	rasterize(Vec2i(120, 434), Vec2i(444, 400), render, green, ybuffer);
	rasterize(Vec2i(330, 463), Vec2i(594, 200), render, blue, ybuffer);*/

	scene.flip_vertically();
	scene.write_tga_file("scene.tga");

	return 0;

}