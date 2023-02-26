## Computer Graphics Assignment #3
## OpenGL renderer

## 1.The software renderer was built on top of OpenGL. The Renderer::SwapBuffer() function loads color_buffer into a texture on the GPU. Read the function Renderer::InitOpenGLRendering() and explain in the report how this texture is displayed. Remove all the now irrelevant code from the renderer.

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


## 2.Update the MeshModel class to load the mesh on the GPU. Use the example OpenGL project as a reference.

## 3.Write a vertex shader that applies all the transformations. Use the reference code, but pay attention to the differences between your code and the reference code. Copy the code the the report.

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

## 4.Write a fragment shader that only outputs a constant color and paste it in the report.The goal for now is to ensure that the pipeline works. Lights will be added later.

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

## 5.In the renderer, update all the relevant vertex attributes and uniforms, and paste the relevant piece of code to the report. If everything was done correctly, you should be able to view the mesh in your application.

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

				colorShader.setUniform("color", glm::vec3(0, 0, 0));

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


## 6.Implement Phong shading in the fragment shader. To demonstrate that the performancehas now improved a lot, load one of the meshes from http://github.com/alecjacobson/common-3d-test-models  display it from several view-point and different lighting.

## 7.Implement texture mapping. Use the example OpenGL project as reference. Enable at least one type of canonical project (for pairs: three): plane, cylinder or sphere. In addition, find a mesh with texture coordinates online and use them. Show your results in the report.

## 8.Implement one (for pairs: all) of the following                                                                                                                         (a) Normal mapping                                                                                                                                                       (b) Enviroment mapping                                                                                                                                                   (c) Toon shading (without the silhouette)                                                                                                                                 All of these can be done in many different ways and levels of complexity. Keep it simple! if in doubt, please ask. Explain what you did and show your results in the     report.
