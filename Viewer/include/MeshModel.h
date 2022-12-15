#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class MeshModel
{
public:
	int Scale_ch = 1;
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name, glm::mat4x4 Matrix);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	const glm::vec3 GetVertix(int i, int j);
	const glm::vec3 GetVertix(int i) const;
	const glm::mat4x4 GetSTMatrix() const;
	const glm::mat4x4 GetTransformMat() const;
	//int GetMax() const;
	//void PrintFaces() const;
	float x = 0, y = 0, z = 0, xw = 0, yw = 0, zw = 0;

	glm::vec3 ObjectColor = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::mat4x4 Scale_mat = glm::mat4x4(500, 0, 0, 0, 0, 500, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 Translation_mat = glm::mat4x4(1);
	glm::mat4x4 w_scale = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 w_translate = glm::mat4x4(1);
	glm::mat4x4 x_rotate = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 y_rotate = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 z_rotate = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 xw_rotate = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 yw_rotate = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 zw_rotate = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 w_move = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 Transformate = glm::mat4x4(1);
	void SetTransformate();
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
private:
	std::vector<Face> faces;
	std::string model_name;

	glm::mat4x4 STMatrix;
	glm::mat4x4 TransformMat;
};