#include "Mesh.h"
#include <fstream>
#include <iostream>


using namespace std;

#define CHAR_BUFFER_SIZE 128
#define BUFFER_OFFSET(i) ((char *)0 + (i))


struct MeshFace {
	MeshFace() {
		vertices[0] = 0;
		vertices[1] = 0;
		vertices[2] = 0;

		textureUVs[0] = 0;
		textureUVs[1] = 0;
		textureUVs[2] = 0;

		normals[0] = 0;
		normals[1] = 0;
		normals[2] = 0;
	}

	MeshFace(unsigned v1, unsigned v2, unsigned v3,
		unsigned t1, unsigned t2, unsigned t3,
		unsigned n1, unsigned n2, unsigned n3)
	{
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;

		textureUVs[0] = t1;
		textureUVs[1] = t2;
		textureUVs[2] = t3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;

	}
	unsigned vertices[3];
	unsigned textureUVs[3];
	unsigned normals[3];
};

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

Mesh::Mesh(const std::string & file)
{
	LoadfromFile(file);
}

bool Mesh::LoadfromFile(const std::string & file)
{
	ifstream input;
	input.open(file);

	if (!input) {
		std::cout << "File open error" << std::endl;
		return false;
	}
	char inputString[CHAR_BUFFER_SIZE];

	//unique data
	vector<glm::vec3> vertexData;
	vector<glm::vec2> textureData;
	vector<glm::vec3> normalData;
	//face data
	vector<MeshFace> faceData;
	//unpacked data


	while (!input.eof()) {
		input.getline(inputString, CHAR_BUFFER_SIZE);

		if (strstr(inputString, "#") != nullptr) {
			//a comment
			continue;
		}
		else if (strstr(inputString, "o") != nullptr || strstr(inputString, "s") != nullptr || strstr(inputString, "mtllib") != nullptr || strstr(inputString, "usemtl") != nullptr) {
			//ignores unused data
			continue;
		}
		else if (strstr(inputString, "vt") != nullptr) {
			//UV data

			glm::vec2 temp;
			sscanf_s(inputString, "vt %f %f", &temp.x, &temp.y);
			textureData.push_back(temp);

		}
		else if (strstr(inputString, "vn") != nullptr) {
			//vertex data

			glm::vec3 temp;

			sscanf_s(inputString, "vn %f %f %f", &temp.x, &temp.y, &temp.z);
			normalData.push_back(temp);
		}
		else if (strstr(inputString, "v") != nullptr) {
			//vertex data

			glm::vec3 temp;
			sscanf_s(inputString, "v %f %f %f", &temp.x, &temp.y, &temp.z);
			if (temp.x > maxSize.x) {
				maxSize.x = temp.x;
			}
			else if (temp.x < minSize.x) {
				minSize.x = temp.x;
			}
			if (temp.y > maxSize.y) {
				maxSize.y = temp.x;
			}
			else if (temp.y < minSize.y) {
				minSize.y = temp.y;
			}

			if (temp.z > maxSize.z) {
				maxSize.z = temp.z;
			}
			else if (temp.z < minSize.z) {
				minSize.z = temp.z;
			}
			vertices.push_back(temp);
			vertexData.push_back(temp);
		}
		else if (strstr(inputString, "f") != nullptr) {
			//face data

			MeshFace temp;
			sscanf_s(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u",
				&temp.vertices[0], &temp.textureUVs[0], &temp.normals[0],
				&temp.vertices[1], &temp.textureUVs[1], &temp.normals[1],
				&temp.vertices[2], &temp.textureUVs[2], &temp.normals[2]);
			faceData.push_back(temp);
		}

	}
	input.close();

	for (unsigned i = 0; i < faceData.size(); i++) {
		for (unsigned j = 0; j < 3; j++) {
			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].x);
			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].y);
			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].z);

			unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].x);
			unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].y);


			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].x);
			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].y);
			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].z);

		}
	}

	_NumFaces = faceData.size();
	_NumVertices = _NumFaces * 3;

	vertexData.clear();
	textureData.clear();
	normalData.clear();
	faceData.clear();

	BoundingBox.m_size = glm::vec3(maxSize.x - minSize.x, maxSize.y - minSize.y, maxSize.z - minSize.z);
	BoundingBox.m_center = glm::vec3((minSize.x + maxSize.x) / 2, (minSize.y + maxSize.y) / 2, (minSize.z + maxSize.z) / 2);

	return true;
}

void Mesh::Unload()
{
	_NumFaces = 0;
	_NumVertices = 0;
}

unsigned int Mesh::GetNumFaces() const
{
	return _NumFaces;
}

unsigned int Mesh::GetNumVertices() const
{
	return _NumVertices;
}
