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
	Z_Buffer = new float* [viewport_width];
	colored = new glm::vec3* [viewport_width];

	int z = 0, c;
	while (z < viewport_width)
	{
		Z_Buffer[z] = new float[viewport_height];
		z++;
	}
	z = 0;
	while (z < viewport_width)
	{
		for (c = 0; c < viewport_height; c++)
			Z_Buffer[z][c] = -INFINITY;
		z++;
	}
	z = 0;
	while (z < viewport_width)
	{
		colored[z] = new glm::vec3[viewport_height];
		z++;
	}
	z = 0;
	while (z < viewport_width)
	{
		for (c = 0; c < viewport_height; c++)
			colored[z][c] = glm::vec3(0.5);
		z++;
	}
	InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer;
	//delete[] Z_Buffer;
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

	/*Ass 2a*/
	//Z_Buffer = new float[w * h];
	//SetMaxZBuffer();
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
	glm::ivec3 color(0 , 0 , 0);
	glm::mat4x4 Changer;
	glm::mat4x4 CameraTr = glm::mat4x4(1);
	int height_half=viewport_height/2, width_half=viewport_width/2;

	Camera& act_camera = temp_scene.GetActiveCamera();

	if(act_camera.orth)
		CameraTr = act_camera.GetOrthTransformation();
	else //if(act_camera.Perspective)
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

				point1[0] += width_half;
				point1[1] += height_half;
				point2[0] += width_half;
				point2[1] += height_half;
				point3[0] += width_half;
				point3[1] += height_half;
				
				
			/*	DrawLine(point1, point2, color);	
				DrawLine(point1, point3, color);
				DrawLine(point2, point3, color);*/
			}
		}
	}
	//SetMaxZBuffer(); ///
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

				point1[0] += width_half;
				point1[1] += height_half;
				point2[0] += width_half;
				point2[1] += height_half;
				point3[0] += width_half;
				point3[1] += height_half;

				glm::vec3 fn1 = p1 - p2;
				glm::vec3 fn2 = p2 - p3;
				glm::vec4 facenormal = glm::vec4(glm::cross(fn1, fn2), 0);
				glm::vec4 f_average = (point1 + point2 + point3);
				f_average /= 3;
				if (MyModel.face_normals)
				{
					facenormal *= 1200;
					DrawLine(f_average, f_average + facenormal, glm::vec3(12, 156, 31));

				}

				const Face& fv = MyModel.GetFace(i);
				glm::vec3 vn_color = glm::vec3(76, 153, 0);
				glm::vec4 vn1 = glm::vec4(MyModel.normals[fv.GetNormalIndex(0) - 1], 0);
				glm::vec4 vn2 = glm::vec4(MyModel.normals[fv.GetNormalIndex(1) - 1], 0);
				glm::vec4 vn3 = glm::vec4(MyModel.normals[fv.GetNormalIndex(2) - 1], 0);
				if (MyModel.vertex_normals)
				{
					glm::vec3 vn_color = glm::vec3(76, 153, 0);
					vn1 *= 15, vn2 *= 15, vn3 *= 15;
					DrawLine(point1, point1 + vn1, vn_color);
					DrawLine(point2, point2 + vn2, vn_color);
					DrawLine(point3, point3 + vn3, vn_color);
				}

				float min_x = min(min(point1.x,point2.x),point3.x);
				float max_x = max(max(point1.x, point2.x),point3.x);
				float min_y = min(min(point1.y, point2.y), point3.y);
				float max_y = max(max(point1.y, point2.y), point3.y);
				glm::vec3 pick_color = glm::vec3(rand() / 5000, rand() / 5000, rand() / 5000);

				if (scene.GetActiveModel().bounding_rectangle)
				{
					DrawLine(glm::vec2(min_x, min_y), glm::vec2(min_x, max_y), pick_color);
					DrawLine(glm::vec2(min_x, min_y), glm::vec2(max_x, min_y), pick_color);
					DrawLine(glm::vec2(max_x, max_y), glm::vec2(min_x, max_y), pick_color);
					DrawLine(glm::vec2(max_x, max_y), glm::vec2(max_x, min_y), pick_color);
				}
				//DrawLine(point1, point2, color);	//MyModel.ObjectColor
				//DrawLine(point1, point3, color);
				//DrawLine(point2, point3, color);

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

					point01[0] += width_half;
					point02[0] += width_half;
					point03[0] += width_half;
					point04[0] += width_half;
					point05[0] += width_half;
					point06[0] += width_half;
					point07[0] += width_half;
					point08[0] += width_half;

					point01[1] += height_half;
					point02[1] += height_half;
					point03[1] += height_half;
					point04[1] += height_half;
					point05[1] += height_half;
					point06[1] += height_half;
					point07[1] += height_half;
					point08[1] += height_half;

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
					x_left[0] += width_half;
					x_left[1] += height_half;
					x_right[0] += width_half;
					x_right[1] += height_half;
					y_top[0] += width_half;
					y_top[1] += height_half;
					y_bottom[0] += width_half;
					y_bottom[1] += height_half;
					z_top[0] += width_half;
					z_top[1] += height_half;
					z_bottom[0] += width_half;
					z_bottom[1] += height_half;

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

				glm::vec3 fc;
				int shade = scene.GetActiveModel().shade_type;
				if (shade == 0)
					fc = ChooseColor(scene.GetActiveModel(), temp_scene, point1, facenormal);
				
				for (int m = min_x; m < max_x + 1; m++)
				{
					for (int n = min_y; n < max_y + 1; n++)
					{
						if (m < viewport_width && n < viewport_height && m>0 && n>0)
						{
							bool is_in = InsidetheTriangle(m, n, point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
							if (is_in)
							{
								float Z, A, A1, A2, A3;
								A1 = area(m, n, point2.x, point2.y, point3.x, point3.y);
								A2 = area(m, n, point1.x, point1.y, point3.x, point3.y);
								A3 = area(m, n, point1.x, point1.y, point2.x, point2.y);
								A = A1 + A2 + A3;
								Z = (((A1 / A) * point1.z) + ((A2 / A) * point2.z) + ((A3 / A) * point3.z));
								
								if (Z > maxbufferZ)
									maxbufferZ = Z;
								if (Z < minbufferZ)
									minbufferZ = Z;
								if (Z > Z_Buffer[m][n])
								{
									if (shade == 0)
										colored[m][n] = fc;
									Z_Buffer[m][n] = Z;
								}
							}
						}
					}
				}
				/* Color Triangles */
				/*if (MyModel.ZBufferOn == 1)
				{
					ZBuffer(glm::vec3(point1.x, point1.y, point1.z), glm::vec3(point2.x, point2.y, point2.z), glm::vec3(point3.x, point3.y, point3.z));
				}
				if (MyModel.GetTriColors() == 1)
				{
					PaintTriangles(glm::vec3(point1.x, point1.y, point1.z), glm::vec3(point2.x, point2.y, point2.z), glm::vec3(point3.x, point3.y, point3.z), MyModel,temp_scene,facenormal);
				}

				if (MyModel.GetTriColors() == 2)
				{
					PaintTrianglesGray();
				}*/

			}
		}
	}
	for (int i = 0; i < viewport_width; i++)
	{
		for (int j = 0; j < viewport_height; j++)
		{
			if (Z_Buffer[i][j] > -INFINITY)
			{
				float sub = 1 - ((maxbufferZ - Z_Buffer[i][j]) / (maxbufferZ - minbufferZ));
				PutPixel(i, j, sub * colored[i][j]);
				Z_Buffer[i][j] = -INFINITY;
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

//void Renderer::PaintTriangles(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, MeshModel& Model,Scene& scene,const glm::vec3 normal)
//{	
//	MeshModel MyModel = Model;
//
//	float minY = min(min(p1.y, p2.y), p3.y);
//	float maxY = max(max(p1.y, p2.y), p3.y);
//	float minX = min(min(p1.x, p2.x), p3.x);
//	float maxX = max(max(p1.x, p2.x), p3.x);
//	
//	/* Generate a random color for every triangle */
//	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//	float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//	glm::vec3 random_color(r, g, b);
//
//	for (int j = maxY; j >= minY; j--)
//	{
//		for (int i = minX; i <= maxX; i++)
//		{
//			if (InsidetheTriangle(i, j, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y))
//			{
//				glm::vec3 z = Z_Calculate(i, j, p1, p2, p3, p1, p2, p3);
//				z =z*glm::vec3(-1);
//				if (MyModel.ZBufferOn == 1)
//				{
//					
//					if (z.z <= GetZ(i, j))
//					{
//						PutPixel(i, j, random_color);
//					}
//				}
//				else
//				{
//					random_color = ChooseColor(MyModel, scene, p1, normal);
//					PutPixel(i, j, random_color);
//					SetZ(i, j, z.z);
//				}
//			}
//		}
//	}
//}

bool Renderer::InsidetheTriangle(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3)
{
	// Calculate the areas of the three sub-triangles formed by the point (x, y) and the triangle's vertices
	float A1 = area(x, y, x2, y2, x3, y3);
	float A2 = area(x1, y1, x, y, x3, y3);
	float A3 = area(x1, y1, x2, y2, x, y);

	// Calculate the area of the triangle formed by the three vertices
	float A = area(x1, y1, x2, y2, x3, y3);

	// If the sum of the areas of the sub-triangles is equal to the area of the triangle, then (x, y) is inside the triangle
	return A == A1 + A2 + A3;
}

/* A utility function to calculate area of triangle formed by (x1, y1), (x2, y2) and (x3, y3) */
float Renderer::area(int x1, int y1, int x2, int y2, int x3, int y3)
{
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

///* Z-buffer */
//void Renderer::ZBuffer(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
//{
//	float min_Y = min(min(p1.y, p2.y), p3.y);
//	float max_Y = max(max(p1.y, p2.y), p3.y);
//	float min_X = min(min(p1.x, p2.x), p3.x);
//	float max_X = max(max(p1.x, p2.x), p3.x);
//
//	// Loop over the pixels of the bounding rectangle
//	for (int y = min_Y; y <= max_Y; y++) 
//	{
//		for (int x = min_X; x <= max_X; x++) 
//		{
//			// Check if the point (x, y) is inside the triangle
//			if (InsidetheTriangle(x, y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y)) 
//			{
//				glm::vec3 z = Z_Calculate(x, y, p1, p2, p3, p1, p2, p3);
//
//				if (z.z <= GetZ(x, y)) 
//				{
//					maxbufferZ = max(maxbufferZ, z.z);
//					minbufferZ = min(minbufferZ, z.z);
//					SetZ(x, y, z.z);
//				}
//			}
//		}
//	}
//}

//glm::vec3 Renderer::Z_Calculate(int x, int y, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& val1, const glm::vec3& val2, const glm::vec3& val3)
//{
//	float A1 = area(x, y, p1.x, p1.y, p2.x, p2.y);
//	float A2 = area(x, y, p1.x, p1.y, p3.x, p3.y);
//	float A3 = area(x, y, p3.x, p3.y, p2.x, p2.y);
//	float A = A1 + A2 + A3;
//
//	return (((A1 / A) * val3) + ((A2 / A) * val2) + ((A3 / A) * val1));
//}

//float Renderer::GetZ(int i, int j)
//{
//	// Check if the point (i, j) is inside the viewport
//	if (i < 0 || i >= viewport_width || j < 0 || j >= viewport_height) 
//	{
//		return 0;
//	}
//
//	// Return the z-value of the point (i, j) from the Z-buffer
//	return Z_Buffer[Z_INDEX(viewport_width, i, j)];
//}

//void Renderer::SetZ(int i, int j, float z)
//{	
//	// Check if the point (i, j) is inside the viewport
//	if (i < 0 || i >= viewport_width || j < 0 || j >= viewport_height) 
//	{
//		return;
//	}
//
//	// Set the z-value of the point (i, j) in the Z-buffer
//	Z_Buffer[Z_INDEX(viewport_width, i, j)] = z;
//}

//void Renderer::SetMaxZBuffer()
//{
//	minbufferZ = FLT_MAX;
//	maxbufferZ = FLT_MIN;
//	for (int i = 0; i < viewport_width; i++)
//	{
//		for (int j = 0; j < viewport_height; j++)
//		{
//			Z_Buffer[Z_INDEX(viewport_width, i, j)] = FLT_MAX;
//
//			//color_buffer[INDEX(viewport_width, i, j, 0)] = 0.f;
//			//color_buffer[INDEX(viewport_width, i, j, 1)] = 0.f;
//			//color_buffer[INDEX(viewport_width, i, j, 2)] = 0.f;
//		}
//	}
//}

/* Gray Scale */
//void Renderer::PaintTrianglesGray() 
//{
//	// Calculate the scale and offset parameters for the linear transformation
//	float a = 1 / (maxbufferZ - minbufferZ);
//	float b = -1 * a * minbufferZ;
//
//	for (int i = 0; i < viewport_width; i++) 
//	{
//		for (int j = 0; j < viewport_height; j++) 
//		{
//			// Check if the point (i, j) has a valid z-value
//			float z = GetZ(i, j);
//			if (z != FLT_MAX) 
//			{
//				// Calculate the gray color of the point (i, j) using the z-value
//				float c = 1 - (a * z + b);
//				glm::vec3 color(c, c, c);
//
//				// Set the color of the point (i, j)
//				PutPixel(i, j, color);
//			}
//		}
//	}
//}

glm::vec3 Renderer::ChooseColor(MeshModel& Model, Scene& scene, glm::vec3 a_point, glm::vec3 normal)
{
	int lightsize = scene.lights.size();
	int c = 0;
	glm::vec3 color = glm::vec3(0);
	/*Defining vectors to save each reflection result*/
		glm::vec3 Ia = glm::vec3(0); /*ambient*/ 
		glm::vec3 Id = glm::vec3(0); /*diffuse*/
		glm::vec3 Is = glm::vec3(0); /*specular*/

		while (c < lightsize)
		{
			/* Ambient Reflection - Ia=La*Ka */
			/* La-ambient light intensity, Ka-ambient color*/
			glm::vec3 La = scene.lights[0]->ambient_ref;
			glm::vec3 Ka = scene.GetActiveModel().Ambient_ref;
			Ia = La * Ka;

			/* Diffuse Reflection - Id=Kd(l*n)Ld */
			/* Ld-Source diffuse intensity, Kd-Surface diffuse reflection coefficient ,(l*n)=cos teta */
			glm::vec3 Kd = scene.GetActiveModel().Diffuse_ref;
			glm::vec3 location = glm::vec3(scene.lights[0]->TranslateMat[3][0], scene.lights[0]->TranslateMat[3][1], scene.lights[0]->TranslateMat[3][2]);
			glm::vec3 l = location-a_point;
			glm::vec3 Ld = scene.lights[0]->diffuse_ref;
			Id = Kd * max(0.0f, (glm::dot(l, glm::normalize(normal)))) * Ld;

			/* Specular Reflection - Is=Ks(r*v)^alfa*Ls */
			/*Ks-Surface specular reflection coefficient, (r*v)=cos teta, alfa-Shininess coefficient, Ls-Source specular intensity */
			glm::vec3 Ks = scene.GetActiveModel().Specular_ref;
			glm::vec3 r = l - 2 * glm::dot(l, glm::normalize(normal)) * glm::normalize(normal);
			glm::vec3 v = a_point - scene.GetActiveCamera().Eye;
			glm::vec3 Ls = scene.lights[0]->specular_ref;
			Is = Ks * pow(glm::dot(r, v), (scene.GetActiveModel().alfa / 800)) * Ls;
			glm::vec4 r_4 = glm::vec4(r, 1);
			glm::vec4 l_4 = glm::vec4(l, 1);
			r_4 /= 50;
			l_4 /= -50;

			color += (Ia+Id);
			c++;
		}
	return color;
}