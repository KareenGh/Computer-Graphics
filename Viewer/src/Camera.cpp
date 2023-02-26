#include "Camera.h"
#include "Utils.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up, const float aspectRatio) :
	zoom(1.0f),
	fovy(glm::pi<float>() / 4.0f),
	height(5),
	zNear(0.1f),
	zFar(200.0f),
	aspectRatio(aspectRatio),
	prespective(true),
	viewTransformation(1),
	eye(eye),
	at(at),
	up(up)
{
	UpdateProjectionMatrix();
	viewTransformation = glm::lookAt(eye, at, up);
}

Camera::~Camera()
{
}

//void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
//{
//	this->eye = eye;
//	this->at = at;
//	this->up = up;
//
//	f = glm::normalize(eye - at);
//	l = glm::normalize(glm::cross(up, f));
//	u = glm::cross(f, l);
//	
//	cameraRotation[0] = glm::vec4(l, 0);
//	cameraRotation[1] = glm::vec4(u, 0);
//	cameraRotation[2] = glm::vec4(f, 0);
//	cameraRotation[3] = glm::vec4(0, 0, 0, 1);
//
//	glm::mat4x4 cameraModelRotation;
//	cameraModelRotation[0] = glm::vec4(-l, 0);
//	cameraModelRotation[1] = glm::vec4(u, 0);
//	cameraModelRotation[2] = glm::vec4(-f, 0);
//	cameraModelRotation[3] = glm::vec4(0, 0, 0, 1);
//
//	cameraInverseRotation = glm::transpose(cameraRotation);
//	cameraTranslation = Utils::TranslationMatrix(eye);
//	cameraInverseTranslation = Utils::TranslationMatrix(-eye);
//	cameraTransformation = cameraInverseRotation * cameraInverseTranslation;
//	worldTransform = cameraTranslation * cameraModelRotation * Utils::ScalingMatrix(glm::vec3(0.2,0.2,0.2));
//}

void Camera::SetOrthographicProjection(
	const float height,
	const float aspectRatio,
	const float zNear,
	const float zFar)
{
	prespective = false;
	float width = aspectRatio * height;
	projectionTransformation = glm::ortho(-width / 2, width / 2, -height / 2, height / 2, zNear, zFar);
}

void Camera::SetPerspectiveProjection(
	const float fovy,
	const float aspectRatio,
	const float zNear,
	const float zFar)
{
	prespective = true;
	projectionTransformation = glm::perspective(fovy, aspectRatio, zNear, zFar);
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projectionTransformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return viewTransformation;
}

void Camera::Zoom(const float factor)
{
	fovy = fovy * factor;
	if (fovy > glm::pi<float>())
	{
		fovy = glm::pi<float>();
	}

	UpdateProjectionMatrix();
}

void Camera::SphericalRotate(const glm::vec2& sphericalDelta)
{
	//glm::mat4x4 vAxisRotation = Utils::AxisRotationMatrix(u, sphericalDelta.x);
	//glm::mat4x4 uAxisRotation = Utils::AxisRotationMatrix(l, sphericalDelta.y);
	//eye = uAxisRotation * vAxisRotation * glm::vec4(eye,1);
	//SetCameraLookAt(eye, at, glm::vec3(0, 1, 0));
}

void Camera::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	UpdateProjectionMatrix();
}

void Camera::UpdateProjectionMatrix()
{
	if (prespective)
	{
		SetPerspectiveProjection(fovy, aspectRatio, zNear, zFar);
	}
	else
	{
		SetOrthographicProjection(height, aspectRatio, zNear, zFar);
	}
}

void Camera::SwitchToPrespective()
{
	prespective = true;
	UpdateProjectionMatrix();
}

void Camera::SwitchToOrthographic()
{
	prespective = false;
	UpdateProjectionMatrix();
}

void Camera::SetNear(const float zNear)
{
	this->zNear = zNear;
	UpdateProjectionMatrix();
}

void Camera::SetFar(const float zFar)
{
	this->zFar = zFar;
	UpdateProjectionMatrix();
}

void Camera::SetHeight(const float height)
{
	this->height = height;
	UpdateProjectionMatrix();
}

void Camera::SetFovy(const float fovy)
{
	this->fovy = fovy;
	UpdateProjectionMatrix();
}

float Camera::GetNear()
{
	return zNear;
}

float Camera::GetFar()
{
	return zFar;
}

float Camera::GetFovy()
{
	return fovy;
}

float Camera::GetHeight()
{
	return height;
}

bool Camera::IsPrespective()
{
	return prespective;
}

