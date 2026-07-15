# fps

C++ FPS timer

## Build

```sh
cmake . --install-prefix=/usr -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=ReleaseStrip
make install
```

## Usage

```cpp
#include <sys/fps.hpp>
#include <sys/kbd.hpp>

int main(int argc, char** argv)
{
	if(!sys::term::init()) exit(EXIT_FAILURE);

	while(sys::term::getc() != 27 && sys::timer.step())
		printf("\r%s",sys::timer.c_str());

	exit(EXIT_SUCCESS);
}
```
```cpp
void sys::timer.draw(const GLfloat col[4], const GLsizei pos[2])
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,sys::vid::w,0,sys::vid::h);
    glColor4fv(col);
    glRasterPos2iv(pos);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10,(const unsigned char*)sys::timer.c_str());
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
```
```sh
./ttyfps 
Tue Jul 14 23:40:13 2026   964440/1146143
```
