# Computer Graphics 203.3710 Assignment #1
# Wireframe Viewer
# Cameras and others

## 1. Implement a camera with an orthographic projection, and remove the centering trick used in the second part of the assignment. Place a mesh in the scene such that it is visible(i.e. it is inside the view volume). Allow the user to change the view volume (e.g. up,down, left, right, near, far) and compare the result with two different view volume. Note:near and far should not affect the rendering in this assignment.

https://user-images.githubusercontent.com/116673155/209305683-60202113-85c0-4176-8675-c6539b27bd02.mp4

## 2. Allow the user to reposition the camera in the camera frame and the world frame using incremental changes. Explain how you implemented them and show results.

Here we create a new gui for camera transformation that u can see if we move the camera right (according to x) the model moving left ,and the same for y if we move the camera up the model will be moving down because it will be far from the model.
so we compute Camera transformation using the same idea when we compute model transformation at previous homework.It will be computed by the following:
CamTransformate = WorldTranslationCam_mat * xwCam_rotate * ywCam_rotate * zwCam_rotate * TranslationCam_mat * xCam_rotate * yCam_rotate * zCam_rotate;
First we have WorldTranslationCam_mat that responisble about translate in world according to camera then multiplied by xwCam_rotate * ywCam_rotate * zwCam_rotate that every one of these matrices responsible about rotating camira arround each axis in world
then multiplying by TranslationCam_mat that responsible about camera translation at model frame then multiplying by xCam_rotate * yCam_rotate * zCam_rotate that every one of these matrices responsible about rotating camira arround each axis at model frame.
Here a video showing incremental changes:
Uploading Assign1C-2.mp4…


## 3. Allow the user to set the view volume automatically based on the window’s size, and show the result on two window sizes. The proportions of the drawn object must remain the same! Verify your result using a sanity check and explain it in the report.

We set a new aspect ratio using a new Renderer variable called rend_for_changed_window and it get frameBufferWidth and frameBufferHeight that changed according to window's size.
Here is a video of how it working while changing window size:
https://user-images.githubusercontent.com/116673155/209406451-18fa6931-afae-40d8-9fd4-d89faa3ff26f.mp4



## 4. Implement a feature in the renderer that draws the axes of the model and the world frames (as short lines or arrows). Show the difference between transforming a model in the world frame and in the model frame while its axes are visible. Transforming in the model frame should keep the model axes fixed, while transforming in the world frame will transform the axes too.



## 5. Load a different mesh and pick two non-commuting transformations T1, T2. Compare the results of applying T1 in model frame and T2 in world frame vs. T1 in world frame and T1 in world frame. Explain the differences.



## 6. Implement and display the bounding box of the object, in local and world coordinates. Demonstrate the differences between them.

This is a picture of bounding box of bunny object:
![Assign1C-BoundingBox](https://user-images.githubusercontent.com/116673155/209305910-142616bd-b156-4fb6-9138-ee880f3eae36.png)

This is a video how it working if we rotate or translate or scale:

https://user-images.githubusercontent.com/116673155/209306142-f6d37b45-3838-48ef-8000-94194ee844f7.mp4

## 7. Compute and display the face normals and vertex normals.

Face Normals:
![Assign1C-FN](https://user-images.githubusercontent.com/116673155/209306809-362fc987-5873-4ab4-b8bb-5343c98d8775.png)

https://user-images.githubusercontent.com/116673155/209306844-bb6e1233-7da8-4d45-a615-1dbc41fbc937.mp4


Vertex Normals:
![Assign1C-VN](https://user-images.githubusercontent.com/116673155/209308064-dc18c78b-61da-4c05-b670-8b4840fb786e.png)

https://user-images.githubusercontent.com/116673155/209308093-5ab55c8d-e2e1-4c22-ab83-793ce3237711.mp4


## 8. Load a mesh, and move the camera away from it. implement a perspective projection and compare the orthographic projection vs. the perspective projection.

Here is a Orthographic Projection on bunny object:
![Orthographic-1C-8](https://user-images.githubusercontent.com/116673155/209394470-95b0f7ce-b828-410b-b3b6-a1ce07e88990.png)

And Here is a Perspective Projection on bunny object:
![Perspective-1C-8](https://user-images.githubusercontent.com/116673155/209394537-22221b4f-3d80-4c4c-9a7e-437758b17ec1.png)

We can see obviously the different if we look at bunny's legs.

## 9. Set the projection to perspective. Show the difference between changing the zoom (by changing the size of the frustum) and moving closer or farther from the model.



## 10. Set the camera position to (a, a, a) and orient it towards the origin using LookAt. Pick a such that the model is clearly visible.

Here is a picture when we set a to be 20:
![Assign1C-10(a=20)](https://user-images.githubusercontent.com/116673155/209306512-ad05461f-0083-4f95-8ba4-7cb5f4684547.png)

Here is a picture when we set a to be 33:
![Assign1C-10(a=33)](https://user-images.githubusercontent.com/116673155/209306621-3f304e90-4451-49f0-bdf9-94f0663ae9d9.png)


## 11. mplement a dolly zoom (the vertigo effect), and use a slider to control it. Show the result.



## 12. Put at least two screenshots showing multiple models and cameras on screen.



## 13. Put a screenshot showing as much of your GUI as possible.


## 14. Describe any additional features or bonuses you implemented.