const glm::vec3& Camera::GetEye() const
{
	return eye;
}

//#include "Camera.h"
//#include <iostream>
//using namespace std;
//
//Camera::Camera()
//{
//	//far1 = -1, near1 = 1;
//	//fovy = 45.0f;
//	//aspectRatio = 16.0f / 9.0f;
//}
//
//Camera::~Camera()
//{
//	
//}
//
//const glm::mat4x4& Camera::GetProjectionTransformation() const
//{
//	//return projection_transformation;
//	if (orth)
//		return glm::ortho(left, right, down, up);
//	return glm::perspective(fovy,aspect,near1,far1);
//}
//
//const glm::mat4x4& Camera::GetViewTransformation() const
//{
//	return view_transformation;
//}
//
//const glm::mat4x4& Camera::GetCameraLookAt()
//{
//	return glm::lookAt(Eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
//}
////glm::mat4x4& Camera::GetCamMat() 
////{
////	glm::mat4x4 to_translate = glm::mat4x4(1);
////
////	for (int i = 0; i < 4; i++)
////	{
////		for (int j = 0; j < 4; j++)
////		{
////			cout << CamTransformate[i][j] <<"   ";
////		}
////		
////	}
////	cout << endl;
////	return GetViewTransformation()*glm::lookAt(Eye,glm::vec3{0,0,0},glm::vec3{0,1,0})* glm::inverse(CamTransformate);
////
////}
//
//void Camera::SetTransformate()
//{
//	double alfa = glm::radians(x_cam);
//	xCam_rotate[1][1] = cos(alfa);
//	xCam_rotate[2][2] = cos(alfa);
//	xCam_rotate[1][2] = sin(alfa);
//	xCam_rotate[2][1] = -sin(alfa);
//	alfa = glm::radians(y_cam);
//	yCam_rotate[0][0] = cos(alfa);
//	yCam_rotate[2][2] = cos(alfa);
//	yCam_rotate[0][2] = sin(alfa);
//	yCam_rotate[2][0] = -sin(alfa);
//	alfa = glm::radians(z_cam);
//	zCam_rotate[0][0] = cos(alfa);
//	zCam_rotate[1][1] = cos(alfa);
//	zCam_rotate[0][1] = sin(alfa);
//	zCam_rotate[1][0] = -sin(alfa);
//	alfa = glm::radians(xw_cam);
//	xwCam_rotate[1][1] = cos(alfa);
//	xwCam_rotate[2][2] = cos(alfa);
//	xwCam_rotate[1][2] = sin(alfa);
//	xwCam_rotate[2][1] = -sin(alfa);
//	alfa = glm::radians(yw_cam);
//	ywCam_rotate[0][0] = cos(alfa);
//	ywCam_rotate[2][2] = cos(alfa);
//	ywCam_rotate[0][2] = sin(alfa);
//	ywCam_rotate[2][0] = -sin(alfa);
//	alfa = glm::radians(zw_cam);
//	zwCam_rotate[0][0] = cos(alfa);
//	zwCam_rotate[1][1] = cos(alfa);
//	zwCam_rotate[0][1] = sin(alfa);
//	zwCam_rotate[1][0] = -sin(alfa);
//
//	CamTransformate = WorldTranslationCam_mat * xwCam_rotate * ywCam_rotate * zwCam_rotate * TranslationCam_mat * xCam_rotate * yCam_rotate * zCam_rotate;
////	CamTransformate = zwCam_rotate * ywCam_rotate * xwCam_rotate * WorldTranslationCam_mat * zCam_rotate * yCam_rotate * xCam_rotate * TranslationCam_mat;
//
//}
//
//glm::mat4x4 Camera::GetOrthTransformation() const
//{
//	return OrthTransformations * glm::lookAt(Eye, glm::vec3(0, 0, 0.5), glm::vec3(0, 1, 0.5)) * glm::inverse(CamTransformate);
//}
//
//void Camera::SetOrthographicProjection(float left, float right, float bottom, float top, float near1, float far1)
//{
//	OrthTransformations = glm::ortho(left, right, bottom, top);// * glm::lookAt(Eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)) * glm::inverse(CamTransformate);
//}
//
//void Camera::SetPerspectiveProjection(float fovy, float aspectRatio, float near1, float far1)
//{
//	PerspectiveTrans = glm::perspective(fovy, aspectRatio, near1, far1);
//}
//
//glm::mat4x4 Camera::GetPerspectiveProjection() const
//{
//	return PerspectiveTrans * glm::inverse(glm::lookAt(Eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))); // * glm::inverse(CamTransformate);
//}