## Computer Graphics Assignment #2
## Lighting and shading
## Triangle fill and z-buffer

## 1.For debugging purposes, change the DrawTriangle function you implement in Assignment 1 such that it will draw the bounding rectangle for each triangle in screen space. The color of a rectangle should be related to the depth of the triangle, and you are free to choose exactly how. Explain, and show an image in the report. Allow the user to toggle this feature on and off.

Here we create a new gui called "Triangle fill and z-buffer" with a checkbox called "Bounding Rectangle" when we press on it ,it will show us bounding rectangle for each triangle in screen space and we did that by calculating min_x,min_y,max_x,max_y we draw for every model face a rectangle by drawing 4 lines using drawLine function from first assignment and defining a color that picked randomly.
It's a screenshot and a video that showing our implement:

![Assignment2a-1-BR](https://user-images.githubusercontent.com/116673155/209837146-08ca3c9b-6d67-45e1-8f62-0913a6d94dcc.png)


https://user-images.githubusercontent.com/116673155/209837168-62286a13-15f3-4de8-bf81-f4870828ab59.mp4


## 2. We implemented the edge walking approach.


## 3. Change the DrawTriangle function such that it fills the entire triangle. Place a model in the scene such that it is clearly visible. Pick a random color for every triangle and place a screenshot of the result in your report. Note that you will see triangles overlapping; triangles in the back might be occluding triangles in the front.

In order to do this we added PaintTriangles function:
1. Calculate the min and max x and y coordinates of the triangle by taking the minimum and maximum values of the x and y coordinates of the three vertices.
2. Generate a random color.
3. Iterates over the pixels of the triangle, and for each pixel, it checks whether the pixel is inside the triangle using the InsidetheTriangle function (1).
4. The function calls the PutPixel function to set the color of the pixel to the random color.

(1) InsidetheTriangle function: The InsidetheTriangle function is used to determine whether a given point (x, y) lies inside a triangle defined by three points (x1, y1), (x2, y2), and (x3, y3).

![Chain](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-rashilmbariky/blob/master/Assignment2Report/Colorful%20Models%202a%20(1).png)
![Tea](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-rashilmbariky/blob/master/Assignment2Report/Colorful%20Models%202a%20(2).png)
![Bunny](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-rashilmbariky/blob/master/Assignment2Report/Colorful%20Models%202a%20(3).png)

