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

## 4. Implement the z-buffer algorithm. Create a buffer the same size as the color buffer but with one floating point value per pixel. Update the z-buffer in the DrawTriangle function. The issue mentioned above should not occur anymore. The z-buffer can be visualized as a grey scale image (as in the lecture). Show side-by-side pictures of the color buffer and z-buffer, for several models or viewpoints.

### Z-Buffer Algorithm: 
To implement the Z-buffer algorithm we added an array (buffer) storing the depth (Z coordinate) of each pixel and using it to decide which pixel should be drawn at each screen coordinate.

first We added a SetMaxZBuffer function to set all values in the buffer to a maximum value.
Next, in the ZBuffer function: 
1. We check if the pixel is inside the triangle.
2. For each pixel within the triangle's bounds:
    - Calculate the Z value, using Z_Calculate function (2).
    - If the calculated Z value is less than the value stored in the Z-buffer at that pixel location if (z.z <= GetZ(x, y)), update the Z-buffer with the new Z value.
    - We call the PaintTriangles and draw the pixel with the appropriate color.

(2) Z_Calculate function: calculates the Z value of a pixel (x, y) within a triangle formed by three vertices with known Z values (p1, p2, p3) by interpolating the Z values of the triangle's vertices based on the areas of the sub-triangles formed by the point (x, y) and the triangle's vertices.

### Grey Scale:
To do the grey scale image we added a function that iterates over all the points in the viewport and checks if each point has a valid z-value. If a point has a valid z-value, the function calculates a gray color for the point based on its z-value using a linear transformation, which scales and offsets the z-value to the range [0, 1]. The function then sets the color of the point to the calculated gray color using the PutPixel function. This results in a grayscale image where points closer to the viewer are darker and points farther from the viewer are lighter.

![Bunny](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-rashilmbariky/blob/master/Assignment2Report/GreyScale%20(1).png)
![Banana](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-rashilmbariky/blob/master/Assignment2Report/GreyScale%20(2).png)



Z-Buffer Off            |  Z-Buffer On
:-------------------------:|:-------------------------:
![Bunny]([../../main/Pictures_As2/1a.png "Bunny No Z-Buffer"](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-rashilmbariky/blob/master/Assignment2Report/Z_Buffer%20Off%20(1).png))  |  ![bunny]([../../main/Pictures_As2/1b.png "Bunny Z-Buffer"](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-rashilmbariky/blob/master/Assignment2Report/Z_Buffer%20On%20(1).png))

