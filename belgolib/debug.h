#include "files.h"

#ifndef _debug_h_
#define _debug_h_


//define some debug levels (for -d command line option)

const int d_fatal  = 1;
const int d_vimp   = 2;
const int d_imp    = 4;
const int d_inote  = 8;
const int d_note   = 16;
const int d_list   = 32;
const int d_detail = 64;
const int d_insane = 128;

extern int cdb; // the current debug level defined
extern Dbgfile dbgs; // the current debug file

#endif

