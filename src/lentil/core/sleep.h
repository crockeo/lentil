#ifndef _LENTIL_CORE_SLEEP_H_
#define _LENTIL_CORE_SLEEP_H_

//////////
// Code //

#ifdef __cplusplus
extern "C" {
#endif

// Sleeping on the current thread for some fraction of a second as represented
// by a float.
void Lentil_Core_sleep(double);

#ifdef __cplusplus
}
#endif

#endif
