#include <Dxlib.h>
#include "Myself.h"



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Myself::Destroy();											// ブログラムの終了
	Myself::GetInstance().Run();								

	return 0;
}


