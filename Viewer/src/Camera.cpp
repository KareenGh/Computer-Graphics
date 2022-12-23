#include "Camera.h"
#include <iostream>
using namespace std;

Camera::Camera()
{
	//far1 = -1, near1 = 1;
	//fovy = 45.0f;
	//aspectRatio = 16.0f / 9.0f;
}

Camera::~Camera()
{
	
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	//return projection_transformation;
	if (orth)
		return glm::ortho(left, right, down, up);
	//return glm::perspective(fovy,aspect,near1,far1);
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation;
}

const glm::mat4x4& Camera::GetCameraLookAt()
{
	return glm::lookAt(Eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}
//glm::mat4x4& Camera::GetCamMat() 
//{
//	glm::mat4x4 to_translate = glm::mat4x4(1);
//
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			cout << CamTransformate[i][j] <<"   ";
//		}
//		
//	}
//	cout << endl;
//	return GetViewTransformation()*glm::lookAt(Eye,glm::vec3{0,0,0},glm::vec3{0,1,0})* glm::inverse(CamTransformate);
//
//}

void Camera::SetTransformate()
{
	double alfa = glm::radians(x_cam);
	xCam_rotate[1][1] = cos(alfa);
	xCam_rotate[2][2] = cos(alfa);
	xCam_rotate[1][2] = sin(alfa);
	xCam_rotate[2][1] = -sin(alfa);
	alfa = glm::radians(y_cam);
	yCam_rotate[0][0] = cos(alfa);
	yCam_rotate[2][2] = cos(alfa);
	yCam_rotate[0][2] = sin(alfa);
	yCam_rotate[2][0] = -sin(alfa);
	alfa = glm::radians(z_cam);
	zCam_rotate[0][0] = cos(alfa);
	zCam_rotate[1][1] = cos(alfa);
	zCam_rotate[0][1] = sin(alfa);
	zCam_rotate[1][0] = -sin(alfa);
	alfa = glm::radians(xw_cam);
	xwCam_rotate[1][1] = cos(alfa);
	xwCam_rotate[2][2] = cos(alfa);
	xwCam_rotate[1][2] = sin(alfa);
	xwCam_rotate[2][1] = -sin(alfa);
	alfa = glm::radians(yw_cam);
	ywCam_rotate[0][0] = cos(alfa);
	ywCam_rotate[2][2] = cos(alfa);
	ywCam_rotate[0][2] = sin(alfa);
	ywCam_rotate[2][0] = -sin(alfa);
	alfa = glm::radians(zw_cam);
	zwCam_rotate[0][0] = cos(alfa);
	zwCam_rotate[1][1] = cos(alfa);
	zwCam_rotate[0][1] = sin(alfa);
	zwCam_rotate[1][0] = -sin(alfa);

	CamTransformate = WorldTranslationCam_mat * xwCam_rotate * ywCam_rotate * zwCam_rotate * TranslationCam_mat * xCam_rotate * yCam_rotate * zCam_rotate;
//	CamTransformate = zwCam_rotate * ywCam_rotate * xwCam_rotate * WorldTranslationCam_mat * zCam_rotate * yCam_rotate * xCam_rotate * TranslationCam_mat;

}

glm::mat4x4 Camera::GetOrthTransformation() const
{
	return OrthTransformations * glm::lookAt(Eye, glm::vec3(0, 0, 0.5), glm::vec3(0, 1, 0.5)) * glm::inverse(CamTransformate);;
}

void Camera::SetOrthographicProjection(float left, float right, float bottom, float top, float near1, float far1)
{
	OrthTransformations = glm::ortho(left, right, bottom, top);// * glm::lookAt(Eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)) * glm::inverse(CamTransformate);
}

void Camera::SetPerspectiveProjection(float fovy, float aspectRatio, float near1, float far1)
{
	PerspectiveTrans = glm::perspective(fovy, aspectRatio, near1, far1);
}

glm::mat4x4 Camera::GetPerspectiveProjection() const
{
	return PerspectiveTrans * glm::inverse(glm::lookAt(Eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))); // * glm::inverse(CamTransformate);
}