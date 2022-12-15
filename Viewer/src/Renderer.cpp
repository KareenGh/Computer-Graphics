#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

#define Scale = 200;
#define Translate = 200;

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height)
{
	InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width) return;
	if (j < 0) return; if (j >= viewport_height) return;

	color_buffer[INDEX(viewport_width, i, j, 0)] = color.x;
	color_buffer[INDEX(viewport_width, i, j, 1)] = color.y;
	color_buffer[INDEX(viewport_width, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	int x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;
	int deltaX = abs(x2 - x1);
	int deltaY = abs(y2 - y1);
	int Switch = 0;

	if (deltaX <= deltaY) // switch = 1 if we have to switch betwen x and y 
	{
		Switch = 1;
		x1 = p1.y;
		x2 = p2.y;
		y1 = p1.x;
		y2 = p2.x;
	}
	if (x1 > x2) // if p1 is in the right side of p2 switch th x-es
	{
		if (Switch == 1)
		{
			x1 = p2.y;
			x2 = p1.y;
			y1 = p2.x;
			y2 = p1.x;
		}
		else
		{
			x1 = p2.x;
			x2 = p1.x;
			y1 = p2.y;
			y2 = p1.y;
		}

	}
	deltaX = abs(x2 - x1);
	deltaY = abs(y2 - y1);

	int e = -deltaX;
	int x = x1;

	while (x1 <= x2)
	{
		if (e >= 0)
		{
			y1 < y2 ? y1++ : y1--; // reflect
			if (Switch == 0)
				PutPixel(x1, y1, color);
			else
				PutPixel(y1, x1, color);

			e -= 2 * deltaX;
		}
		else
		{
			if (Switch == 0)
				PutPixel(x1, y1, color);
			else
				PutPixel(y1, x1, color);
		}
		x1++;
		e += 2 * deltaY;
	}

}

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenglBuffer(); //Do not remove this line.
	color_buffer = new float[3 * w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenglRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[] = {
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vtc));

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
}

void Renderer::CreateOpenglBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width, viewport_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width, viewport_height);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width, viewport_height, GL_RGB, GL_FLOAT, color_buffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width; i++)
	{
		for (int j = 0; j < viewport_height; j++)
		{
			PutPixel(i, j, color);
		}
	}
}

//void Renderer::Render(const Scene& scene)
//{
//	// TODO: Replace this code with real scene rendering code
//	int half_width = viewport_width / 2;
//	int half_height = viewport_height / 2;
//	// draw circle
//	if (scene.GetModelCount())
//	{
//		DrawObject(scene.GetActiveModel());
//	}
//}


void Renderer::Render(const Scene& scene) 
{
	Scene temp_scene = scene;
	glm::vec4 point1, point2, point3;
	glm::mat4x4 ScaleTransMat;
	glm::mat4x4 TransMat;
	glm::ivec3 color(1 , 0.57 , 1.31);
	glm::mat4x4 Changer;
	glm::mat4x4 CameraTr = glm::mat4x4(1);

	Camera& act_camera = temp_scene.GetActiveCamera();

	
			
	if (act_camera.orth)
		CameraTr = glm::ortho(act_camera.left, act_camera.right, act_camera.down, act_camera.up);
	if (scene.GetModelCount()) {
		//we use for loop to enable more than one object to be active 
		for (int j = 0; j < scene.GetModelCount(); j++)
		{
			auto MyModel = scene.GetModel(j);
			ScaleTransMat = MyModel.GetSTMatrix();
			TransMat = MyModel.GetTransformMat();
			ScaleTransMat = TransMat * ScaleTransMat;
			Changer =CameraTr*MyModel.Transformate;

			for (int i = 0; i < MyModel.GetFacesCount(); i++)
			{
				// turn a vec3 to vec4, then mul to sacle and translation matrix
				point1 = Changer * glm::vec4(MyModel.GetVertix(i, 0), 1);
				point2 = Changer * glm::vec4(MyModel.GetVertix(i, 1), 1);
				point3 = Changer * glm::vec4(MyModel.GetVertix(i, 2), 1);

				// turn back to vec3	
				point1 /= point1.w;
				point2 /= point2.w;
				point3 /= point3.w;


				DrawLine(point1, point2, color);	//MyModel.ObjectColor
				DrawLine(point1, point3, color);
				DrawLine(point2, point3, color);
			}
		}
	}
}

int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}

void Renderer::DrawObject(MeshModel& Model)
{
	glm::vec4 point1 = glm::vec4(0.0f);
	glm::vec4 point2 = glm::vec4(0.0f);
	glm::vec4 point3 = glm::vec4(0.0f);

	MeshModel MyModel = Model;
	glm::mat4x4 Changer = MyModel.w_move * MyModel.w_scale * MyModel.xw_rotate * MyModel.yw_rotate * MyModel.zw_rotate * MyModel.Translation_mat * MyModel.x_rotate * MyModel.y_rotate * MyModel.z_rotate * MyModel.Scale_mat;
	;
	for (int i = 0; i < Model.GetFacesCount(); i++)
	{
		point1 = glm::vec4(MyModel.GetVertix(i, 0),1);
		point2 = glm::vec4(MyModel.GetVertix(i, 1),1);
		point3 = glm::vec4(MyModel.GetVertix(i, 2),1);

		point1 = Changer*point1;
		point2 = Changer*point2;
		point3 = Changer*point3;

		{
			DrawLine(point1, point2, MyModel.ObjectColor);
			DrawLine(point1, point3, MyModel.ObjectColor);
			DrawLine(point2, point3, MyModel.ObjectColor);
		}
	}
}
