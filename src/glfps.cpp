/*****************************************************************************
 * Wave Simulation in OpenGL
 * (C) 2002 Jakob Thomsen
 * http://home.in.tum.de/~thomsen
 * Modified for GLFW by Sylvain Hellegouarch - sh@programmationworld.com
 * Modified for variable frame rate by Marcus Geelnard
 * 2003-Jan-31: Minor cleanups and speedups / MG
 * 2010-10-24: Formatting and cleanup - Camilla Löwy
 *****************************************************************************/

#if defined(_MSC_VER)
 // Make MS math.h define M_PI
 #define _USE_MATH_DEFINES
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sys/kbd.hpp>
#include <sys/fps.hpp>
#include <sys/vid.hpp>

inline static void error(int error, const char* description);
inline static void size(vid::win::native window, int width, int height);
inline static void key(vid::win::native window, int key, int scancode, int action, int mods);

inline static void init_opengl(void);
inline static void draw_quad(void);

int main(int argc, char* argv[])
{
	glutInit(&argc,argv);

	if(!vid::init(error)) exit(EXIT_FAILURE);
	if(!vid::win.open(800, 600, size, key, "FPS timer"))
	{
        	vid::halt();
        	exit(EXIT_FAILURE);
    	}
    	init_opengl();

	while (vid::win.swap() && sys::timer.step())
	{
		vid::win.clear();

		draw_quad();

		vid::fnt.draw(sys::timer.c_str());
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

static void error(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

inline static void size(vid::win::native window, int width, int height)
{
	if (width > 0 && height > 0)
	{
		glViewport(0, 0, width, height);
		vid::win.ext.set(width,height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.f, vid::win.ext.r, 1.f, 1024.f);
	}
}

inline static void key(vid::win::native window, int key, int scancode, int action, int mods)
{
	if(action != GLFW_PRESS)
		return;

	switch(key)
	{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		default:
			break;
	}
}


inline void init_opengl()
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glPointSize(2.0);
    glClearColor(vid::win.col.back[0],
    		 vid::win.col.back[1],
                 vid::win.col.back[2],
                 vid::win.col.back[3]);
    vid::fnt.col = {0.0f,1.0f,0.0f,1.0f};
}
inline void draw_quad()
{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(0.0, 0.0, -2.0f);
		glRotatef(-70.0f, 1.0, 0.0, 0.0);
		glRotatef(210.0f, 0.0, 0.0, 1.0);

		glBegin(GL_QUADS);
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(-0.5f,-0.5f, 0.0f);
		glColor3f(1.0f,1.0f,0.0f);
		glVertex3f( 0.5f,-0.5f, 0.0f);
		glColor3f(0.0f,1.0f,1.0f);
		glVertex3f( 0.5f, 0.5f, 0.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(-0.5f, 0.5f, 0.0f);
		glEnd();

		glPopMatrix();
}
