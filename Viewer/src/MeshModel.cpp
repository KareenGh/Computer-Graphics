#define _USE_MATH_DEFINES
#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>


MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& modelName) :
	modelTransform(1),
	worldTransform(1),
	modelName(modelName)
{
	worldTransform = glm::mat4x4(1);
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0, 1);
	color = glm::vec3(dist(mt), dist(mt), dist(mt));

	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;

			Vertex vertex;
			vertex.position = vertices[vertexIndex];
			vertex.normal = normals[vertexIndex];

			if (textureCoords.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				vertex.textureCoords = textureCoords[textureCoordsIndex];
			}

			modelVertices.push_back(vertex);
		}
	}

	//PlaneMap();
	//CylinderMap();
	//SpherMap();

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}

MeshModel::~MeshModel()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

const glm::vec3& MeshModel::GetColor() const
{
	return color;
}

void MeshModel::SetColor(const glm::vec3& color)
{
	this->color = color;
}

const std::string& MeshModel::GetModelName()
{
	return modelName;
}

void MeshModel::SetWorldTransformation(const glm::mat4x4& worldTransform)
{
	this->worldTransform = worldTransform;
}

const glm::mat4x4& MeshModel::GetWorldTransformation() const
{
	return worldTransform;
}

void MeshModel::SetModelTransformation(const glm::mat4x4& worldTransform)
{
	this->modelTransform = worldTransform;
}

const glm::mat4x4& MeshModel::GetModelTransformation() const
{
	return modelTransform;
}

void MeshModel::TranslateModel(const glm::vec3& translationVector)
{
	modelTransform = Utils::TranslationMatrix(translationVector) * modelTransform;
}

void MeshModel::TranslateWorld(const glm::vec3& translationVector)
{
	worldTransform = Utils::TranslationMatrix(translationVector) * worldTransform;
}

void MeshModel::RotateXModel(double angle)
{
	modelTransform = Utils::XRotationMatrix(angle) * modelTransform;
}

void MeshModel::RotateYModel(double angle)
{
	modelTransform = Utils::YRotationMatrix(angle) * modelTransform;
}

void MeshModel::RotateZModel(double angle)
{
	modelTransform = Utils::ZRotationMatrix(angle) * modelTransform;
}

void MeshModel::ScaleXModel(double factor)
{
	modelTransform = Utils::XScalingMatrix(factor) * modelTransform;
}

void MeshModel::ScaleYModel(double factor)
{
	modelTransform = Utils::YScalingMatrix(factor) * modelTransform;
}

void MeshModel::ScaleZModel(double factor)
{
	modelTransform = Utils::ZScalingMatrix(factor) * modelTransform;
}

void MeshModel::ScaleModel(double factor)
{
	modelTransform = glm::scale(glm::mat4(1), glm::vec3(factor, factor, factor)) * modelTransform;
}

void MeshModel::RotateXWorld(double angle)
{
	worldTransform = Utils::XRotationMatrix(angle) * worldTransform;
}

void MeshModel::RotateYWorld(double angle)
{
	worldTransform = Utils::YRotationMatrix(angle) * worldTransform;
}

void MeshModel::RotateZWorld(double angle)
{
	worldTransform = Utils::ZRotationMatrix(angle) * worldTransform;
}

void MeshModel::ScaleXWorld(double factor)
{
	worldTransform = Utils::XScalingMatrix(factor) * worldTransform;
}

void MeshModel::ScaleYWorld(double factor)
{
	worldTransform = Utils::YScalingMatrix(factor) * worldTransform;
}

void MeshModel::ScaleZWorld(double factor)
{
	worldTransform = Utils::ZScalingMatrix(factor) * worldTransform;
}

void MeshModel::ScaleWorld(double factor)
{
	worldTransform = glm::scale(glm::mat4(1), glm::vec3(factor, factor, factor)) * worldTransform;
}



GLuint MeshModel::GetVAO() const
{
	return vao;
}

GLuint MeshModel::GetVBO() const
{
	return vbo;
}

const std::vector<Vertex>& MeshModel::GetModelVertices()
{
	return modelVertices;
}

void MeshModel::PlaneMap()
{
	/*  drop z coord */
	for (Vertex& vertex : modelVertices) {
		float u = vertex.position[0];
		float v = vertex.position[1];
		vertex.textureCoords = glm::vec2(u, v);
	}
	glBindVertexArray(GetVAO());
	glBindBuffer(GL_VERTEX_ARRAY, GetVBO());
	glBufferSubData(GL_ARRAY_BUFFER, 0, modelVertices.size() * sizeof(Vertex), &modelVertices[0]);
	glBindVertexArray(0);
}

