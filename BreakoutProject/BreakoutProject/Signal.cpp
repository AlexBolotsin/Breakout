#include "Signal.h"

extern bool exitVal;
extern int sceneIndex;

void Signal::Send(Code code, int arg)
{
	switch (code)
	{
	case Code::EXIT:
		exitVal = true;
		break;
	case Code::NEXT_SCREEN:
		sceneIndex = arg;
		break;
	default:
		break;
	}
}
