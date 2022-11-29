#include "MeshModel.h"
#include <iostream>

using namespace std;

//MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
//	faces(faces),
//	vertices(vertices),
//	normals(normals),
//	model_name(model_name)
//{
//	//{
//	//	for (int i = 0; i < vertices.size(); i++)
//	//	{
//	//		cout << "V " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << endl;
//	//	}
//	//	for (int i = 0; i < faces.size(); i++)
//	//	{
//	//		cout << "F " << faces[i].GetVertexIndex(0) << "\\" << faces[i].GetNormalIndex(0) << " " << faces[i].GetVertexIndex(1) << "\\" << faces[i].GetNormalIndex(1) << " " << faces[i].GetVertexIndex(2) << "\\" << faces[i].GetNormalIndex(2) << endl;
//	//	}
//	//}
//}

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name, glm::mat4x4 Matrix) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	model_name(model_name),
	STMatrix(Matrix),
	TransformMat(glm::mat4x4(1.0f))
{

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

const glm::vec3 MeshModel::GetVertix(int i, int j)
{
	return vertices[faces[i].GetVertexIndex(j) - 1];
}

const glm::vec3 MeshModel::GetVertix(int i) const
{
	return vertices[i - 1];
}

const glm::mat4x4 MeshModel::GetSTMatrix() const
{
	return STMatrix;
}

const glm::mat4x4 MeshModel::GetTransformMat() const {
	return TransformMat;
}

//void MeshModel::SetTransforMat()
//{
//	float alfa = glm::radians(x);
	
	
//}

//int MeshModel::GetMax() const
//{
//
//}

//Responsible about rotation calculating
void MeshModel::SetTransformate()
{
	double alfa = glm::radians(x);
	x_rotate[1][1] = cos(alfa);
	x_rotate[2][2] = cos(alfa);
	x_rotate[1][2] = sin(alfa);
	x_rotate[2][1] = -sin(alfa);
	alfa = glm::radians(y);
	y_rotate[0][0] = cos(alfa);
	y_rotate[2][2] = cos(alfa);
	y_rotate[0][2] = sin(alfa);
	y_rotate[2][0] = -sin(alfa);
	alfa = glm::radians(z);
	z_rotate[0][0] = cos(alfa);
	z_rotate[1][1] = cos(alfa);
	z_rotate[0][1] = sin(alfa);
	z_rotate[1][0] = -sin(alfa);
	alfa = glm::radians(xw);
	xw_rotate[1][1] = cos(alfa);
	xw_rotate[2][2] = cos(alfa);
	xw_rotate[1][2] = sin(alfa);
	xw_rotate[2][1] = -sin(alfa);
	alfa = glm::radians(yw);
	yw_rotate[0][0] = cos(alfa);
	yw_rotate[2][2] = cos(alfa);
	yw_rotate[0][2] = sin(alfa);
	yw_rotate[2][0] = -sin(alfa);
	alfa = glm::radians(zw);
	zw_rotate[0][0] = cos(alfa);
	zw_rotate[1][1] = cos(alfa);
	zw_rotate[0][1] = sin(alfa);
	zw_rotate[1][0] = -sin(alfa);

	Transformate = w_translate * w_scale * xw_rotate * yw_rotate * zw_rotate * Translation_mat * x_rotate * y_rotate * z_rotate * Scale_mat;
}