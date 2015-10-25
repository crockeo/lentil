#include "sleep.h"

//////////////
// Includes //
#ifdef _WIN32
    #error "No windows support yet."
#elif __APPLE__
    #include <unistd.h>
#elif __linux__
    #include <unistd.h>
#elif __unix__
    #include <unistd.h>
#else
    #error "Unsupported platform."
#endif

//////////
// Code //

// Sleeping on the current thread for some fraction of a second as represented
// by a float.
void Lentil_Core_sleep(double time) {
    #ifdef _WIN32

    #elif __APPLE__
        usleep(time * 1000000);
    #elif __linux__
        usleep(time * 1000000);
    #elif __unix__
        usleep(time * 1000000);
    #endif
}
