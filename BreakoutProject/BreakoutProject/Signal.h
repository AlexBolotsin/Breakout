#pragma once

class Signal
{
public:

	enum class Code
	{
		NONE,
		EXIT,
		NEXT_SCREEN
	};

	void Send(Code code, int arg);
};

