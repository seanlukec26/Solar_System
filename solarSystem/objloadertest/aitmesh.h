#ifndef AITMESH_H
#define AITMESH_H

#include <string>
#include <fstream>
#include <vector>

#include "aitvertex.h"

class aitMesh
{
public:
	aitMesh() {};
	~aitMesh() {};
	bool loadFromObj(std::string path);
	std::vector<aitVertex>& getVertices(); 

private:
	std::vector<aitVertex> vertices;
};


inline std::vector<aitVertex>& aitMesh::getVertices()
{
	return vertices;
}

#endif //AITTEXTUREDMESH_H