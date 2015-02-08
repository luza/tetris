#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "SDL.h"
#include "AppController.h"

int
main(int argc, char* argv[])
{
	/* initialize random seed */
	srand((unsigned int)time(NULL));

    AppController app;
	return app.start();
}
