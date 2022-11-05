/******************* The algorithm for the circule ***********************/
void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
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

/*********************************************************************************/

void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	// draw circle
	glm::ivec2 p1;
	glm::ivec2 p2;
	glm::vec3 color(1, 0.57, 1.31);
	int r = 750, a = 100; // r = length , a = number of lines
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



/******************* The algorithm for our drowing **********************/ 
void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	// draw circle
	glm::ivec2 p1;
	glm::ivec2 p2;
	glm::vec3 color(1 , 0.57 , 1.31);
	p1.x = half_width;
	p1.y = half_height + 500;
	p2.x = half_width + 700;
	p2.y = half_height + 500;
	DrawLine(p1, p2, color);
	p2.x = half_width + 250;
	p2.y = half_height + 250;
	DrawLine(p1, p2, color);
	p2.x = half_width - 250;
	p2.y = half_height + 250;
	DrawLine(p1, p2, color);
	p1.x = half_width + 950;
	p1.y = half_height + 250;
	p2.x = half_width + 700;
	p2.y = half_height + 500;
	DrawLine(p1, p2, color);
	p1.x = half_width + 950;
	p1.y = half_height + 250;
	p2.x = half_width - 250;
	p2.y = half_height + 250;
	DrawLine(p1, p2, color);
	p1.x = half_width - 250;
	p1.y = half_height + 250;
	p2.x = half_width - 250;
	p2.y = half_height - 250;
	DrawLine(p1, p2, color);
	p1.x = half_width + 250;
	p1.y = half_height + 250;
	p2.x = half_width + 250;
	p2.y = half_height - 250;
	DrawLine(p1, p2, color);
	p1.x = half_width - 250;
	p1.y = half_height - 250;
	p2.x = half_width + 250;
	p2.y = half_height - 250;
	DrawLine(p1, p2, color);
	p1.x = half_width + 950;
	p1.y = half_height + 250;
	p2.x = half_width + 950;
	p2.y = half_height - 250;
	DrawLine(p1, p2, color);
	p1.x = half_width + 950;
	p1.y = half_height - 250;
	p2.x = half_width + 250;
	p2.y = half_height - 250;
	DrawLine(p1, p2, color);
	p1.x = half_width - 100;
	p1.y = half_height - 250;
	p2.x = half_width - 100;
	p2.y = half_height;
	DrawLine(p1, p2, color);
	p1.x = half_width + 50;
	p1.y = half_height;
	DrawLine(p1, p2, color);
	p2.x = half_width + 50;
	p2.y = half_height - 250;
	DrawLine(p1, p2, color);

	p1.x = 0;
	p1.y = half_height - 250;
	p2.x = half_width + 10000;
	p2.y = p1.y;
	DrawLine(p1, p2, color);

	glm::vec3 color1(0, 1, 0);
	p1.x = 250;
	p1.y = half_height - 250;
	p2.x = 250;
	p2.y = half_height + 150;
	DrawLine(p1, p2, color1);
	p1.x = 400;
	p1.y = half_height - 250;
	p2.x = 400;
	p2.y = half_height + 150;
	DrawLine(p1, p2, color1);

	p1.x = 100;
	p1.y = half_height + 150;
	p2.x = 550;
	p2.y = half_height + 150;
	DrawLine(p1, p2, color1);
	p2.x = 250;
	p2.y = half_height + 250;
	DrawLine(p1, p2, color1);
	p1.x = 400;
	p1.y = half_height + 250;
	p2.x = 550;
	p2.y = half_height + 150;
	DrawLine(p1, p2, color1);
	p2.x = 550;
	p2.y = half_height + 250;
	DrawLine(p1, p2, color1);
	p1.x = 400;
	p1.y = half_height + 350;
	DrawLine(p1, p2, color1);
	p1.x = 100;
	p1.y = half_height + 250;
	p2.x = 250;
	p2.y = half_height + 250;
	DrawLine(p1, p2, color1);
	p2.x = 250;
	p2.y = half_height + 350;
	DrawLine(p1, p2, color1);
	p1.x = 100;
	p1.y = half_height + 350;
	DrawLine(p1, p2, color1);
	p2.x = 325;
	p2.y = half_height + 575;
	DrawLine(p1, p2, color1);
	p1.x = 400;
	p1.y = half_height + 350;
	p2.x = 550;
	p2.y = half_height + 350;
	DrawLine(p1, p2, color1);
	p1.x = 325;
	p1.y = half_height + 575;
	DrawLine(p1, p2, color1);

	p1.x = 0;
	p1.y = half_height - 250;
	p2.x = 30;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 60;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 90;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 120;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 150;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 180;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 210;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 240;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 270;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 300;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 330;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 360;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 390;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 420;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 450;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 480;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 510;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 540;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 570;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 600;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 630;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 660;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 690;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 720;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 750;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 780;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 810;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 840;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 870;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 900;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 930;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 960;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 990;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 1020;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 1040;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 1080;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 1100;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = 1140;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = 1160;
	p2.y = half_height - 150;
	DrawLine(p1, p2, color1);
	p1.x = half_width - 250;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);

	p1.x = half_width + 950;
	p1.y = half_height - 250;
	p2.x = p1.x + 30;
	p2.y = p1.y + 100;
	DrawLine(p1, p2, color1);
	p1.x = half_width + 1010;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = p1.x + 30;
	p2.y = p1.y + 100;
	DrawLine(p1, p2, color1);
	p1.x = half_width + 1070;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = p1.x + 30;
	p2.y = p1.y + 100;
	DrawLine(p1, p2, color1);
	p1.x = half_width + 1130;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = p1.x + 30;
	p2.y = p1.y + 100;
	DrawLine(p1, p2, color1);
	p1.x = half_width + 1190;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = p1.x + 30;
	p2.y = p1.y + 100;
	DrawLine(p1, p2, color1);
	p1.x = half_width + 1250;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = p1.x + 30;
	p2.y = p1.y + 100;
	DrawLine(p1, p2, color1);
	p1.x = half_width + 1310;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = p1.x + 30;
	p2.y = p1.y + 100;
	DrawLine(p1, p2, color1);
	p1.x = half_width + 1370;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = p1.x + 30;
	p2.y = p1.y + 100;
	DrawLine(p1, p2, color1);
	p1.x = half_width + 1430;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = p1.x + 30;
	p2.y = p1.y + 100;
	DrawLine(p1, p2, color1);
	p1.x = half_width + 1490;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = p1.x + 30;
	p2.y = p1.y + 100;
	DrawLine(p1, p2, color1);
	p1.x = half_width + 1550;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);
	p2.x = p1.x + 30;
	p2.y = p1.y + 100;
	DrawLine(p1, p2, color1);
	p1.x = half_width + 1610;
	p1.y = half_height - 250;
	DrawLine(p1, p2, color1);

	for (int i = 4; i < 250; i += 4)
	{
		p1.x = half_width + 250 - i;
		p1.y = half_height + 250 + i;
		p2.x = half_width + 950 - i;
		p2.y = half_height + 250 + i;
		DrawLine(p1, p2, color);
		p1.x = half_width - 250 + i;
		p1.y = half_height + 250 + i;
		p2.x = half_width + 250 - i;
		p2.y = half_height + 250 + i;
		DrawLine(p1, p2, color);

		p1.x = half_width - 100;
		p1.y = half_height - i;
		p2.x = half_width + 50;
		p2.y = half_height - i;
		DrawLine(p1, p2, color);

	}

	for (int i = 0; i < 100; i++)
	{
		p1.x = 100 + i;
		p1.y = half_height + 150 + i;
		p2.x = 550 - i;
		p2.y = half_height + 150 + i;
		DrawLine(p1, p2, color1);
		p1.x = 100 + i;
		p1.y = half_height + 250 + i;
		p2.x = 550 - i;
		p2.y = half_height + 250 + i;
		DrawLine(p1, p2, color1);
		
	}
	for (double i = 0 ; i < 225 ; i++)
	{
		p1.x = 100 + i;
		p1.y = half_height + 350 + i;
		p2.x = 550 - i;
		p2.y = half_height + 350 + i;
		DrawLine(p1, p2, color1);
	}
	
}