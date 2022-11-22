 #include "MeshModel.h"
#include <iostream>

using namespace std;

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	model_name(model_name)
{
	{
		for (int i = 0; i < vertices.size(); i++)
		{
			cout << "V " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << endl;
		}
		for (int i = 0; i < faces.size(); i++)
		{
			cout << "F " << faces[i].GetVertexIndex(0) << "\\" << faces[i].GetNormalIndex(0) << " " << faces[i].GetVertexIndex(1) << "\\" << faces[i].GetNormalIndex(1) << " " << faces[i].GetVertexIndex(2) << "\\" << faces[i].GetNormalIndex(2) << endl;
		}
	}
}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}

glm::vec3 MeshModel::GetVertix(int i, int j)
{
	return vertices[faces[i].GetVertexIndex(j) - 1];
}