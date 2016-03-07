#pragma once

class Utils
{
public:
	static int Min(int x, int y) {
		return y ^ ((x ^ y) & -(x < y));
	}

	static int Max(int x, int y) {
		return x ^ ((x ^ y) & -(x < y));
	}
};

