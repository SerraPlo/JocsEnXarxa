#pragma once
#include <conio.h>

namespace Utils
{
	extern inline int Min(int x, int y) {
		return y ^ ((x ^ y) & -(x < y));
	}

	extern inline int Max(int x, int y) {
		return x ^ ((x ^ y) & -(x < y));
	}

	extern void ReadChar(char* data) {
		int ch, pos = 0;
		do {
			ch = _getch();
			if (ch == 8) *(data + (pos--)) = '\0', std::cout << '\b' << ' ' << '\b';
			else *(data + (pos++)) = ch, std::cout << char(ch);
		} while (ch != 13);
		*(data + pos) = '\0';
	}
};