void MeshModel::CylinderMap()
{
	/* Given a point (x,y,z), convert it to cylindrical coordinates (r, theta, z) and use (theta,z) as the 2D texture coordinates */
	for (Vertex& vertex : modelVertices) {
		float x = vertex.position[0];
		float y = vertex.position[1];
		float z = vertex.position[2];
		float u = std::atan2(x, y);
		float v = std::atan2(x, z);
		vertex.textureCoords = glm::vec2(u, v);

		//float x = vertex.position[0];
		//float y = vertex.position[1];
		//float z = vertex.position[2];
		//float r = sqrt(x * x + y * y);
		//float theta = atan2(y, x);
		//vertex.textureCoords = glm::vec2(theta, z);
	}
	glBindVertexArray(GetVAO());
	glBindBuffer(GL_VERTEX_ARRAY, GetVBO());
	glBufferSubData(GL_ARRAY_BUFFER, 0, modelVertices.size() * sizeof(Vertex), &modelVertices[0]);
	glBindVertexArray(0);
}

void MeshModel::SpherMap()
{
	/* Given a point (x,y,z), convert it to spherical coordinate coordinates (theta,phi) */
	for (Vertex& vertex : modelVertices) {
		vertex.textureCoords = glm::vec2(vertex.position[0] / (1 - vertex.position[2]), vertex.position[1] / (1 - vertex.position[2]));
	}
	glBindVertexArray(GetVAO());
	glBindBuffer(GL_VERTEX_ARRAY, GetVBO());
	glBufferSubData(GL_ARRAY_BUFFER, 0, modelVertices.size() * sizeof(Vertex), &modelVertices[0]);
	glBindVertexArray(0);
}

void MeshModel::LoadTextures(const char* path)
{
	if (!(Texture.loadTexture(path, true)))
		Texture.loadTexture(path, true);

}

void MeshModel::LoadNormalMap(const char* path)
{
	if (!(NormalMap.loadTexture(path, true)))
		NormalMap.loadTexture(path, true);
}


////Responsible about rotation calculating
//void MeshModel::SetTransformate()
//{
//	double alfa = glm::radians(x);
//	x_rotate[1][1] = cos(alfa);
//	x_rotate[2][2] = cos(alfa);
//	x_rotate[1][2] = sin(alfa);
//	x_rotate[2][1] = -sin(alfa);
//	alfa = glm::radians(y);
//	y_rotate[0][0] = cos(alfa);
//	y_rotate[2][2] = cos(alfa);
//	y_rotate[0][2] = sin(alfa);
//	y_rotate[2][0] = -sin(alfa);
//	alfa = glm::radians(z);
//	z_rotate[0][0] = cos(alfa);
//	z_rotate[1][1] = cos(alfa);
//	z_rotate[0][1] = sin(alfa);
//	z_rotate[1][0] = -sin(alfa);
//	alfa = glm::radians(xw);
//	xw_rotate[1][1] = cos(alfa);
//	xw_rotate[2][2] = cos(alfa);
//	xw_rotate[1][2] = sin(alfa);
//	xw_rotate[2][1] = -sin(alfa);
//	alfa = glm::radians(yw);
//	yw_rotate[0][0] = cos(alfa);
//	yw_rotate[2][2] = cos(alfa);
//	yw_rotate[0][2] = sin(alfa);
//	yw_rotate[2][0] = -sin(alfa);
//	alfa = glm::radians(zw);
//	zw_rotate[0][0] = cos(alfa);
//	zw_rotate[1][1] = cos(alfa);
//	zw_rotate[0][1] = sin(alfa);
//	zw_rotate[1][0] = -sin(alfa);
//
//	//modelTransform = w_translate * w_scale * xw_rotate * yw_rotate * zw_rotate * Translation_mat * x_rotate * y_rotate * z_rotate * Scale_mat;
//	//Axis_World_Trans = w_translate * w_scale * xw_rotate * yw_rotate * zw_rotate *Translation_mat* Scale_mat;
//	worldTransform = w_translate * w_scale * xw_rotate * yw_rotate * zw_rotate * Scale_mat;
//	modelTransform = w_translate * w_scale * x_rotate * y_rotate * z_rotate * Scale_mat;
//}
//#include "MeshModel.h"
//#include <iostream>
//
//using namespace std;
//
////MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
////	faces(faces),
////	vertices(vertices),
////	normals(normals),
////	model_name(model_name)
////{
////	//{
////	//	for (int i = 0; i < vertices.size(); i++)
////	//	{
////	//		cout << "V " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << endl;
////	//	}
////	//	for (int i = 0; i < faces.size(); i++)
////	//	{
////	//		cout << "F " << faces[i].GetVertexIndex(0) << "\\" << faces[i].GetNormalIndex(0) << " " << faces[i].GetVertexIndex(1) << "\\" << faces[i].GetNormalIndex(1) << " " << faces[i].GetVertexIndex(2) << "\\" << faces[i].GetNormalIndex(2) << endl;
////	//	}
////	//}
////}
//
//MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name, glm::mat4x4 Matrix) :
//	faces(faces),
//	vertices(vertices),
//	normals(normals),
//	model_name(model_name),
//	STMatrix(Matrix),
//	TransformMat(glm::mat4x4(1.0f))
//{
//	Find_Max_Min_BB();
//	//Translation_mat[3][2] = -30;
//}
//
//MeshModel::~MeshModel()
//{
//}
//
//const Face& MeshModel::GetFace(int index) const
//{
//	return faces[index];
//}
//
//int MeshModel::GetFacesCount() const
//{
//	return faces.size();
//}
//
//const std::string& MeshModel::GetModelName() const
//{
//	return model_name;
//}
//
//const glm::vec3 MeshModel::GetVertix(int i, int j)
//{
//	return vertices[faces[i].GetVertexIndex(j) - 1];
//}
//
//const glm::vec3 MeshModel::GetVertix(int i) const
//{
//	return vertices[i - 1];
//}
//
//const glm::mat4x4 MeshModel::GetSTMatrix() const
//{
//	return STMatrix;
//}
//
//const glm::mat4x4 MeshModel::GetTransformMat() const {
//	return TransformMat;
//}
//
//void MeshModel::Find_Max_Min_BB()
//{
//	glm::vec3 ModelVer = vertices[0];
//	min_x = ModelVer[0], max_x = ModelVer[0], min_y = ModelVer[1], max_y = ModelVer[1], min_z = ModelVer[2], max_z = ModelVer[2];
//	for (int i = 0; i < faces.size(); i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			ModelVer = vertices[faces[i].GetVertexIndex(j) - 1];
//			if (min_x > ModelVer[0])
//				min_x = ModelVer[0];
//			if (max_x < ModelVer[0])
//				max_x = ModelVer[0];
//			if (min_y > ModelVer[1])
//				min_y = ModelVer[1];
//			if (max_y < ModelVer[1])
//				max_y = ModelVer[1];
//			if (min_z > ModelVer[2])
//				min_z = ModelVer[2];
//			if (max_z < ModelVer[2])
//				max_z = ModelVer[2];
//
//		}
//	}
//}
//
////void MeshModel::SetTransforMat()
////{
////	float alfa = glm::radians(x);
//	
//	
////}
//
////int MeshModel::GetMax() const
////{
////
////}
//
////Responsible about rotation calculating
//void MeshModel::SetTransformate()
//{
//	double alfa = glm::radians(x);
//	x_rotate[1][1] = cos(alfa);
//	x_rotate[2][2] = cos(alfa);
//	x_rotate[1][2] = sin(alfa);
//	x_rotate[2][1] = -sin(alfa);
//	alfa = glm::radians(y);
//	y_rotate[0][0] = cos(alfa);
//	y_rotate[2][2] = cos(alfa);
//	y_rotate[0][2] = sin(alfa);
//	y_rotate[2][0] = -sin(alfa);
//	alfa = glm::radians(z);
//	z_rotate[0][0] = cos(alfa);
//	z_rotate[1][1] = cos(alfa);
//	z_rotate[0][1] = sin(alfa);
//	z_rotate[1][0] = -sin(alfa);
//	alfa = glm::radians(xw);
//	xw_rotate[1][1] = cos(alfa);
//	xw_rotate[2][2] = cos(alfa);
//	xw_rotate[1][2] = sin(alfa);
//	xw_rotate[2][1] = -sin(alfa);
//	alfa = glm::radians(yw);
//	yw_rotate[0][0] = cos(alfa);
//	yw_rotate[2][2] = cos(alfa);
//	yw_rotate[0][2] = sin(alfa);
//	yw_rotate[2][0] = -sin(alfa);
//	alfa = glm::radians(zw);
//	zw_rotate[0][0] = cos(alfa);
//	zw_rotate[1][1] = cos(alfa);
//	zw_rotate[0][1] = sin(alfa);
//	zw_rotate[1][0] = -sin(alfa);
//
//	Transformate = w_translate * w_scale * xw_rotate * yw_rotate * zw_rotate * Translation_mat * x_rotate * y_rotate * z_rotate * Scale_mat;
//	//Axis_World_Trans = w_translate * w_scale * xw_rotate * yw_rotate * zw_rotate *Translation_mat* Scale_mat;
//	Axis_World_Trans = w_translate * w_scale * xw_rotate * yw_rotate * zw_rotate * Scale_mat;
//}
//
//void MeshModel::SetTriColors(int color)
//{
//	colors = color; // 1 -> colors			2 -> gray
//}
//
//int MeshModel::GetTriColors() const
//{
//	return colors;
//}