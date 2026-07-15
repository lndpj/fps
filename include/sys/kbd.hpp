#pragma once

#include <fcntl.h>
#include <sys/param.h>
#include <unistd.h>
#include <termios.h>

namespace sys::term
{
	void halt()
	{
		struct termios info;
		int flags = fcntl(STDIN_FILENO,F_GETFL,0);
		fcntl(STDIN_FILENO, F_SETFL,flags & ~O_NONBLOCK);
		tcgetattr(STDIN_FILENO, &info);
		info.c_lflag |= ICANON;
		info.c_lflag |= ECHO;
		tcsetattr(STDIN_FILENO, TCSANOW, &info);
		puts("");
	}
	bool init()
	{
		struct termios info;
		tcgetattr(STDIN_FILENO, &info);
		info.c_lflag &= ~ICANON;
		info.c_lflag &= ~ECHO;
		info.c_cc[VMIN] = 1;
		info.c_cc[VTIME] = 0;
		tcsetattr(STDIN_FILENO, TCSANOW, &info);
		int flags = fcntl(STDIN_FILENO,F_GETFL,0);
		if (flags == -1)
		{
			perror("fcntl: F_GETFL failed");
			halt();
			return false;
		}
		if(fcntl(STDIN_FILENO, F_SETFL,flags | O_NONBLOCK) == -1)
		{
			perror("fcntl: F_SETFL failed");
			halt();
			return false;
		}
		atexit(sys::term::halt);

		return true;
	}
	int getc()
	{
		int c = '\0';
		if(read(STDIN_FILENO, &c, 1) == 1)
			return c;

		if(errno == EAGAIN || errno == EWOULDBLOCK)
			return c;

		perror("fgetc: failed\n");
		exit(EXIT_FAILURE);
	}
};
