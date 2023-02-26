#define _USE_MATH_DEFINES
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <nfd.h>

#include <stdio.h>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <sstream>
#include <stdlib.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
//#include "AmbientLight.h"
#include "PointLight.h"
#include "Utils.h"

double zoomFactor = 1;
int windowWidth = 1280;
int windowHeight = 720;
char* windowTitle = "OpenGL Demo";
glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
bool zoomChanged = false;
std::shared_ptr<Scene> scene;

ImGuiIO* imgui;
GLFWwindow* window;

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupImgui(GLFWwindow* window);
bool Setup(int windowWidth, int windowHeight, const char* windowName);
void Cleanup();

static void GlfwErrorCallback(int error, const char* description);
void RenderFrame(GLFWwindow* window, std::shared_ptr<Scene> scene, Renderer& renderer, ImGuiIO& io);

void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height);

float GetAspectRatio();
void DrawImguiMenus();
void HandleImguiInput();

int main(int argc, char** argv)
{

	if (!Setup(windowWidth, windowHeight, windowTitle))
	{
		std::cerr << "Setup failed" << std::endl;
		return -1;
	}

	scene = std::make_shared<Scene>();
	glm::vec3 eye = glm::vec3(0, 0, 10);
	glm::vec3 at = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	Camera camera = Camera(eye, at, up, GetAspectRatio());
	scene->AddCamera(camera);

	//scene->AddLight(std::make_shared<PointLight>(glm::vec3(0, 0, 15), glm::vec3(1, 1, 1)));
	//scene->AddLight(std::make_shared<PointLight>(glm::vec3(0, 5, 5), glm::vec3(0, 0, 0)));
	//scene->AddLight(std::make_shared<PointLight>(glm::vec3(-5, 0, 0), glm::vec3(0, 0, 0)));

	Renderer renderer;
	renderer.LoadShaders();
	renderer.LoadTextures();

	while (!glfwWindowShouldClose(window))
	{
		// Poll and process events
		glfwPollEvents();

		// Imgui stuff
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		DrawImguiMenus();
		ImGui::Render();
		HandleImguiInput();

		// Clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render scene
		renderer.Render(scene);

		// Imgui stuff
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool Setup(int windowWidth, int windowHeight, const char* windowName)
{
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, windowName);
	if (!window)
	{
		std::cerr << "Window setup failed" << std::endl;
		return false;
	}

	imgui = &SetupImgui(window);

	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glEnable(GL_DEPTH_TEST);

	return true;
}

GLFWwindow* SetupGlfwWindow(int windowWidth, int windowHeight, const char* windowName)
{
	// Intialize GLFW
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create an OpenGL 3.3 core, forward compatible context window
	window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(window);

	// Setup window events callbacks
	glfwSetFramebufferSizeCallback(window, glfw_OnFramebufferSize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// An error occured
		std::cerr << "GLAD initialization failed" << std::endl;
		return false;
	}

	return window;
}

ImGuiIO& SetupImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Setup style
	ImGui::StyleColorsDark();

	glfwSetScrollCallback(window, glfw_OnMouseScroll);

	return io;
}

void HandleImguiInput()
{
	if (!imgui->WantCaptureKeyboard)
	{
		if (imgui->KeysDown[49]) // 1
		{
			scene->GetActiveModel()->RotateXModel(M_PI / 200);
		}

		if (imgui->KeysDown[50]) // 2
		{
			scene->GetActiveModel()->RotateXModel(-M_PI / 200);
		}

		if (imgui->KeysDown[51]) // 3
		{
			scene->GetActiveModel()->RotateYModel(M_PI / 200);
		}

		if (imgui->KeysDown[52]) // 4
		{
			scene->GetActiveModel()->RotateYModel(-M_PI / 200);
		}

		if (imgui->KeysDown[53]) // 5
		{
			scene->GetActiveModel()->RotateZModel(M_PI / 200);
		}

		if (imgui->KeysDown[54]) // 6
		{
			scene->GetActiveModel()->RotateZModel(-M_PI / 200);
		}

		if (imgui->KeysDown[45]) // -
		{
			scene->GetActiveModel()->ScaleModel(1 / 1.01);
		}

		if (imgui->KeysDown[61]) // +
		{
			scene->GetActiveModel()->ScaleModel(1.01);
		}

		if (imgui->KeysDown[65]) // a
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(-0.02, 0, 0));
		}

		if (imgui->KeysDown[68]) // d
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(0.02, 0, 0));
		}

		if (imgui->KeysDown[83]) // s
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(0, 0, 0.02));
		}

		if (imgui->KeysDown[87]) // w
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(0, 0, -0.02));
		}
	}

	if (!imgui->WantCaptureMouse)
	{
		if (zoomChanged)
		{
			scene->GetActiveCamera().Zoom(zoomFactor);
			zoomChanged = false;
		}
	}
}

void Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, windowWidth, windowHeight);
	scene->GetActiveCamera().SetAspectRatio(GetAspectRatio());
}

