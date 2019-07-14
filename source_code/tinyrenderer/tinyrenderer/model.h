#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<Vec3f> uv_;
	std::vector<std::vector<int> > faces_, texts_;
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	int nuv();
	Vec3f vert(int i);
	Vec3f uv(int i);
	std::vector<int> face(int idx), text(int idy);
};

#endif //__MODEL_H__
