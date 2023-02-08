#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void Renderer::DrawObject(MeshModel& Model);


	void PaintTriangles(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
	void PaintTriangles(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, MeshModel& Model, Scene& scene, const glm::vec3 normal); ///
	///
	bool InsidetheTriangle(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3); ///
	float area(int x1, int y1, int x2, int y2, int x3, int y3); ///
	void ZBuffer(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3); ///
	glm::vec3 Z_Calculate(int x, int y, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& val1, const glm::vec3& val2, const glm::vec3& val3); ///
	float GetZ(int i, int j); ////
	void SetZ(int i, int j, float z); ///
	void SetMaxZBuffer(); ///
	void PaintTrianglesGray(); ///
	glm::vec3 GetGrayColor(float z); //
	float maxbufferZ = std::numeric_limits<float>::min();	//
	float minbufferZ = std::numeric_limits<float>::max();	//
	float* Z_Buffer; ///
	glm::vec3 ChooseColor(MeshModel& MyModel, Scene& scene, glm::vec3 a_point,glm::vec3 normal);
	//float EdgeFunction(glm::vec3 v1, glm::vec3 v2, glm::vec3 p); ///////
	//bool Overlaps(const glm::vec3 v1, const glm::vec3 v2, const glm::vec3 v3, const glm::vec3 point); ///
	//void EdgeWalking(const Face& face, const MeshModel& model, const Camera& camera, const glm::vec3 color); ///
	int viewport_width;
	int viewport_height;

private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
	void DrawTriangle(const glm::ivec2& p1, const glm::ivec2& p2, const glm::ivec2& p3, const glm::vec3& color);
	


	void CreateBuffers(int w, int h);
	void CreateOpenglBuffer();
	void InitOpenglRendering();

	float* color_buffer;
	GLuint gl_screen_tex;
	GLuint gl_screen_vtc;
};
