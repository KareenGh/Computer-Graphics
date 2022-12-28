#Computer Graphics Assignment #2
#Lighting and shading
#Triangle fill and z-buffer


## 1.For debugging purposes, change the DrawTriangle function you implement in Assignment 1 such that it will draw the bounding rectangle for each triangle in screen space. The
color of a rectangle should be related to the depth of the triangle, and you are free to choose exactly how. Explain, and show an image in the report. Allow the user to toggle this feature on and off.

Here we create a new gui called "Triangle fill and z-buffer" with a checkbox called "Bounding Rectangle" when we press on it ,it will show as ounding rectangle for each triangle in screen space
and we did that by calculating min_x,min_y,max_x,max_y we draw for every model face a rectangle by drawing 4 lines using drawLine function from first assignment and defining a color that picked randomly.
It's a screenshot and a video that showing our implement:
