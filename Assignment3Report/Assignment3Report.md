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

## 4.Write a fragment shader that only outputs a constant color and paste it in the report.The goal for now is to ensure that the pipeline works. Lights will be added later.

## 5.In the renderer, update all the relevant vertex attributes and uniforms, and paste the relevant piece of code to the report. If everything was done correctly, you should be able to view the mesh in your application.

## 6.Implement Phong shading in the fragment shader. To demonstrate that the performancehas now improved a lot, load one of the meshes from http://github.com/alecjacobson/common-3d-test-models  display it from several view-point and different lighting.

## 7.Implement texture mapping. Use the example OpenGL project as reference. Enable at least one type of canonical project (for pairs: three): plane, cylinder or sphere. In addition, find a mesh with texture coordinates online and use them. Show your results in the report.

## 8.Implement one (for pairs: all) of the following                                                                                                                         (a) Normal mapping                                                                                                                                                       (b) Enviroment mapping                                                                                                                                                   (c) Toon shading (without the silhouette)                                                                                                                                 All of these can be done in many different ways and levels of complexity. Keep it simple! if in doubt, please ask. Explain what you did and show your results in the     report.
