## Computer Graphics Assignment #3
## OpenGL renderer

## 1. The software renderer was built on top of OpenGL. The Renderer::SwapBuffer() function loads color_buffer into a texture on the GPU. Read the function Renderer::InitOpenGLRendering() and explain in the report how this texture is displayed. Remove all the now irrelevant code from the renderer.

The Renderer::InitOpenGLRendering() function is responsible for creating a mapping of the color buffer to a texture, 
and displaying that texture on the screen. To accomplish this, the function defines screen coordinates as two triangles, 
and defines coordinates for the texture that will be mapped onto those triangles. It then binds both coordinate buffers to OpenGL,
allocates space on the GPU, and copies the coordinate buffers to the GPU.

After initializing the buffers, the function creates a unique ID for the OpenGL texture and one for the vertex array object using glGenTextures() and glGenVertexArrays().
It then binds the vertex array object buffer to OpenGL using glBindVertexArray(). The buffer is responsible for transferring data to the GPU, so the function allocates and binds
the buffer using glGenBuffers(), glBindBuffer(), and glBufferData().

The function then loads and compiles shaders using InitShader(), which attaches OpenGL data to GLSL variables. To use the program, the function calls glUseProgram() with the program ID.
It then passes data to the shader, including vertex position and texture coordinates, and tells the shader where to look for those variables using glGetAttribLocation() 
and glVertexAttribPointer(). The function also sets uniform variables using glUniform1i().

With these initial settings established, the Renderer::SwapBuffer() function is called to load the color buffer into the texture on the GPU. Finally, the texture is displayed on 
the screen by mapping the texture onto the two triangles defined in Renderer::InitOpenGLRendering(), using glDrawArrays().


## 2. Update the MeshModel class to load the mesh on the GPU. Use the example OpenGL project as a reference.

We've updated the MeshModel class accordingly by using the example raised by the lecturer.
so we have changed MeshModel.h file by adding vertex struct and for loading a mesh on the GPU we need to create vertex buffer objects VBO's.so vbo and vao are OpenGL buffer handles used to store the vertex and index data of the model.These handles are used in the GetVAO() method to get the handle of the vertex array object associated with the model.

```
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

class MeshModel
{
protected:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> textureCoords;

	std::vector<Vertex> modelVertices;

	glm::mat4x4 modelTransform;
	glm::mat4x4 worldTransform;

	std::string modelName;

	glm::vec3 color;

	GLuint vbo;
	GLuint vao;

public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& modelName = "");
	virtual ~MeshModel();

	const glm::mat4x4& GetWorldTransformation() const;
	const glm::mat4x4& GetModelTransformation() const;

	void SetWorldTransformation(const glm::mat4x4& worldTransform);
	void SetModelTransformation(const glm::mat4x4& modelTransform);

	const glm::vec3& GetColor() const;
	void SetColor(const glm::vec3& color);

	const std::string& GetModelName();

	const std::vector<Vertex>& GetModelVertices();

	void TranslateModel(const glm::vec3& translationVector);
	void TranslateWorld(const glm::vec3& translationVector);

	void RotateXModel(double angle);
	void RotateYModel(double angle);
	void RotateZModel(double angle);
	void ScaleXModel(double factor);
	void ScaleYModel(double factor);
	void ScaleZModel(double factor);
	void ScaleModel(double factor);

	void RotateXWorld(double angle);
	void RotateYWorld(double angle);
	void RotateZWorld(double angle);
	void ScaleXWorld(double factor);
	void ScaleYWorld(double factor);
	void ScaleZWorld(double factor);
	void ScaleWorld(double factor);

	GLuint GetVAO() const;
};
```

and we've changed MeshModel.cpp file accordingly like the example,We've changed MeshModel constructor takes as input four vectors : faces,vertices,normals and textureCoords which represents the faces,vertices,normals and texture coordinates of the model.it also takes an optional modelName string.
  In the constructor,the model transformation matri,world transformation matrix and color are initialzed.Then for each face in faces, the three vertices are extracted and used to create a Vertex object,which is added to the modelVertices vector.This vector will be used to store vertex data that will be loaded into the GPU.In addition to this there is the implementation of the functions that we have defined at MeshModel.h file previously.
  

## 3. Write a vertex shader that applies all the transformations. Use the reference code, but pay attention to the differences between your code and the reference code. Copy the code the the report.

We used the refence code for now and updated the vertex shader to applies all the transformation.

```cpp
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// These outputs will be available in the fragment shader as inputs
out vec3 orig_fragPos;
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

void main()
{
	// Apply the model transformation to the 'position' and 'normal' properties of the vertex,
	// so the interpolated values of these properties will be available for us in the fragment shader
	orig_fragPos = vec3(vec4(pos, 1.0f));
  fragPos = vec3(model * vec4(pos, 1.0f));
	fragNormal = mat3(model) * normal;

	// Pass the vertex texture coordinates property as it is. Its interpolated value
	// will be avilable for us in the fragment shader
	fragTexCoords = texCoords;

	// This is an internal OpenGL variable, we must set a value to this variable
	gl_Position = projection * view *  model * vec4(pos, 1.0f);
}
  
```

