#include <sys/fps.hpp>
#include <sys/kbd.hpp>

int main(int argc, char** argv)
{
	if(!sys::term::init()) exit(EXIT_FAILURE);

	while(sys::term::getc() != 27 && sys::timer.step())
		printf("\r%s",sys::timer.c_str());

	exit(EXIT_SUCCESS);
}
