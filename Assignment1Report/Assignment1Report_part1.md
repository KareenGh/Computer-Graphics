# Assignment 1 Report : Part 1 

## Bresenham Algorithm 
**_We used Bresenham Algorithm to draw a line, we checked when we had to do switch and when to do a reflect, 
and write the algorithm to put pixels in the right place_** 

	void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
	{
		int x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;
		int deltaX = abs(x2 - x1);
		int deltaY = abs(y2 - y1);
		int Switch = 0;

		if (deltaX <= deltaY) // switch = 1 if we have to switch betwen x and y 
		{
			Switch = 1;
			x1 = p1.y;
			x2 = p2.y;
			y1 = p1.x;
			y2 = p2.x;
		}
		if (x1 > x2) // if p1 is in the right side of p2 switch th x-es
		{
			if (Switch == 1)
			{
				x1 = p2.y;
				x2 = p1.y;
				y1 = p2.x;
				y2 = p1.x;
			}
			else
			{
				x1 = p2.x;
				x2 = p1.x;
				y1 = p2.y;
				y2 = p1.y;
			}

		}
		deltaX = abs(x2 - x1);
		deltaY = abs(y2 - y1);

		int e = -deltaX;
		int x = x1;

		while (x1 <= x2)
		{
			if (e >= 0)
			{
				y1 < y2 ? y1++ : y1--; // reflect
				if (Switch == 0)
					PutPixel(x1, y1, color);
				else
					PutPixel(y1, x1, color);

				e -= 2 * deltaX;
			}
			else
			{
				if (Switch == 0)
					PutPixel(x1, y1, color);
				else
					PutPixel(y1, x1, color);
			}
			x1++;
			e += 2 * deltaY;
		}	

	}


## Draw Circle Algorithm
**_We used the circle coordinates to draw a circle using Bresenham Algorithm to draw a line from the center of the page_** 

**_(X0 + r * sin((2*i*pi)/a) , y0 + r * cos((2*i*pi)/a))_**

	void Renderer::Render(const Scene& scene)
	{
		int half_width = viewport_width / 2;
		int half_height = viewport_height / 2;
		glm::ivec2 p1;
		glm::ivec2 p2;
		glm::vec3 color(1, 0.57, 1.31);
		int r = 750, a = 100; 
		p1.x = half_width;
		p1.y = half_height;
	
		for (int i = 0; i < 100; i++) //number of lines
		{
			p2.x = p1.x + r * sin((2 * M_PI * i) / a);
			p2.y = p1.y + r * cos((2 * M_PI * i) / a);
			if (i > 25 && i <= 50)
			{
				color.x = 1;
				color.y = 1;
				color.z = 1;
			}
			if (i > 50 && i <= 75)
			{
				color.x = 1.55;
				color.y = 1;
				color.z = 0;
			}
			if (i > 75 && i < 100)
			{
				color.x = 0.75;
				color.y = 0;
				color.z = 0.5;
			}
			DrawLine(p1, p2, color);
		}
	}

## The circle
![Circle](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-rashilmbariky/blob/master/Assignment1Report/SanityCheck.jpeg)

## Our drawing
![Our drawing](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-rashilmbariky/blob/master/Assignment1Report/OurDrawing.jpeg)