## 4. Write a fragment shader that only outputs a constant color and paste it in the report.The goal for now is to ensure that the pipeline works. Lights will be added later.

We used the refence code for now and updated the fragment shader to outputs a constant color, red.

```cpp
#version 330 core

struct Material
{
	sampler2D textureMap;
};

// We set this field's properties from the C++ code
uniform Material material;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

	frag_color = vec4(1.0, 0.0, 0.0, 1.0); // Red
}

```

## 5. In the renderer, update all the relevant vertex attributes and uniforms, and paste the relevant piece of code to the report. If everything was done correctly, you should be able to view the mesh in your application.

Here we also used the refence code, and we can see now the red model is drown in the Mesh viewr. 

```cpp
void Renderer::Render(const std::shared_ptr<Scene>& scene)
{
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;

	int cameraCount = scene->GetCameraCount();
	if (cameraCount > 0)
	{
		int modelCount = scene->GetModelCount();

		if (modelCount > 0)
		{
			const Camera& camera = scene->GetActiveCamera();

			for (int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
			{
				std::shared_ptr<MeshModel> currentModel = scene->GetModel(currentModelIndex);

				// Activate the 'colorShader' program (vertex and fragment shaders)
				colorShader.use();

				// Set the uniform variables
				colorShader.setUniform("model", currentModel->GetWorldTransformation() * currentModel->GetModelTransformation());
				colorShader.setUniform("view", camera.GetViewTransformation());
				colorShader.setUniform("projection", camera.GetProjectionTransformation());
				colorShader.setUniform("material.textureMap", 0);

				// Set 'texture1' as the active texture at slot #0
				texture1.bind(0);

				// Drag our model's faces (triangles) in fill mode
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindVertexArray(currentModel->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
				glBindVertexArray(0);

				// Unset 'texture1' as the active texture at slot #0
				texture1.unbind(0);

				// Drag our model's faces (triangles) in line mode (wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBindVertexArray(currentModel->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
				glBindVertexArray(0);
			}
		}
	}
}
```

![trans](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-rashilmbariky/blob/master/Assignment3Report/Transformation.png)

## 6. Implement Phong shading in the fragment shader. To demonstrate that the performancehas now improved a lot, load one of the meshes from http://github.com/alecjacobson/common-3d-test-models  display it from several view-point and different lighting.

Here we use the same equation for computing Phong Shading that we use at previous assignment as a sum of Ambient+Diffuse+Specular ,we just move it to fshader file and define a Material variable and Light variable and using the example that have been shared to complete.

Here is an example of phong shading on teapot:

![teapot-6 1](https://user-images.githubusercontent.com/116673155/221545973-d8b6b83c-26b3-491a-b336-7ae51578a0c6.png)


![teapot-6 2](https://user-images.githubusercontent.com/116673155/221546015-b963c556-f433-4d36-b6a5-11349d4e8f6f.png)


![teapot-6 3](https://user-images.githubusercontent.com/116673155/221546069-e96958af-2f97-40b4-8607-1ba9601cc1a5.png)

Another example on beethoven:

![beethoven](https://user-images.githubusercontent.com/116673155/221546146-bdf1b355-b180-4125-a90f-a6d96c2cac12.png)

And as mentioned we downloaded beetle.obj mesh from the link and load it:

![beetle-6 1](https://user-images.githubusercontent.com/116673155/221546488-43c16ac8-b0b6-4b22-9c9a-ad29664501d4.png)


![beetle-6 2](https://user-images.githubusercontent.com/116673155/221546534-645d1d5b-78ef-44ad-8b4a-699bf1eb4f8a.png)


Here is with two lights:
![beetle-6 4 two lights](https://user-images.githubusercontent.com/116673155/221546577-24c73217-f5f8-4a6d-988a-a7e0d37b1ca6.png)

![beetle-6 5](https://user-images.githubusercontent.com/116673155/221546599-12546042-5e84-448b-b9ad-9866df906646.png)

## 7. Implement texture mapping. Use the example OpenGL project as reference. Enable at least one type of canonical project (for pairs: three): plane, cylinder or sphere. In addition, find a mesh with texture coordinates online and use them. Show your results in the report.

We used the OpenGL demo to load a texture mapping and we also added to the meshModel function to calculate the different kinds. 

### Plane

![Plane](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-rashilmbariky/blob/master/Assignment3Report/Plane2.png)
![Plane2](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-rashilmbariky/blob/master/Assignment3Report/Plane1.png)

### Cylinder

![Cylinder](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-rashilmbariky/blob/master/Assignment3Report/Cylinder.png)

### Sphere

![Spher](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-rashilmbariky/blob/master/Assignment3Report/Spher.png)

## 8. Implement one (for pairs: all) of the following                                                                                                                         (a) Normal mapping                                                                                                                                                       (b) Enviroment mapping                                                                                                                                                   (c) Toon shading (without the silhouette)                                                                                                                                 All of these can be done in many different ways and levels of complexity. Keep it simple! if in doubt, please ask. Explain what you did and show your results in the report.
