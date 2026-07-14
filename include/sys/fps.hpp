#pragma once

#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <chrono>
#include <cstring>

namespace sys
{
	struct timer	
	{
		struct date
		{
			std::time_t now = 0;
			char str[32];
		} date;
		struct fps
		{
			std::size_t cnt = 0;
			char str[32];
		} fps;
		char str[64];
		using clock = std::chrono::high_resolution_clock;
		std::chrono::time_point<clock> pre = clock::now();
		std::chrono::time_point<clock> now = clock::now();
		std::size_t acc = 0;
		std::size_t dif = std::chrono::duration_cast<std::chrono::nanoseconds>(now - pre).count();

		bool step(const std::size_t len = 1000000000)
		{
			static bool first = true;
			pre  = now;
			now  = clock::now();
			dif  = std::chrono::duration_cast<std::chrono::nanoseconds>(now - pre).count();
			acc += dif;

			if(first)
			{
				snprintf(fps.str,31, "%zu", fps.cnt);
				date.now = std::chrono::system_clock::to_time_t(now);
				ctime_r(&date.now, date.str); date.str[strlen(date.str) - 1] = '\0';
			}
			if(acc >= len)
			{
				snprintf(fps.str,31, "%zu", fps.cnt);
				date.now = std::chrono::system_clock::to_time_t(now);
				ctime_r(&date.now, date.str); date.str[strlen(date.str) - 1] = '\0';
				fps.cnt = 0;
				acc -= len;
				if(first) first = false;
			}
			else 
				fps.cnt++;
			return true;
		}
		const char* c_str()
		{
			snprintf(str, 64, "%s %8zu/%s", date.str, fps.cnt, fps.str);
			return str;
		}
	} timer;
};
