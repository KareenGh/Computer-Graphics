#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "MeshModel.h"
//#include "AmbientLight.h"
#include "PointLight.h"
#include "Camera.h"

class Scene {
private:
	std::vector<std::shared_ptr<MeshModel>> models;
	std::vector<std::shared_ptr<PointLight>> lights;
//	AmbientLight ambientLight;
	std::vector<Camera> cameras;

	int activeCameraIndex;
	int activeModelIndex;

public:
	Scene();

	void AddModel(const std::shared_ptr<MeshModel>& model);
	int GetModelCount() const;
	std::shared_ptr<MeshModel> GetModel(int index) const;

	void AddCamera(const Camera& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index);
	const Camera& GetCamera(int index) const;

	void AddLight(const std::shared_ptr<PointLight>& light);
	int GetLightCount() const;
	std::shared_ptr<PointLight> GetLight(int index) const;
	const std::vector<std::shared_ptr<PointLight>>& GetActiveLights() const;

//	const AmbientLight& GetAmbientLight();

	const Camera& GetActiveCamera() const;
	Camera& GetActiveCamera();

	void SetActiveCameraIndex(int index);
	const int GetActiveCameraIndex() const;

	const std::shared_ptr<MeshModel>& GetActiveModel() const;

	void SetActiveModelIndex(int index);
	const int GetActiveModelIndex() const;

	
};

//#pragma once
//
//#include <vector>
//#include <memory>
//
//#include "Camera.h"
//#include "MeshModel.h"
//#include "Light.h"
//
//using namespace std;
//
//class Scene {
//public:
//	Scene();
//
//	void AddModel(const shared_ptr<MeshModel>& mesh_model);
//	int GetModelCount() const;
//	MeshModel& GetModel(int index) const;
//	MeshModel& GetActiveModel() const;
//	
//	void AddCamera(const shared_ptr<Camera>& camera);
//	int GetCameraCount() const;
//	Camera& GetCamera(int index);
//	Camera& GetActiveCamera();
//
//	void SetActiveCameraIndex(int index);
//	int GetActiveCameraIndex() const;
//
//	void SetActiveModelIndex(int index);
//	int GetActiveModelIndex() const;
//
//	vector<shared_ptr<Light>> lights;
//	shared_ptr<MeshModel> camera1;
//	int active_camera_index;
//
//
//	bool lighting;
//	bool more_than_1_light;
//
//
//private:
//	vector<shared_ptr<MeshModel>> mesh_models;
//	vector<shared_ptr<Camera>> cameras;
//
//	
//	int active_model_index;
//};