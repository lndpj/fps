#pragma once

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <GL/glc.h>

template<size_t N>
using i32 = std::array<GLsizei,N>;
template<size_t N>
using f32 = std::array<GLfloat,N>;

namespace vid
{
	void halt()
	{
		glfwTerminate();
	}
	bool init(GLFWerrorfun error_cb)
	{
		glfwSetErrorCallback(error_cb);
		return glfwInit();
	}
	struct win
	{
		struct ext
		{
			GLsizei w;
			GLsizei h;
			GLfloat r;
			inline void set(GLsizei width, GLsizei height)
			{
				w = width;
				h = height;
				r = (GLfloat)w/(GLfloat)h;
			}
		} ext;
		struct col
		{
			f32<4> back;
			f32<4> front;
		} col;
		using native = GLFWwindow*;
		native handle = NULL;
		bool close()
		{
			glfwDestroyWindow(handle);
			handle = NULL;
			return true;
		}
		bool open(GLsizei w, GLsizei h, GLFWframebuffersizefun size_cb, GLFWkeyfun key_cb, const char* title)
		{
			if(handle) close();
			handle = glfwCreateWindow(w,h,title,NULL,NULL);
			if(handle == NULL) return false;
			glfwSetFramebufferSizeCallback(handle,size_cb);
			glfwSetKeyCallback(handle,key_cb);
			glfwGetFramebufferSize(handle, &ext.w, &ext.h);
			glfwMakeContextCurrent(handle);
			gladLoadGL(glfwGetProcAddress);
			glfwSwapInterval(0);
			size_cb(handle, ext.w, ext.h);
			return true;
		}
		bool swap()
		{
			glfwSwapBuffers(handle);		
			glfwPollEvents();
			return !glfwWindowShouldClose(handle);
		}
		void clear()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		~win()
		{
		}
	} win;
	struct fnt
	{
		i32<2> pos;
		f32<4> col;
		using native = int;
		native font = 1;
		bool init()
		{
			glcContext(glcGenContext());
			glcScale(10, 10);
#ifdef __WIN32__
			glcNewFontFromFamily(1, "Times New Roman");
#else
			glcNewFontFromFamily(1, "Times");
#endif
			glcFont(font);
			return true;
		}
		inline void draw(const char* str)
		{
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D(0,vid::win.ext.w,0,vid::win.ext.h);
			glColor4fv(&col[0]);
			glRasterPos2iv(&pos[0]);
			glcRenderString(str);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
	} fnt;
};
