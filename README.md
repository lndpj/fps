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
```sh
./ttyfps 
Tue Jul 14 23:40:13 2026   964440/1146143
```