void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	zoomFactor = glm::pow(1.1, -yoffset);
	zoomChanged = true;
}

float GetAspectRatio()
{
	return static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
}

void DrawImguiMenus()
{
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
	{
		ImGui::Begin("Scene Menu");

		/* Menu */
		{
			ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open", "CTRL+O"))
					{
						nfdchar_t* outPath = NULL;
						nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
						if (result == NFD_OKAY) {
							scene->AddModel(Utils::LoadMeshModel(outPath));
							free(outPath);
						}
						else if (result == NFD_CANCEL) {
						}
						else {
						}

					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
		}

		if (ImGui::ColorEdit3("Clear Color", (float*)&clearColor))
		{
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		}

		if (ImGui::CollapsingHeader("Cameras"))
		{
			if (ImGui::Button("Add Camera"))
			{

				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_real_distribution<double> dist(0, 2 * M_PI);
				std::uniform_real_distribution<double> dist2(2, 10);
				double angle = dist(mt);
				double radius = dist2(mt);

				glm::vec4 eye = glm::vec4(radius * glm::cos(angle), 0, radius * glm::sin(angle), 1);
				glm::vec4 at = glm::vec4(0, 0, 0, 1);
				glm::vec4 up = glm::vec4(0, 1, 0, 1);
				scene->AddCamera(Camera(-scene->GetActiveCamera().GetEye(), at, up, GetAspectRatio()));
			}

			const char** items;
			std::vector<std::string> cameraStrings;
			items = new const char* [scene->GetCameraCount()];
			for (int i = 0; i < scene->GetCameraCount(); i++)
			{
				std::ostringstream s;
				s << "Camera " << i;
				std::string mystring = s.str();
				cameraStrings.push_back(mystring);
			}

			for (int i = 0; i < scene->GetCameraCount(); i++)
			{
				items[i] = cameraStrings[i].c_str();
			}

			int currentCamera = scene->GetActiveCameraIndex();
			ImGui::Combo("Active Camera", &currentCamera, items, scene->GetCameraCount());

			if (currentCamera != scene->GetActiveCameraIndex())
			{
				scene->SetActiveCameraIndex(currentCamera);
				scene->GetActiveCamera().SetAspectRatio(GetAspectRatio());
			}

			delete items;

			int newProjectionType = scene->GetActiveCamera().IsPrespective() ? 0 : 1;
			ImGui::RadioButton("Prespective", &newProjectionType, 0);
			ImGui::RadioButton("Orthographic", &newProjectionType, 1);

			if (newProjectionType == 0)
			{
				float fovy;
				float zNear;
				float zFar;

				scene->GetActiveCamera().SwitchToPrespective();

				if (ImGui::SliderFloat("Fovy", &fovy, 0.0f, M_PI))
				{
					scene->GetActiveCamera().SetFovy(fovy);
				}

				if (ImGui::SliderFloat("Near", &zNear, 1.0f, 10.0f))
				{
					scene->GetActiveCamera().SetNear(zNear);
				}

				if (ImGui::SliderFloat("Far", &zFar, 1.0f, 10.0f))
				{
					scene->GetActiveCamera().SetFar(zFar);
				}
			}
			else if (newProjectionType == 1)
			{
				float height;
				float zNear;
				float zFar;

				scene->GetActiveCamera().SwitchToOrthographic();

				if (ImGui::SliderFloat("Height", &height, 0.0f, M_PI))
				{
					scene->GetActiveCamera().SetHeight(height);
				}

				if (ImGui::SliderFloat("Near", &zNear, 1.0f, 10.0f))
				{
					scene->GetActiveCamera().SetNear(zNear);
				}

				if (ImGui::SliderFloat("Far", &zFar, 1.0f, 10.0f))
				{
					scene->GetActiveCamera().SetFar(zFar);
				}
			}
		}

		if (ImGui::CollapsingHeader("Models"))
		{
			if (scene->GetModelCount() > 0)
			{
				/* world */
				static int models_num = 1;
				ImGui::ColorEdit3("Material_Ambient_Reflection", (float*)&scene->GetModel(models_num - 1)->Ambient_ref);
				ImGui::ColorEdit3("Material_Diffuse_Reflection", (float*)&scene->GetModel(models_num-1)->Diffuse_ref);
				ImGui::ColorEdit3("Material_Specular_Reflection", (float*)&scene->GetModel(models_num-1)->Specular_ref);
				static float XTranslate = 0;
				static float YTranslate = 0;
				static float ZTranslate = 0;
				glm::vec3 TranslationVector = glm::vec3(0.0f);
				static float XRotate = 0;
				static float YRotate = 0;
				static float ZRotate = 0;
				static float XScale = 1;
				static float YScale = 1;
				static float ZScale = 1;
				static bool inWorld = false;

				const char** items;
				std::vector<std::string> modelStrings;
				items = new const char* [scene->GetModelCount()];
				for (int i = 0; i < scene->GetModelCount(); i++)
				{
					std::ostringstream s;
					s << scene->GetModel(i)->GetModelName();
					std::string mystring = s.str();
					modelStrings.push_back(mystring);
				}

				for (int i = 0; i < scene->GetModelCount(); i++)
				{
					items[i] = modelStrings[i].c_str();
				}

				int currentModelIndex = scene->GetActiveModelIndex();
				ImGui::Combo("Active Model", &currentModelIndex, items, scene->GetModelCount());


				if (currentModelIndex != scene->GetActiveModelIndex())
				{
					scene->SetActiveModelIndex(currentModelIndex);
				}

				/**/

				delete items;

				ImGui::Checkbox("World Transformate", &inWorld);

				/* Scale */
				if (ImGui::InputFloat("X Scale", &XScale, 0.1f, 1.0f, "%.3f"))
				{
					if (inWorld)
						scene->GetActiveModel()->ScaleXWorld(XScale);
					else
						scene->GetActiveModel()->ScaleXModel(XScale);
				}
				if (ImGui::InputFloat("Y Scale", &YScale, 0.1f, 1.0f, "%.3f"))
				{
					if (inWorld)
						scene->GetActiveModel()->ScaleYWorld(YScale);
					else
						scene->GetActiveModel()->ScaleYModel(YScale);
				}
				if (ImGui::InputFloat("Z Scale", &ZScale, 0.1f, 1.0f, "%.3f"))
				{
					if (inWorld)
						scene->GetActiveModel()->ScaleZWorld(ZScale);
					else
						scene->GetActiveModel()->ScaleZModel(ZScale);
				}

				/* Rotate */
				if (ImGui::InputFloat("X Rotate", &XRotate, 0.1f, 1.0f, "%.3f"))
				{
					if (inWorld)
						scene->GetActiveModel()->RotateXWorld(XRotate);
					else
						scene->GetActiveModel()->RotateXModel(XRotate);
				}
				if (ImGui::InputFloat("Y Rotate", &YRotate, 1.0f, 1.0f, "%.3f"))
				{
					if (inWorld)
						scene->GetActiveModel()->RotateYWorld(YRotate);
					else
						scene->GetActiveModel()->RotateYModel(YRotate);
				}
				if (ImGui::InputFloat("Z Rotate", &ZRotate, 1.0f, 1.0f, "%.3f"))
				{
					if (inWorld)
						scene->GetActiveModel()->RotateZWorld(ZRotate);
					else
						scene->GetActiveModel()->RotateZModel(ZRotate);
				}

				/* Translate */
				if (ImGui::InputFloat("X Translate", &TranslationVector.x, 0.1f, 0.5f, "%.3f"))
				{
					if (inWorld)
						scene->GetActiveModel()->TranslateWorld(TranslationVector);
					else
						scene->GetActiveModel()->TranslateModel(TranslationVector);
				}
				if (ImGui::InputFloat("Y Translate", &TranslationVector.y, 0.1f, 0.5f, "%.3f"))
				{
					if (inWorld)
						scene->GetActiveModel()->TranslateWorld(TranslationVector);
					else
						scene->GetActiveModel()->TranslateModel(TranslationVector);
				}
				if (ImGui::InputFloat("Z Translate", &TranslationVector.z, 0.1f, 0.5f, "%.3f"))
				{
					if (inWorld)
						scene->GetActiveModel()->TranslateWorld(TranslationVector);
					else
						scene->GetActiveModel()->TranslateModel(TranslationVector);
				}

			}

			glm::vec3 modelColor = scene->GetActiveModel()->GetColor();
			if (ImGui::ColorEdit3("Model Color", (float*)&modelColor))
			{
				scene->GetActiveModel()->SetColor(modelColor);
			}

			std::shared_ptr<MeshModel> meshModel = std::dynamic_pointer_cast<MeshModel>(scene->GetActiveModel());
			if (meshModel)
			{
				//glm::vec4 normalColor = meshModel->GetNormalModel().GetColor();
				//if (ImGui::ColorEdit3("Normal Color", (float*)&normalColor))
				//{
				//	meshModel->GetNormalModel().SetColor(normalColor);
				//}
			}
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();


		/* Lights */
		ImGui::Begin("Lighting & Shading");
		ImGui::Text("LightCount = %d", scene->GetLightCount());
		if (ImGui::Button("Add point light"))
		{
			std::shared_ptr<PointLight> newLight = std::make_shared<PointLight>();
			scene->AddLight(newLight);
		}
		static int LightCount = 1;
		if (scene->GetLightCount() > 1)
		{
//			ImGui::Combo("Choose Light", &LightCount, scene->lights, scene->GetLightCount());
			ImGui::SliderInt("Light Selction", &LightCount, 1, scene->GetLightCount());
		}
		if (scene->GetLightCount() > 0)
		{
//			ImGui::Combo("Choose Light", &LightCount, scene->lights, LightCount);

			ImGui::SliderFloat("MoveLight_x", &scene->GetLight(LightCount - 1)->GetPosition().x, -1000, 1000);
			ImGui::SliderFloat("MoveLight_y", &scene->GetLight(LightCount - 1)->GetPosition().y, -1000, 1000);
			ImGui::SliderFloat("MoveLight_z", &scene->GetLight(LightCount - 1)->GetPosition().z, -1000, 1000);

			ImGui::SliderFloat("alfa", (float*)&scene->GetLight(LightCount - 1)->alfa, 0, 360);

			ImGui::ColorEdit3("Ambient_Reflection", (float*)&scene->GetLight(LightCount - 1)->ambient_ref);
			ImGui::ColorEdit3("Diffuse_Reflection", (float*)&scene->GetLight(LightCount - 1)->diffuse_ref);
			ImGui::ColorEdit3("Specular_Reflection", (float*)&scene->GetLight(LightCount - 1)->specular_ref);

		}
		ImGui::End();
	}

	
}


//#define _USE_MATH_DEFINES
//#include <cmath>
//#include <imgui/imgui.h>
//#include <stdio.h>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <nfd.h>
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"
//
//#include "Renderer.h"
//#include "Scene.h"
//#include "Utils.h"
//#include <iostream>
//
///**
// * Fields
// */
//static bool orthograph = false;
//int models_number = 0;
//static bool inWorld = false;
//static bool cam_transform = false;
//bool show_demo_window = false;
//bool show_another_window = false;
//
////glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
//glm::vec4 clear_color = glm::vec4(0, 0, 0, 0.00f); ///
//
//
///**
// * Function declarations
// */
//static void GlfwErrorCallback(int error, const char* description);
//GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
//ImGuiIO& SetupDearImgui(GLFWwindow* window);
//void StartFrame();
//void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
//void Cleanup(GLFWwindow* window);
//void DrawImguiMenus(ImGuiIO& io, Scene& scene);
//
///**
// * Function implementation
// */
//void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
//	// TODO: Handle mouse scroll here
//}
//
//int main(int argc, char** argv)
//{
//	int windowWidth = 1920, windowHeight = 1080; // 1280 , 720      1920 , 1080
//	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
//	if (!window)
//		return 1;
//
//	int frameBufferWidth, frameBufferHeight;
//	glfwMakeContextCurrent(window);
//	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
//	
//	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
//	Scene scene = Scene();
//	//Camera camera2 = Camera();
//	shared_ptr <Camera> new_camera = make_shared<Camera>();
//	scene.AddCamera(new_camera);
//	//shared_ptr<MeshModel> camera = Utils::LoadMeshModel("camera.obj");
//	//scene.camera1 = camera;
//
//	scene.active_camera_index = 0;
//	
//	Light a_light = Light();
//	std::shared_ptr<Light> lighting = std::make_shared<Light>(a_light);
//	scene.lights.push_back(lighting);
//
//	ImGuiIO& io = SetupDearImgui(window);
//	glfwSetScrollCallback(window, ScrollCallback);
//	while (!glfwWindowShouldClose(window))
//	{
//		glfwPollEvents();
//		StartFrame();
//		DrawImguiMenus(io, scene);
//		RenderFrame(window, scene, renderer, io);
//	}
//
//	Cleanup(window);
//	return 0;
//}
//
//static void GlfwErrorCallback(int error, const char* description)
//{
//	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
//}
//
//GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
//{
//	glfwSetErrorCallback(GlfwErrorCallback);
//	if (!glfwInit())
//		return NULL;
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#if __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
//	glfwMakeContextCurrent(window);
//	glfwSwapInterval(1); // Enable vsync
//	// very importent!! initialization of glad
//	// https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped
//
//	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
//	return window;
//}
//
//ImGuiIO& SetupDearImgui(GLFWwindow* window)
//{
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO();
//	ImGui_ImplGlfw_InitForOpenGL(window, true);
//	ImGui_ImplOpenGL3_Init();
//	ImGui::StyleColorsDark();
//	return io;
//}
//
//void StartFrame()
//{
//	ImGui_ImplOpenGL3_NewFrame();
//	ImGui_ImplGlfw_NewFrame();
//	ImGui::NewFrame();
//}
//
//void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
//{
//	ImGui::Render();
//	int frameBufferWidth, frameBufferHeight;
//	glfwMakeContextCurrent(window);
//	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
//
//	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
//	{
//		// TODO: Set new aspect ratio
//		Renderer rend_for_changed_window = Renderer(frameBufferWidth, frameBufferHeight);
//	}
//	
//	if (!io.WantCaptureKeyboard)
//	{
//		// TODO: Handle keyboard events here
//		if (io.KeysDown[65])
//		{
//			// A key is down
//			// Use the ASCII table for more key codes (https://www.asciitable.com/)
//
//		}
//		//Here we add an option to do Model Translation , if we want to move it right and Up so we have to click "I" and "T" together
//		//so that mean increasing translation and if we want the opposite so we have to click "D" and "T"
//		if (io.KeysDown[73] && io.KeysDown[84])
//		{
//			scene.GetActiveModel().Translation_mat[3][0] += 10;
//			scene.GetActiveModel().Translation_mat[3][1] += 10;
//		}
//		if (io.KeysDown[68] && io.KeysDown[84])
//		{
//			scene.GetActiveModel().Translation_mat[3][0] -= 10;
//			scene.GetActiveModel().Translation_mat[3][1] -= 10;
//		}
//		//Click "R" to move Right 
//		if (io.KeysDown[82])
//		{
//			scene.GetActiveModel().Translation_mat[3][0] += 15;
//		}
//		//Click "L" to move Left
//		if (io.KeysDown[76])
//		{
//			scene.GetActiveModel().Translation_mat[3][0] -= 15;
//		}
//		//Click "U" to move Up
//		if (io.KeysDown[85])
//		{
//			scene.GetActiveModel().Translation_mat[3][1] += 15;
//		}
//		//Click "D" to move Down
//		if (io.KeysDown[68])
//		{
//			scene.GetActiveModel().Translation_mat[3][1] -= 15;
//		}
//
//	}
//	
//	//Here we control model scale by using mouse ,make it bigger by clicking on the mouse left side and make it smaller by clicking on the mouse right side
//	if (!io.WantCaptureMouse)
//	{
//		// TODO: Handle mouse events here
//		if (io.MouseDown[0])
//		{
//			if (scene.GetModelCount())
//				// Left mouse button is down
//			{
//				scene.GetActiveModel().Scale_mat[0][0] += 5;
//				scene.GetActiveModel().Scale_mat[1][1] += 5;
//			}
//
//		}
//		if (io.MouseDown[1])
//		{
//			if (scene.GetModelCount())
//				// Right mouse button is down
//			{
//				scene.GetActiveModel().Scale_mat[0][0] -= 5;
//				scene.GetActiveModel().Scale_mat[1][1] -= 5;
//			}
//
//		}
//	}
//
//	renderer.ClearColorBuffer(clear_color);
//	renderer.Render(scene);
//	renderer.SwapBuffers();
//
//	if (scene.GetModelCount())
//	{
//		scene.GetActiveModel().SetTransformate();
//		scene.GetActiveCamera().SetTransformate();
//	}
//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//	glfwMakeContextCurrent(window);
//	glfwSwapBuffers(window);
//}
//
//void Cleanup(GLFWwindow* window)
//{
//	ImGui_ImplOpenGL3_Shutdown();
//	ImGui_ImplGlfw_Shutdown();
//	ImGui::DestroyContext();
//
//	glfwDestroyWindow(window);
//	glfwTerminate();
//}
//
//void DrawImguiMenus(ImGuiIO& io, Scene& scene)
//{
//	/**
//	 * MeshViewer menu
//	 */
//	ImGui::Begin("MeshViewer Menu");
//
//	// Menu Bar
//	if (ImGui::BeginMainMenuBar())
//	{
//		if (ImGui::BeginMenu("File"))
//		{
//			if (ImGui::MenuItem("Open", "CTRL+O"))
//			{
//				nfdchar_t* outPath = NULL;
//				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
//				if (result == NFD_OKAY)
//				{
//					scene.AddModel(Utils::LoadMeshModel(outPath));
//					free(outPath);
//				}
//				else if (result == NFD_CANCEL)
//				{
//				}
//				else
//				{
//				}
//
//			}
//			ImGui::EndMenu();
//		}
//
//		// TODO: Add more menubar items (if you want to)
//		ImGui::EndMainMenuBar();
//	}
//
//	// Controls
//	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
//	// TODO: Add more controls as needed
//
//	ImGui::End();
//
//	/**
//	 * Imgui demo - you can remove it once you are familiar with imgui
//	 */
//
//	 // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//	if (show_demo_window)
//		ImGui::ShowDemoWindow(&show_demo_window);
//
//	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
//	{
//		static float f = 0.0f;
//		static int counter = 0;
//
//		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//
//		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//		ImGui::Checkbox("Another Window", &show_another_window);
//
//		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//		//ImGui::SliderInt("Scale", &scene.GetModel(0).Scale_ch, -1000, 1000);
//		//
//		if (scene.GetModelCount())
//			ImGui::ColorEdit3("Object color", (float*)&scene.GetActiveModel().ObjectColor); // Edit 3 floats representing a color
//		//
//
//		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//			counter++;
//		ImGui::SameLine();
//		ImGui::Text("counter = %d", counter);
//
//		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//		ImGui::End();
//	}
//
//	// 3. Show another simple window.
//	if (show_another_window)
//	{
//		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
//		ImGui::Text("Hello from another window!");
//		if (ImGui::Button("Close Me"))
//			show_another_window = false;
//		ImGui::End();
//	}
//
//	//Create GUI items to specify the local and world transformations
//	if (scene.GetModelCount())
//	{
//		ImGui::Begin("Translate,Rotate,Scale");
//		if (ImGui::Button("Next_model"))
//			scene.SetActiveModelIndex((scene.GetActiveModelIndex() + 1) % scene.GetModelCount());
//
//		ImGui::Checkbox("tranformate_world", &inWorld);
//		if (!inWorld)
//		{
//			ImGui::SliderFloat("xTranslate", &scene.GetActiveModel().Translation_mat[3][0], -1000, 1000);
//			ImGui::SliderFloat("yTranslate", &scene.GetActiveModel().Translation_mat[3][1], -1000, 1000);
//			ImGui::SliderFloat("zTranslate", &scene.GetActiveModel().Translation_mat[3][2], -1000, 1000);
//			ImGui::SliderFloat("xRotate", &scene.GetActiveModel().x, -180, 180);
//			ImGui::SliderFloat("yRotate", &scene.GetActiveModel().y, -180, 180);
//			ImGui::SliderFloat("zRotate", &scene.GetActiveModel().z, -180, 180);
//			ImGui::SliderFloat("xScale", &scene.GetActiveModel().Scale_mat[0][0], 0, 1000);
//			ImGui::SliderFloat("yScale", &scene.GetActiveModel().Scale_mat[1][1], 0, 1000);
//			ImGui::SliderFloat("zScale", &scene.GetActiveModel().Scale_mat[2][2], 0, 1000);
//		}
//		else
//		{
//			ImGui::SliderFloat("xWorld_Translate", &scene.GetActiveModel().w_translate[3][0], -1000, 1000);
//			ImGui::SliderFloat("yWorld_Translate", &scene.GetActiveModel().w_translate[3][1], -1000, 1000);
//			ImGui::SliderFloat("zWorld_Translate", &scene.GetActiveModel().w_translate[3][2], -1000, 1000);
//			ImGui::SliderFloat("xWorld_Rotate", &scene.GetActiveModel().xw, -180, 180);
//			ImGui::SliderFloat("yWorld_Rotate", &scene.GetActiveModel().yw, -180, 180);
//			ImGui::SliderFloat("zWorld_Rotate", &scene.GetActiveModel().zw, -180, 180);
//			ImGui::SliderFloat("xWorld_Scale", &scene.GetActiveModel().w_scale[0][0], 0, 1000);
//			ImGui::SliderFloat("yWorld_Scale", &scene.GetActiveModel().w_scale[1][1], 0, 1000);
//			ImGui::SliderFloat("zWorld_Scale", &scene.GetActiveModel().w_scale[2][2], 0, 1000);
//		}
//		ImGui::End();
//		if (scene.GetModelCount())
//		{
//			ImGui::Begin("Camera-Change View Volume-Transform & Rotate");
//			
//			if (ImGui::Button("Next_model"))
//				scene.SetActiveModelIndex((scene.GetActiveModelIndex() + 1) % scene.GetModelCount());
// 
//			ImGui::Checkbox("Camera_Transformation",&cam_transform);
//			ImGui::Checkbox("Bounding Box", &scene.GetActiveModel().bounding_box);
//			ImGui::Checkbox("faces_normal", &scene.GetActiveModel().face_normals);
//			ImGui::Checkbox("vertices_normal", &scene.GetActiveModel().vertex_normals);
//			ImGui::Checkbox("Orthograhic", &scene.GetActiveCamera().orth);
//			ImGui::Checkbox("Perspective", &scene.GetActiveCamera().Perspective);
//			ImGui::Checkbox("World Axis", &scene.GetActiveCamera().WorldAxis);
//			ImGui::Checkbox("Model Axis", &scene.GetActiveModel().ModelAxis);
//
//			if (cam_transform)
//			{
//				ImGui::SliderFloat("xCamera_Translate", &scene.GetActiveCamera().TranslationCam_mat[3][0], -1000, 1000);
//				ImGui::SliderFloat("yCamera_Translate", &scene.GetActiveCamera().TranslationCam_mat[3][1], -1000, 1000);
//				ImGui::SliderFloat("zCamera_Translate", &scene.GetActiveCamera().TranslationCam_mat[3][2], -1000, 1000);
//				ImGui::SliderFloat("xCamera_Rotate", &scene.GetActiveCamera().x_cam, -360, 360);
//				ImGui::SliderFloat("yCamera_Rotate", &scene.GetActiveCamera().y_cam, -360, 360);
//				ImGui::SliderFloat("zCamera_Rotate", &scene.GetActiveCamera().z_cam, -360, 360);
//				ImGui::SliderFloat("Eye's_x", &scene.GetActiveCamera().Eye.x, -10, 10);
//				ImGui::SliderFloat("Eye's_y", &scene.GetActiveCamera().Eye.y, -10, 10);
//				ImGui::SliderFloat("Eye's_z", &scene.GetActiveCamera().Eye.z,-10,10);
//			}
//			if (scene.GetActiveCamera().orth)
//			{
////				scene.GetActiveCamera().Perspective = false;
//				ImGui::SliderFloat("up_orthographic", &scene.GetActiveCamera().up, -5, 5);
//				ImGui::SliderFloat("down_orthographic", &scene.GetActiveCamera().down, -5, 5);
//				ImGui::SliderFloat("left_orthographic", &scene.GetActiveCamera().left, -5, 5);
//				ImGui::SliderFloat("right_orthographic", &scene.GetActiveCamera().right, -5, 5);
//
//				scene.GetActiveCamera().SetOrthographicProjection(scene.GetActiveCamera().left, scene.GetActiveCamera().right, scene.GetActiveCamera().down, scene.GetActiveCamera().up, 1, -1);
//			}
//			else if(scene.GetActiveCamera().Perspective)
//			{
//	//			scene.GetActiveCamera().orth = false;
//				/*ImGui::SliderFloat("Up_Perspective", &scene.GetActiveCamera().up, -5, 5);
//				ImGui::SliderFloat("Down_Perspective", &scene.GetActiveCamera().down, -5, 5);
//				ImGui::SliderFloat("Left_Perspective", &scene.GetActiveCamera().left, -5, 5);
//				ImGui::SliderFloat("Right_Perspective", &scene.GetActiveCamera().right, -5, 5);*/
//				ImGui::SliderFloat("Near_Perspective", &scene.GetActiveCamera().near1, -5, 5);
//				ImGui::SliderFloat("Far_Perspective", &scene.GetActiveCamera().far1, -5, 5);
//				/*ImGui::SliderFloat("Aspect_perspective", &scene.GetActiveCamera().aspect, -50, 50);
//				ImGui::SliderFloat("Fovy_perspective", &scene.GetActiveCamera().fovy, -0.4, 0.8);*/
//				ImGui::InputFloat("fovy", &scene.GetActiveCamera().fovy, 10, 180);
//				float aspectRatio = ((scene.GetActiveCamera().right - scene.GetActiveCamera().left) / (scene.GetActiveCamera().up - scene.GetActiveCamera().down));
//				scene.GetActiveCamera().SetPerspectiveProjection(glm::radians(scene.GetActiveCamera().fovy), scene.GetActiveCamera().aspect, scene.GetActiveCamera().near1, scene.GetActiveCamera().far1);
//				//				scene.GetActiveCamera().SetPerspectiveProjection(glm::radians(scene.GetActiveCamera().fovy), aspectRatio, scene.GetActiveCamera().near1, scene.GetActiveCamera().far1);
//			}
//
//			//if (ImGui::Button("Add Camera"))
//			//{
//			//	shared_ptr <Camera> new_camera = make_shared<Camera>();
//			//	scene.AddCamera(new_camera);
//			//}
//			//if (ImGui::Button("Next Camera"))
//			//	scene.SetActiveCameraIndex((scene.GetActiveCameraIndex() + 1) % scene.GetCameraCount());
//			ImGui::End();
//		}
//
//		if (scene.GetModelCount())
//		{
//			ImGui::Begin("Triangle fill and z-buffer");
//			ImGui::Checkbox("Bounding Rectangle", &scene.GetActiveModel().bounding_rectangle);
//
//			int Colourful = ImGui::Button("Colorful Model");
//			if (Colourful)
//			{
//				scene.GetActiveModel().SetTriColors(1);
//			}
//			int Gray = ImGui::Button("Gray Model");
//			if (Gray)
//			{
//				scene.GetActiveModel().SetTriColors(2);
//			}
//			int ZBuffer = ImGui::Button("ZBuffer");
//			if (ZBuffer)
//			{
//				scene.GetActiveModel().ZBufferOn = 1;
//			}
//
//			ImGui::End();
//		}
//
//		/* Lighting and shading */
//		ImGui::Begin("Lighting & Shading");
//		ImGui::Checkbox("reflection vector", &scene.GetActiveModel().reflection_vec);
//		ImGui::RadioButton("Flat Shading", &scene.GetActiveModel().shade_type, 0);
//		ImGui::RadioButton("Gouraud Shading", &scene.GetActiveModel().shade_type, 1);
//		ImGui::RadioButton("Phong Shading", &scene.GetActiveModel().shade_type, 2);
//
//		static int LightCount = 0;
//		static char* lights[5] = { "1","2","3","4","5" };
//		ImGui::Text("LightCount = %d", LightCount);
//		if (ImGui::Button("Add point light") && (LightCount < 3))
//		{
//			LightCount++;
//		}
//		if (LightCount)
//			scene.lighting = true;
//
//		if (LightCount == 1) {
////			ImGui::Combo("Choose Light", &LightCount, lights, LightCount);
//			ImGui::Text("Light RGB");
//			ImGui::SliderFloat("MoveLight_x", &scene.lights[0]->TranslateMat[3][0], -1000, 1000);
//			ImGui::SliderFloat("MoveLight_y", &scene.lights[0]->TranslateMat[3][1], -1000, 1000);
//			ImGui::SliderFloat("MoveLight_z", &scene.lights[0]->TranslateMat[3][2], -1000, 1000);
//			ImGui::SliderFloat("alfa", &scene.GetActiveModel().alfa, 0, 360);
//
//			ImGui::ColorEdit3("Ambient_Reflection", (float*)&scene.lights[0]->ambient_ref);
//			ImGui::ColorEdit3("Diffuse_Reflection", (float*)&scene.lights[0]->diffuse_ref);
//			ImGui::ColorEdit3("Specular_Reflection", (float*)&scene.lights[0]->specular_ref);
// 		}
//		if (LightCount == 2) {
//			static int light_num = 1;
//			ImGui::RadioButton("Light 1", &light_num, 1); ImGui::SameLine();
//			ImGui::RadioButton("Light 2", &light_num, 2);
//			scene.more_than_1_light = true;
//
//
//			if (light_num == 1) {
////				ImGui::Combo("Choose Light", &LightCount, lights, LightCount);
//				ImGui::SliderFloat("MoveLight_x", &scene.lights[0]->TranslateMat[3][0], -1000, 1000);
//				ImGui::SliderFloat("MoveLight_y", &scene.lights[0]->TranslateMat[3][1], -1000, 1000);
//				ImGui::SliderFloat("MoveLight_z", &scene.lights[0]->TranslateMat[3][2], -1000, 1000);
//				ImGui::SliderFloat("alfa", &scene.GetActiveModel().alfa, 0, 360);
//				ImGui::ColorEdit3("Ambient_Reflection", (float*)&scene.lights[0]->ambient_ref);
//				ImGui::ColorEdit3("Diffuse_Reflection", (float*)&scene.lights[0]->diffuse_ref);
//				ImGui::ColorEdit3("Specular_Reflection", (float*)&scene.lights[0]->specular_ref);
//			}
//			else {
//
////				ImGui::Combo("Choose Light", &LightCount, lights, LightCount);
//				ImGui::SliderFloat("MoveLight_x", &scene.lights[0]->TranslateMat[3][0], -1000, 1000);
//				ImGui::SliderFloat("MoveLight_y", &scene.lights[0]->TranslateMat[3][1], -1000, 1000);
//				ImGui::SliderFloat("MoveLight_z", &scene.lights[0]->TranslateMat[3][2], -1000, 1000);
//				ImGui::SliderFloat("alfa", &scene.GetActiveModel().alfa, 0, 360);
//				ImGui::ColorEdit3("Ambient_Reflection", (float*)&scene.lights[0]->ambient_ref);
//				ImGui::ColorEdit3("Diffuse_Reflection", (float*)&scene.lights[0]->diffuse_ref);
//				ImGui::ColorEdit3("Specular_Reflection", (float*)&scene.lights[0]->specular_ref);
//			}
//		}
//
//		if (scene.GetModelCount())
//		{
//			/*for materials*/
//			ImGui::ColorEdit3("Material_Ambient_Reflection", (float*)&scene.GetActiveModel().Ambient_ref);
//			ImGui::ColorEdit3("Material_Diffuse_Reflection", (float*)&scene.GetActiveModel().Diffuse_ref);
//			ImGui::ColorEdit3("Material_Specular_Reflection", (float*)&scene.GetActiveModel().Specular_ref);
//
//		}
//
//
//
//		//ImGui::Checkbox("reflection vector", &scene.GetActiveModel().reflection_vec);
//		//ImGui::RadioButton("Flat Shading", &scene.GetActiveModel().shade_type, 0);
//		//ImGui::RadioButton("Gouraud Shading", &scene.GetActiveModel().shade_type, 1);
//		//ImGui::RadioButton("Phong Shading", &scene.GetActiveModel().shade_type, 2);
//		//ImGui::SliderFloat("MoveLight_x", &scene.lights[0]->TranslateMat[3][0], -1000, 1000);
//		//ImGui::SliderFloat("MoveLight_y", &scene.lights[0]->TranslateMat[3][1], -1000, 1000);
//		//ImGui::SliderFloat("MoveLight_z", &scene.lights[0]->TranslateMat[3][2], -1000, 1000);
//		//ImGui::SliderFloat("alfa", &scene.GetActiveModel().alfa, 0, 360);
//		//ImGui::ColorEdit3("Ambient_Reflection", (float*)&scene.lights[0]->ambient_ref);
//		//ImGui::ColorEdit3("Diffuse_Reflection", (float*)&scene.lights[0]->diffuse_ref);
//		//ImGui::ColorEdit3("Specular_Reflection", (float*)&scene.lights[0]->specular_ref);
//		///*for materials*/
//		//ImGui::ColorEdit3("Material_Ambient_Reflection", (float*)&scene.GetActiveModel().Ambient_ref);
//		//ImGui::ColorEdit3("Material_Diffuse_Reflection", (float*)&scene.GetActiveModel().Diffuse_ref);
//		//ImGui::ColorEdit3("Material_Specular_Reflection", (float*)&scene.GetActiveModel().Specular_ref);
//		ImGui::End();
//	}
//}
