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
Model *model = new Model("obj/african_head.obj");


// Line drawing method using Bresenham's line algorithm
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {

	bool steep = false;

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


// Main method
int main(int argc, char** argv) {

	TGAImage image(width, height, TGAImage::RGB);
	
	// Draw mesh 
	/*
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);

		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);

			int x0 = (v0.x + 1.0) * width / 2;
			int y0 = (v0.y + 1.0) * height / 2;
			int x1 = (v1.x + 1.0) * width / 2;
			int y1 = (v1.y + 1.0) * height / 2;

			line(x0, y0, x1, y1, image, white);
		}
	}*/

	Vec2i t0[3] = { Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80) };
	Vec2i t1[3] = { Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180) };
	Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };
	triangle(t0[0], t0[1], t0[2], image, red);
	triangle(t1[0], t1[1], t1[2], image, green);
	triangle(t2[0], t2[1], t2[2], image, blue);

	image.flip_vertically();
	image.write_tga_file("output.tga");
	
	return 0;

}