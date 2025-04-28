#include "circle.h"
#include <stdlib.h>
#include <math.h>

void set_circle_data(Circle* circle, double x, double y, double radius)
{
	circle->x = x;
	circle->y = y;
	if (radius > 0.0) {
		circle->radius = radius;
	} else {
		circle->radius = NAN;
	}
}

void set_circle_color(Circle* circle, unsigned char r, unsigned char g, unsigned char b)
{
    circle->color.r = r;
    circle->color.g = g;
    circle->color.b = b;
}

double calc_circle_area(const Circle* circle)
{
	double area = circle->radius * circle->radius * M_PI;
	return area;
}
