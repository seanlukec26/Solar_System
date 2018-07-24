/*
A00207283 Sean Costin
*/
#include "aitMesh.h"
#include <sstream>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <vector>

using namespace std;

bool aitMesh::loadFromObj(std::string path)
{
	
	/*aitVertex v1(glm::vec3(0.000000, 1.000000, 0.000000), glm::vec3(0.000000, 0.000000, 1.000000));
	aitVertex v2(glm::vec3(-1.000000, -1.000000, 0.000000), glm::vec3(0.000000, 0.000000, 1.000000));
	aitVertex v3(glm::vec3(1.000000, -1.000000, 0.000000), glm::vec3(0.000000, 0.000000, 1.000000));
	
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);*/
	
	vector< glm::vec2 > faces;
	vector< glm::vec3 > vert;
	vector< glm::vec3 > normals;
	vector<string> tokens;

	string line;
	ifstream file;
	file.open(path); //open file using path
	if (file.is_open()) //checks if file is open
	{
		while (getline(file, line))
		{
			if (file.eof()) //checks if end of file
			{
				break; //exit while loop
			}
			else if (line[0] == 'v' && line[1] == ' ') //checks if line starts with v and ' '
			{
				glm::vec3 v; //declare vector
				istringstream str(line.substr(2, line.length() - 2)); 
				float x, y, z;
				str >> x >> y >> z;
				//cout << x <<", "<< y <<","<< z <<endl;
				v = glm::vec3(x, y, z);  //assigns float values to vector
				vert.push_back(v); //push back to the vector

			}
			else if (line[0] == 'v' && line[1] == 'n')
			{
				glm::vec3 vn;
				istringstream st(line.substr(3, line.length() - 3));
				float x, y, z;
				st >> x >> y >> z;
				//cout << x <<", "<< y <<","<< z <<endl;
				vn = glm::vec3(x, y, z);
				normals.push_back(vn);
			}
			else if (line[0] == 'f')
			{
				istringstream st(line.substr(2, line.length() - 2));
				string x, y, z;
				char rid = '//';
				st >> x >> y >> z;
				//cout << x <<", "<< y <<","<< z <<endl;
				vector<string> t;
				
				t.push_back(x);
				t.push_back(y);
				t.push_back(z);
				for (string s:t)
				{
					int first, second;
					int pos = s.find('/');
					first = stoi(s.substr(0,pos));
					second = stoi(s.substr(pos+2, s.length()));

					vertices.push_back(aitVertex(vert[first-1], normals[second-1]));
				}
			}
		}
		return true;
	}
	else
	{
		cout<< "unable to open file";
		return false;
	}
	
}




