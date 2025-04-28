#ifndef CIRCLE_H
#define CIRCLE_H

typedef struct {
    unsigned char r, g, b;
} Color;

/**
 * Data of a circle object in Descartes coordinate system
 */
typedef struct Circle
{
	double x;
	double y;
	double radius;
	Color color;
} Circle;



/**
 * Set the data of the circle
 */
void set_circle_data(Circle* circle, double x, double y, double radius);
// set circle color
void set_circle_color(Circle* circle, unsigned char r, unsigned char g, unsigned char b);

/**
 * Calculate the area of the circle.
 */
double calc_circle_area(const Circle* circle);

#endif // CIRCLE_H
