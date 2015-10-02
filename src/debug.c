#include "debug.h"

//////////
// Code //

// The internal value for debug mode.
static int Lentil_Core_debug = 0;

// Sets & gets the debugging level.
//
// If level < 0, it solely returns the current debugging level. 
//
// If level >= 0, it attempts to set the debug level to the level prescribed,
// and returns the new debugging level. Debugging levels are in ascending order,
// with higher levels representing more costly (but more informative) debugging
// information.
//
// Examples:
//   0 = no debugging information (fastest)
//
//   1 = computationally cheap debugging information (fast - like printing out
//       compile errors on shaders))
//
//   2 = inexpensive, but not cheap debugging information (average)
//
//   3 = costly debugging information (slow - information about specific
//       renders, render call amounts, etc. etc.)
int Lentil_Core_debugLevel(int level) {
    if (level >= 0)
        Lentil_Core_debug = level;

    return Lentil_Core_debug;
}
