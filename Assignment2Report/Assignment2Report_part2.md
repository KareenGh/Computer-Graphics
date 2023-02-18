## Computer Graphics Assignment #2
## Lighting and shading
## Implementing Lighting and Shading

## 1. Implement the ability to add point lights to the scene and allow the user to change its position. Each light source should have three RGB components, for the ambient, diffuse and specular reflections, so 9 values in total. In addition, allow the user to specify a material for each model. Similarly to light, each material should have RGB values for ambient diffuse and specular reflections. Compute the lighting for the ambient light and show the result in the report. Specify the colors you chose and why you got the result that you got.

First of all we have defined a new class Called Light,we add the option for moving the light and we have choosed ColorEdit3 for letting the user choose wanted color for every reflection type 
and specify a material for each model to.As we learned at lecture to compute lighting for ambient light is by using this equation - Ia = La * Ka when :
La-ambient light intensity
Ka-ambient color
You can see that we got one colored model and not affected by any light moving

Here we will show you the results we've got:
Here u can see when the Ambient light is white we got a model colored with the material color ,We applied Ambient Reflection on bunny model when Ambient Material color is (0,0,255)

![Ambient-blue](https://user-images.githubusercontent.com/116673155/219873189-9930f721-3872-4c54-9b64-32d36516cc90.png)

And there is another example for Ambient Reflection applied on bunny model when ambient light color is black , we can see in this case no mater what Ambient material color is :

![Ambient-black-light](https://user-images.githubusercontent.com/116673155/219873495-7d9d4a04-37ad-4821-9ab2-fdd3ffa0bdfd.png)

This is an Ambient Reflection applied on bunny model when Ambient Material color is (223,234,60) and Ambient light color is (158,0,255):

![AmbientRefTwoDiffColors](https://user-images.githubusercontent.com/116673155/219874053-3a8dc392-0e20-49de-9a7c-4b8fb6bd99ff.png)

Here is a video showing that changing light position didnt affect:

https://user-images.githubusercontent.com/116673155/219874719-f3ded833-9a68-43a2-9495-110bcec66691.mp4


## 2.Compute the diffuse component with flat shading and show the result using the same scene.

For computing diffuse component with flat shading we first calculated face normal at each point on the surface of an object,then we calculated amount light that reflected by the surface in the direction of light source.
the amount of light reflected is proportional to the cosine of the angel between surface normal and the direction of light source.
For doing that we use the equation for Diffuse reflect that we have seen at the lecture:
Diffuse Reflection - Id=Kd(l*n)Ld
when: Ld-Source diffuse intensity, Kd-Surface diffuse reflection coefficient ,(l*n)=cos teta

Here we will show an example on same scene on bunny model:

![Diffuse-Flat Shading](https://user-images.githubusercontent.com/116673155/219882424-94a05771-3261-462d-88c4-bb89098cc112.png)


## 3.For pairs only: Compute Gouraud shading and compare the result to the previous result.






## 4.Implement the function that computes the reflection vector used for specular lighting.Show the direction of the light from the center of each face and the direction of the reflected light as short lines (similar to how normal vectors were drawn in the first assignment).

First of all we add Check box called reflection_vec to our Gui started as not choosen.

Here is reflection vector applied on bunny model:

![RV-bunny](https://user-images.githubusercontent.com/116673155/219893678-ff9cc184-d7f5-4ac1-81ee-05fd2c696e65.png)

And this is another example for reflection vector applied on teapot:

![RV-teapot](https://user-images.githubusercontent.com/116673155/219893861-5e72a36e-b3a3-462f-b3b5-5ef66644fe0d.png)


