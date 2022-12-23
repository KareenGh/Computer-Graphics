# Computer Graphics 203.3710 Assignment #1
# Wireframe Viewer
# Cameras and others

## 1. Implement a camera with an orthographic projection, and remove the centering trick used in the second part of the assignment. Place a mesh in the scene such that it is visible
(i.e. it is inside the view volume). Allow the user to change the view volume (e.g. up,down, left, right, near, far) and compare the result with two different view volume. Note:
near and far should not affect the rendering in this assignment.

## 2. Allow the user to reposition the camera in the camera frame and the world frame using incremental changes. Explain how you implemented them and show results.



## 3. Allow the user to set the view volume automatically based on the window’s size, and show the result on two window sizes. The proportions of the drawn object must remain the same! Verify your result using a sanity check and explain it in the report.



## 4. Implement a feature in the renderer that draws the axes of the model and the world frames (as short lines or arrows). Show the difference between transforming a model in the world frame and in the model frame while its axes are visible. Transforming in the model frame should keep the model axes fixed, while transforming in the world frame will transform the axes too.



## 5. Load a different mesh and pick two non-commuting transformations T1, T2. Compare the results of applying T1 in model frame and T2 in world frame vs. T1 in world frame and T1 in world frame. Explain the differences.



## 6. Implement and display the bounding box of the object, in local and world coordinates. Demonstrate the differences between them.


## 7. Compute and display the face normals and vertex normals.


## 8. Load a mesh, and move the camera away from it. implement a perspective projection and compare the orthographic projection vs. the perspective projection.



## 9. Set the projection to perspective. Show the difference between changing the zoom (by changing the size of the frustum) and moving closer or farther from the model.



## 10. Set the camera position to (a, a, a) and orient it towards the origin using LookAt. Pick a such that the model is clearly visible.


## 11. mplement a dolly zoom (the vertigo effect), and use a slider to control it. Show the result.



## 12. Put at least two screenshots showing multiple models and cameras on screen.



## 13. Put a screenshot showing as much of your GUI as possible.


## 14. Describe any additional features or bonuses you implemented.




