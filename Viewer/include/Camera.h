#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"
/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 */
class Camera
{
private:
	glm::mat4x4 viewTransformation;
	glm::mat4x4 projectionTransformation;

	glm::vec3 eye;
	glm::vec3 up;
	glm::vec3 at;

	glm::vec3 x;
	glm::vec3 y;
	glm::vec3 z;

	float zoom;
	float fovy;
	float height;
	float zNear;
	float zFar;
	float aspectRatio;

	bool prespective;

public:
	Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up, const float aspectRatio);
	~Camera();

	void SetOrthographicProjection(
		const float height,
		const float aspectRatio,
		const float zNear,
		const float zFar);

	void SetPerspectiveProjection(
		const float fovy,
		const float aspect,
		const float zNear,
		const float zFar);

	void UpdateProjectionMatrix();

	void SetNear(const float zNear);

	void SetFar(const float zFar);

	void SetFovy(const float fovy);

	void SetHeight(const float height);

	void Zoom(const float factor);

	void SphericalRotate(const glm::vec2& sphericalDelta);

	const glm::mat4x4& GetProjectionTransformation() const;

	const glm::mat4x4& GetViewTransformation() const;

	void SetAspectRatio(float aspectRatio);

	void SwitchToPrespective();
	void SwitchToOrthographic();

	float GetNear();

	float GetFar();

	float GetFovy();

	float GetHeight();

	bool IsPrespective();

	const glm::vec3& GetEye() const;
};


//#pragma once
//#include <glm/glm.hpp>
//#include <glm/gtx/transform.hpp>
//
//class Camera
//{
//public:
//	Camera();
//	virtual ~Camera();
//
//	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
//
//	const glm::mat4x4& GetProjectionTransformation() const;
//	const glm::mat4x4& GetViewTransformation() const;
//	//glm::mat4x4& GetCamMat();
//	const glm::mat4x4& GetCameraLookAt();
//	float x_cam = 0, y_cam = 0, z_cam = 0, xw_cam = 0, yw_cam = 0, zw_cam = 0;
//	bool orth = false;
//	bool Perspective = false;
//	bool WorldAxis = false;
//	glm::vec3 Eye = glm::vec3(0, 0, 10);
//	glm::mat4x4 TranslationCam_mat = glm::mat4x4(1);
//	glm::mat4x4 WorldTranslationCam_mat = glm::mat4x4(1);
//	glm::mat4x4 xCam_rotate = glm::mat4x4(1);
//	glm::mat4x4 yCam_rotate = glm::mat4x4(1);
//	glm::mat4x4 zCam_rotate = glm::mat4x4(1);
//	glm::mat4x4 xwCam_rotate = glm::mat4x4(1);
//	glm::mat4x4 ywCam_rotate = glm::mat4x4(1);
//	glm::mat4x4 zwCam_rotate = glm::mat4x4(1);
//	glm::mat4x4 CamTransformate = glm::mat4x4(1);
//	glm::mat4x4 OrthTransformations = glm::mat4x4(1);
//	glm::mat4x4 PerspectiveTrans = glm::mat4x4(1);
//	float up = 1, down = -1, left = -1, right = 1,far1=100,near1=0.0001;
////	float fovy=glm::radians(180.0f), aspect = 2;
//	float fovy = 45.0f, aspect = 800.0f / 600.0f;
//	void SetTransformate();
//	glm::mat4x4 GetOrthTransformation() const;
//	void SetOrthographicProjection(float left, float right, float bottom, float top, float near1, float far1);
//	void SetPerspectiveProjection(float fovy, float aspectRatio, float near1, float far1);
//	glm::mat4x4 GetPerspectiveProjection() const;
//private:
//	glm::mat4x4 view_transformation;
//	glm::mat4x4 projection_transformation;
//
//};
