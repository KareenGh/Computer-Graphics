## Computer Graphics Assignment #2
## Lighting and shading
## Implementing Lighting and Shading

## 1. Implement the ability to add point lights to the scene and allow the user to change its
position. Each light source should have three RGB components, for the ambient, diffuse
and specular reflections, so 9 values in total. In addition, allow the user to specify a
material for each model. Similarly to light, each material should have RGB values for
ambient diffuse and specular reflections. Compute the lighting for the ambient light and
show the result in the report. Specify the colors you chose and why you got the result
that you got.

First of all we have defined a new class Called Light,we add the option for moving the light and we have choosed ColorEdit3 for letting the user choose wanted color for every reflection type 
and specify a material for each model to.As we learned at lecture to compute lighting for ambient light is by using this equation - Ia = La * Ka when :
La-ambient light intensity
Ka-ambient color

Here we will show you the results we've got:
