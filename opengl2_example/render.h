#pragma once

#ifndef __RENDER_H__
#define __RENDER_H__

#include <GLFW\glfw3.h>

	void draw_box(float box_l, float box_r, float box_d,
		float box_u, float box_f, float box_b,
		float surf_r, float surf_g, float surf_b, float surf_a,
		GLuint *NoForAll, int *t_idx);

void draw_sphere(GLfloat xx, GLfloat yy, GLfloat zz, GLfloat aa, GLfloat bb, GLfloat cc, GLfloat M, GLfloat N);

void grab(int Width, int Height);

void Draw_Box(float box_l, float box_r, float box_d,
	float box_u, float box_f, float box_b,
	float surf_r, float surf_g, float surf_b, float surf_a,
	float line_r, float line_g, float line_b, float line_a,
	GLuint* NoForAll, int *t_idx);

#endif // !__RENDER_H__
