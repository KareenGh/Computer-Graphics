#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"
#include "Light.h"

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
	void MeshModel::Find_Max_Min_BB();
	//int GetMax() const;
	//void PrintFaces() const;
	float x = 0, y = 0, z = 0, xw = 0, yw = 0, zw = 0;
	float max_x, max_y, max_z, min_x, min_y, min_z;
	bool bounding_box = false;
	bool face_normals = false, vertex_normals = false;
	bool ModelAxis = false; //
	bool bounding_rectangle = false;
	glm::vec3 ObjectColor = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::mat4x4 Scale_mat = glm::mat4x4(200, 0, 0, 0, 0, 200, 0, 0, 0, 0, 200, 0, 0, 0, 0, 1);
	glm::mat4x4 Translation_mat = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
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
	glm::mat4x4 Axis_World_Trans = glm::mat4x4(1);
	glm::mat4x4 Axis_Model_Trans = glm::mat4x4(1);
	void SetTransformate();
	void SetTriColors(int color); ///
	int GetTriColors() const; ///
	int colors = 0; ///
	bool ZBufferOn = 0; ///
	float alfa = 1;
	glm::vec3 Ambient_ref=glm::vec3(1);
	glm::vec3 Diffuse_ref= glm::vec3(1);
	glm::vec3 Specular_ref= glm::vec3(1);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
private:
	std::vector<Face> faces;
	std::string model_name;

	glm::mat4x4 STMatrix;
	glm::mat4x4 TransformMat;
};