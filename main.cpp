#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include "SDL.h"
#include "AppController.h"

int
main(int argc, char* argv[])
{
	/*AllocConsole();

    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;*/

    AppController app;
	return app.start();
}
