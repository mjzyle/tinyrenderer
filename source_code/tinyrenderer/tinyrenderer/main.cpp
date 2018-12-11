#include <vector>
#include <cmath>
#include "tgaimage.h"


// Mesh variables
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255,   0,   0, 255);
const int width = 800;
const int height = 800;


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


int main(int argc, char** argv) {

	TGAImage image(100, 100, TGAImage::RGB);
	
	// Draw mesh
	/* ADD CUSTOM CODE */

	image.flip_vertically();
	image.write_tga_file("output.tga");
	
	return 0;

}