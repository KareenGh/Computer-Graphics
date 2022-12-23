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
	glm::vec4 point1, point2, point3,p1,p2,p3;
	glm::mat4x4 ScaleTransMat;
	glm::mat4x4 TransMat;
	glm::ivec3 color(1 , 0.57 , 1.31);
	glm::mat4x4 Changer;
	glm::mat4x4 CameraTr = glm::mat4x4(1);

	Camera& act_camera = temp_scene.GetActiveCamera();

	if(act_camera.Orthograhic)
		CameraTr = act_camera.GetOrthTransformation();
	else// if(act_camera.Perspective)
		CameraTr = act_camera.GetPerspectiveProjection();

//	if (act_camera.orth)
//		CameraTr = act_camera.GetOrthTransformation();
//			CameraTr = glm::ortho(act_camera.left, act_camera.right, act_camera.down, act_camera.up)*glm::lookAt(act_camera.Eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))* glm::inverse(act_camera.CamTransformate);
//	else
//		CameraTr = act_camera.GetPerspectiveProjection();
////		CameraTr = glm::perspective(act_camera.fovy, act_camera.aspectRatio, act_camera.near1, act_camera.far1) * glm::lookAt(act_camera.Eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)) * glm::inverse(act_camera.CamTransformate);

	MeshModel& MyModel = *scene.camera1;
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		cout << act_camera.CamTransformate[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	for (int c1 = 0; c1 < scene.GetCameraCount(); c1++)
	{
		Changer = CameraTr * act_camera.CamTransformate;
		Camera camera1 = temp_scene.GetCamera(c1);
		if (c1 != scene.GetActiveCameraIndex())
		{
			for (int c2 = 0; c2 < MyModel.GetFacesCount(); c2++)
			{
				
				point1 = Changer * glm::vec4(MyModel.GetVertix(c2, 0), 1);
				point2 = Changer * glm::vec4(MyModel.GetVertix(c2, 1), 1);
				point3 = Changer * glm::vec4(MyModel.GetVertix(c2, 2), 1);


				point1 /= point1.w;
				point2 /= point2.w;
				point3 /= point3.w;

				point1[0] += viewport_width / 2;
				point1[1] += viewport_height / 2;
				point2[0] += viewport_width / 2;
				point2[1] += viewport_height / 2;
				point3[0] += viewport_width / 2;
				point3[1] += viewport_height / 2;
				
				
				DrawLine(point1, point2, color);	
				DrawLine(point1, point3, color);
				DrawLine(point2, point3, color);
			}
		}
	}
	if (scene.GetModelCount()) {
		//we use for loop to enable more than one object to be active 
		for (int j = 0; j < scene.GetModelCount(); j++)
		{
			auto MyModel = scene.GetModel(j);
			ScaleTransMat = MyModel.GetSTMatrix();
			TransMat = MyModel.GetTransformMat();
			ScaleTransMat = TransMat * ScaleTransMat;
			Changer = CameraTr * MyModel.Transformate;

			for (int i = 0; i < MyModel.GetFacesCount(); i++)
			{
				p1 = glm::vec4(MyModel.GetVertix(i, 0), 1);
				p2 = glm::vec4(MyModel.GetVertix(i, 1), 1);
				p3 = glm::vec4(MyModel.GetVertix(i, 2), 1);
				// turn a vec3 to vec4, then mul to sacle and translation matrix
				point1 = Changer * p1;
				point2 = Changer * p2;
				point3 = Changer * p3;

				// turn back to vec3	
				point1 /= point1.w;
				point2 /= point2.w;
				point3 /= point3.w;

				point1[0] += viewport_width / 2;
				point1[1] += viewport_height / 2;
				point2[0] += viewport_width / 2;
				point2[1] += viewport_height / 2;
				point3[0] += viewport_width / 2;
				point3[1] += viewport_height / 2;


				if (MyModel.face_normals)
				{
					glm::vec3 fn1 = p1 - p2;
					glm::vec3 fn2 = p2 - p3;
					glm::vec4 facenormal = glm::vec4(glm::cross(fn1, fn2), 0);
					glm::vec4 f_average = (point1 + point2 + point3);
					f_average /= 3;
					facenormal *= 1200;
					DrawLine(f_average, f_average + facenormal, glm::vec3(12, 156, 31));

				}

				if (MyModel.vertex_normals)
				{
					const Face& fv = MyModel.GetFace(i);
					glm::vec3 vn_color = glm::vec3(76, 153, 0);
					glm::vec4 vn1 = glm::vec4(MyModel.normals[fv.GetNormalIndex(0) - 1], 0);
					glm::vec4 vn2 = glm::vec4(MyModel.normals[fv.GetNormalIndex(1) - 1], 0);
					glm::vec4 vn3 = glm::vec4(MyModel.normals[fv.GetNormalIndex(2) - 1], 0);
					vn1 *= 15, vn2 *= 15, vn3 *= 15;
					DrawLine(point1, point1 + vn1, vn_color);
					DrawLine(point2, point2 + vn2, vn_color);
					DrawLine(point3, point3 + vn3, vn_color);
				}


				DrawLine(point1, point2, color);	//MyModel.ObjectColor
				DrawLine(point1, point3, color);
				DrawLine(point2, point3, color);

				if (MyModel.bounding_box)
				{
					glm::vec4 point01 = Changer * glm::vec4(MyModel.min_x, MyModel.min_y, MyModel.min_z, 1);
					glm::vec4 point02 = Changer * glm::vec4(MyModel.min_x, MyModel.min_y, MyModel.max_z, 1);
					glm::vec4 point03 = Changer * glm::vec4(MyModel.min_x, MyModel.max_y, MyModel.min_z, 1);
					glm::vec4 point04 = Changer * glm::vec4(MyModel.min_x, MyModel.max_y, MyModel.max_z, 1);
					glm::vec4 point05 = Changer * glm::vec4(MyModel.max_x, MyModel.min_y, MyModel.min_z, 1);
					glm::vec4 point06 = Changer * glm::vec4(MyModel.max_x, MyModel.min_y, MyModel.max_z, 1);
					glm::vec4 point07 = Changer * glm::vec4(MyModel.max_x, MyModel.max_y, MyModel.min_z, 1);
					glm::vec4 point08 = Changer * glm::vec4(MyModel.max_x, MyModel.max_y, MyModel.max_z, 1);

					point01[0] += viewport_width / 2;
					point02[0] += viewport_width / 2;
					point03[0] += viewport_width / 2;
					point04[0] += viewport_width / 2;
					point05[0] += viewport_width / 2;
					point06[0] += viewport_width / 2;
					point07[0] += viewport_width / 2;
					point08[0] += viewport_width / 2;

					point01[1] += viewport_height / 2;
					point02[1] += viewport_height / 2;
					point03[1] += viewport_height / 2;
					point04[1] += viewport_height / 2;
					point05[1] += viewport_height / 2;
					point06[1] += viewport_height / 2;
					point07[1] += viewport_height / 2;
					point08[1] += viewport_height / 2;

					DrawLine(point01, point02, glm::vec3(0, 0, 153));
					DrawLine(point01, point03, glm::vec3(0, 0, 153));
					DrawLine(point01, point05, glm::vec3(0, 0, 153));
					DrawLine(point02, point04, glm::vec3(0, 0, 153));
					DrawLine(point02, point06, glm::vec3(0, 0, 153));
					DrawLine(point03, point04, glm::vec3(0, 0, 153));
					DrawLine(point03, point07, glm::vec3(0, 0, 153));
					DrawLine(point04, point08, glm::vec3(0, 0, 153));
					DrawLine(point05, point06, glm::vec3(0, 0, 153));
					DrawLine(point05, point07, glm::vec3(0, 0, 153));
					DrawLine(point06, point08, glm::vec3(0, 0, 153));
					DrawLine(point07, point08, glm::vec3(0, 0, 153));
				}

				/* Model Axis */
				if (MyModel.ModelAxis)
				{
					MeshModel model = temp_scene.GetActiveModel();
					glm::mat4x4 transform = act_camera.GetOrthTransformation() * model.Axis_World_Trans;

					glm::vec4 x_left = transform * glm::vec4(model.min_x, (model.min_y + model.max_y) / 2, (model.min_z + model.max_z) / 2, 1);
					glm::vec4 x_right = transform * glm::vec4(model.max_x, (model.min_y + model.max_y) / 2, (model.min_z + model.max_z) / 2, 1);
					glm::vec4 y_top = transform * glm::vec4((model.min_x + model.max_x) / 2, model.max_y, (model.min_z + model.max_z) / 2, 1);
					glm::vec4 y_bottom = transform * glm::vec4((model.min_x + model.max_x) / 2, model.min_y, (model.min_z + model.max_z) / 2, 1);
					glm::vec4 z_top = transform * glm::vec4((model.min_x + model.max_x) / 2, (model.min_y + model.max_y) / 2, model.max_z, 1);
					glm::vec4 z_bottom = transform * glm::vec4((model.min_x + model.max_x) / 2, (model.min_y + model.max_y) / 2, model.min_z, 1);

					// start in the middle 
					x_left[0] += viewport_width / 2;
					x_left[1] += viewport_height / 2;
					x_right[0] += viewport_width / 2;
					x_right[1] += viewport_height / 2;
					y_top[0] += viewport_width / 2;
					y_top[1] += viewport_height / 2;
					y_bottom[0] += viewport_width / 2;
					y_bottom[1] += viewport_height / 2;
					z_top[0] += viewport_width / 2;
					z_top[1] += viewport_height / 2;
					z_bottom[0] += viewport_width / 2;
					z_bottom[1] += viewport_height / 2;

					DrawLine(x_left, x_right, glm::vec3(1, 0, 0));
					DrawLine(y_top, y_bottom, glm::vec3(0, 1, 0));
					DrawLine(z_top, z_bottom, glm::vec3(0, 0, 1));
				}

				/* World Axis */
				if (act_camera.WorldAxis)
				{
					
					glm::mat4x4 transformWorld = act_camera.OrthTransformations * glm::lookAt(act_camera.Eye, glm::vec3{ 0,0,0 }, glm::vec3{ 0,1,0 })
						* glm::inverse(act_camera.GetOrthTransformation());

					glm::vec4 point_left = transformWorld * glm::vec4(-viewport_width, 0, 0, 1);
					glm::vec4 point_right = transformWorld * glm::vec4(viewport_width, 0, 0, 1);
					glm::vec4 point_top = transformWorld * glm::vec4(0, viewport_height, 0, 1);
					glm::vec4 point_bottom = transformWorld * glm::vec4(0, -viewport_height, 0, 1);

					point_left[0] += viewport_width / 2;
					point_left[1] += viewport_height / 2;
					point_right[0] += viewport_width / 2;
					point_right[1] += viewport_height / 2;
					point_top[0] += viewport_width / 2;
					point_top[1] += viewport_height / 2;
					point_bottom[0] += viewport_width / 2;
					point_bottom[1] += viewport_height / 2;

					DrawLine(point_left, point_right, glm::vec3(0.8f, 0.8f, 0.8f));
					DrawLine(point_top, point_bottom, glm::vec3(0.8f, 0.8f, 0.8f));
				}
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
