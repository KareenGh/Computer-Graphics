#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;
	//glm::mat4x4& GetCamMat();
	const glm::mat4x4& GetCameraLookAt();
	float x_cam = 0, y_cam = 0, z_cam = 0, xw_cam = 0, yw_cam = 0, zw_cam = 0;
	bool orth = true;
	glm::vec3 Eye = glm::vec3(0, 0, 1);
	glm::mat4x4 TranslationCam_mat = glm::mat4x4(1);
	glm::mat4x4 WorldTranslationCam_mat = glm::mat4x4(1);
	glm::mat4x4 xCam_rotate = glm::mat4x4(1);
	glm::mat4x4 yCam_rotate = glm::mat4x4(1);
	glm::mat4x4 zCam_rotate = glm::mat4x4(1);
	glm::mat4x4 xwCam_rotate = glm::mat4x4(1);
	glm::mat4x4 ywCam_rotate = glm::mat4x4(1);
	glm::mat4x4 zwCam_rotate = glm::mat4x4(1);
	glm::mat4x4 CamTransformate = glm::mat4x4(1);
	glm::mat4x4 OrthTransformations = glm::mat4x4(1);
	float up = 1, down = -1, left = -1, right = 1,far1=-1,near1=1;
	float fovy=glm::radians(180.0f), aspect = 2;
	void SetTransformate();
private:
	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;

};
