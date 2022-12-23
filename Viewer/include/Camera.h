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
	bool WorldAxis = false; //
	bool Orthograhic = false; //
	bool Perspective = false; //
	glm::vec3 Eye = glm::vec3(0, 0, 10);
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
	glm::mat4x4 PerspectiveTrans = glm::mat4x4(1);

	float up = 1, down = -1, left = -1, right = 1,far1=100,near1=0.0001;
	//float fovy=0.3 ,aspect =1 ;
	float fovy = 45.0f;
	float aspectRatio = 800.0f / 600.0f; //1920 / 1080; //4.0f / 3.0f; //1280.0f / 720.0f;

	void SetTransformate();
	glm::mat4x4 GetOrthTransformation() const; //
	glm::mat4x4 GetPerspectiveProjection() const;

	void SetOrthographicProjection(float left, float right, float bottom, float top, float near1, float far1); //
	void SetPerspectiveProjection(float fovy, float aspectRatio, float near1, float far1);


private:
	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;

};
