#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include <iostream>

/**
 * Fields
 */
static bool orthograph = false;
int models_number = 0;
static bool inWorld = false;
static bool cam_transform = false;
bool show_demo_window = false;
bool show_another_window = false;

//glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
glm::vec4 clear_color = glm::vec4(0.3f, 0.3f, 0.3f, 0.00f); ///


/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char** argv)
{
	int windowWidth = 1920, windowHeight = 1080; // 1280 , 720      1920 , 1080
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	
	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	shared_ptr <Camera> new_camera = make_shared<Camera>();
	scene.AddCamera(new_camera);
	

	scene.active_camera_index = 0;
	

	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		RenderFrame(window, scene, renderer, io);
	}

	Cleanup(window);
	return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
	// very importent!! initialization of glad
	// https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
		Renderer rend_for_changed_window = Renderer(frameBufferWidth, frameBufferHeight);
	}
	
	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown[65])
		{
			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)

		}
		//Here we add an option to do Model Translation , if we want to move it right and Up so we have to click "I" and "T" together
		//so that mean increasing translation and if we want the opposite so we have to click "D" and "T"
		if (io.KeysDown[73] && io.KeysDown[84])
		{
			scene.GetActiveModel().Translation_mat[3][0] += 10;
			scene.GetActiveModel().Translation_mat[3][1] += 10;
		}
		if (io.KeysDown[68] && io.KeysDown[84])
		{
			scene.GetActiveModel().Translation_mat[3][0] -= 10;
			scene.GetActiveModel().Translation_mat[3][1] -= 10;
		}
		//Click "R" to move Right 
		if (io.KeysDown[82])
		{
			scene.GetActiveModel().Translation_mat[3][0] += 15;
		}
		//Click "L" to move Left
		if (io.KeysDown[76])
		{
			scene.GetActiveModel().Translation_mat[3][0] -= 15;
		}
		//Click "U" to move Up
		if (io.KeysDown[85])
		{
			scene.GetActiveModel().Translation_mat[3][1] += 15;
		}
		//Click "D" to move Down
		if (io.KeysDown[68])
		{
			scene.GetActiveModel().Translation_mat[3][1] -= 15;
		}

	}
	
	//Here we control model scale by using mouse ,make it bigger by clicking on the mouse left side and make it smaller by clicking on the mouse right side
	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			if (scene.GetModelCount())
				// Left mouse button is down
			{
				scene.GetActiveModel().Scale_mat[0][0] += 5;
				scene.GetActiveModel().Scale_mat[1][1] += 5;
			}

		}
		if (io.MouseDown[1])
		{
			if (scene.GetModelCount())
				// Right mouse button is down
			{
				scene.GetActiveModel().Scale_mat[0][0] -= 5;
				scene.GetActiveModel().Scale_mat[1][1] -= 5;
			}

		}
	}

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	if (scene.GetModelCount())
	{
		scene.GetActiveModel().SetTransformate();
		scene.GetActiveCamera().SetTransformate();
	}
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	// TODO: Add more controls as needed

	ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */

	 // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//ImGui::SliderInt("Scale", &scene.GetModel(0).Scale_ch, -1000, 1000);
		//
		if (scene.GetModelCount())
			ImGui::ColorEdit3("Object color", (float*)&scene.GetActiveModel().ObjectColor); // Edit 3 floats representing a color
		//

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	//Create GUI items to specify the local and world transformations
	if (scene.GetModelCount())
	{
		ImGui::Begin("Translate,Rotate,Scale");
		if (ImGui::Button("Next_model"))
			scene.SetActiveModelIndex((scene.GetActiveModelIndex() + 1) % scene.GetModelCount());

		ImGui::Checkbox("tranformate_world", &inWorld);
		if (!inWorld)
		{
			ImGui::SliderFloat("xTranslate", &scene.GetActiveModel().Translation_mat[3][0], -1000, 1000);
			ImGui::SliderFloat("yTranslate", &scene.GetActiveModel().Translation_mat[3][1], -1000, 1000);
			ImGui::SliderFloat("zTranslate", &scene.GetActiveModel().Translation_mat[3][2], -1000, 1000);
			ImGui::SliderFloat("xRotate", &scene.GetActiveModel().x, -180, 180);
			ImGui::SliderFloat("yRotate", &scene.GetActiveModel().y, -180, 180);
			ImGui::SliderFloat("zRotate", &scene.GetActiveModel().z, -180, 180);
			ImGui::SliderFloat("xScale", &scene.GetActiveModel().Scale_mat[0][0], 0, 1000);
			ImGui::SliderFloat("yScale", &scene.GetActiveModel().Scale_mat[1][1], 0, 1000);
			ImGui::SliderFloat("zScale", &scene.GetActiveModel().Scale_mat[2][2], 0, 1000);
		}
		else
		{
			ImGui::SliderFloat("xWorld_Translate", &scene.GetActiveModel().w_translate[3][0], -1000, 1000);
			ImGui::SliderFloat("yWorld_Translate", &scene.GetActiveModel().w_translate[3][1], -1000, 1000);
			ImGui::SliderFloat("zWorld_Translate", &scene.GetActiveModel().w_translate[3][2], -1000, 1000);
			ImGui::SliderFloat("xWorld_Rotate", &scene.GetActiveModel().xw, -180, 180);
			ImGui::SliderFloat("yWorld_Rotate", &scene.GetActiveModel().yw, -180, 180);
			ImGui::SliderFloat("zWorld_Rotate", &scene.GetActiveModel().zw, -180, 180);
			ImGui::SliderFloat("xWorld_Scale", &scene.GetActiveModel().w_scale[0][0], 0, 1000);
			ImGui::SliderFloat("yWorld_Scale", &scene.GetActiveModel().w_scale[1][1], 0, 1000);
			ImGui::SliderFloat("zWorld_Scale", &scene.GetActiveModel().w_scale[2][2], 0, 1000);
		}
		ImGui::End();
		if (scene.GetModelCount())
		{
			ImGui::Begin("Camera-Change View Volume-Transform & Rotate");
			
			if (ImGui::Button("Next_model"))
				scene.SetActiveModelIndex((scene.GetActiveModelIndex() + 1) % scene.GetModelCount());
 
			ImGui::Checkbox("Camera_Transformation",&cam_transform);
			ImGui::Checkbox("Bounding Box", &scene.GetActiveModel().bounding_box);
			ImGui::Checkbox("faces_normal", &scene.GetActiveModel().face_normals);
			ImGui::Checkbox("vertices_normal", &scene.GetActiveModel().vertex_normals);
			ImGui::Checkbox("Orthograhic", &scene.GetActiveCamera().orth);
			ImGui::Checkbox("Perspective", &scene.GetActiveCamera().Perspective);
			ImGui::Checkbox("World Axis", &scene.GetActiveCamera().WorldAxis);
			ImGui::Checkbox("Model Axis", &scene.GetActiveModel().ModelAxis);

			if (cam_transform)
			{
				ImGui::SliderFloat("xCamera_Translate", &scene.GetActiveCamera().TranslationCam_mat[3][0], -1000, 1000);
				ImGui::SliderFloat("yCamera_Translate", &scene.GetActiveCamera().TranslationCam_mat[3][1], -1000, 1000);
				ImGui::SliderFloat("zCamera_Translate", &scene.GetActiveCamera().TranslationCam_mat[3][2], -1000, 1000);
				ImGui::SliderFloat("xCamera_Rotate", &scene.GetActiveCamera().x_cam, -360, 360);
				ImGui::SliderFloat("yCamera_Rotate", &scene.GetActiveCamera().y_cam, -360, 360);
				ImGui::SliderFloat("zCamera_Rotate", &scene.GetActiveCamera().z_cam, -360, 360);
				ImGui::SliderFloat("Eye's_x", &scene.GetActiveCamera().Eye.x, -10, 10);
				ImGui::SliderFloat("Eye's_y", &scene.GetActiveCamera().Eye.y, -10, 10);
				ImGui::SliderFloat("Eye's_z", &scene.GetActiveCamera().Eye.z,-10,10);
			}
			if (scene.GetActiveCamera().orth)
			{
//				scene.GetActiveCamera().Perspective = false;
				ImGui::SliderFloat("up_orthographic", &scene.GetActiveCamera().up, -5, 5);
				ImGui::SliderFloat("down_orthographic", &scene.GetActiveCamera().down, -5, 5);
				ImGui::SliderFloat("left_orthographic", &scene.GetActiveCamera().left, -5, 5);
				ImGui::SliderFloat("right_orthographic", &scene.GetActiveCamera().right, -5, 5);

				scene.GetActiveCamera().SetOrthographicProjection(scene.GetActiveCamera().left, scene.GetActiveCamera().right, scene.GetActiveCamera().down, scene.GetActiveCamera().up, 1, -1);
			}
			else if(scene.GetActiveCamera().Perspective)
			{
	//			scene.GetActiveCamera().orth = false;
				/*ImGui::SliderFloat("Up_Perspective", &scene.GetActiveCamera().up, -5, 5);
				ImGui::SliderFloat("Down_Perspective", &scene.GetActiveCamera().down, -5, 5);
				ImGui::SliderFloat("Left_Perspective", &scene.GetActiveCamera().left, -5, 5);
				ImGui::SliderFloat("Right_Perspective", &scene.GetActiveCamera().right, -5, 5);*/
				ImGui::SliderFloat("Near_Perspective", &scene.GetActiveCamera().near1, -5, 5);
				ImGui::SliderFloat("Far_Perspective", &scene.GetActiveCamera().far1, -5, 5);
				/*ImGui::SliderFloat("Aspect_perspective", &scene.GetActiveCamera().aspect, -50, 50);
				ImGui::SliderFloat("Fovy_perspective", &scene.GetActiveCamera().fovy, -0.4, 0.8);*/
				ImGui::InputFloat("fovy", &scene.GetActiveCamera().fovy, 10, 180);
				float aspectRatio = ((scene.GetActiveCamera().right - scene.GetActiveCamera().left) / (scene.GetActiveCamera().up - scene.GetActiveCamera().down));
				scene.GetActiveCamera().SetPerspectiveProjection(glm::radians(scene.GetActiveCamera().fovy), scene.GetActiveCamera().aspect, scene.GetActiveCamera().near1, scene.GetActiveCamera().far1);
				//				scene.GetActiveCamera().SetPerspectiveProjection(glm::radians(scene.GetActiveCamera().fovy), aspectRatio, scene.GetActiveCamera().near1, scene.GetActiveCamera().far1);
			}

			//if (ImGui::Button("Add Camera"))
			//{
			//	shared_ptr <Camera> new_camera = make_shared<Camera>();
			//	scene.AddCamera(new_camera);
			//}
			//if (ImGui::Button("Next Camera"))
			//	scene.SetActiveCameraIndex((scene.GetActiveCameraIndex() + 1) % scene.GetCameraCount());
			ImGui::End();
		}
	}
}
