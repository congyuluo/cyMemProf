#ifndef INSR_PROF_H
#define INSR_PROF_H

#include <stdlib.h>

void base_check();

// Wrapper functions
void logInstScopeEnter(int id);
void logInstScopeExit(int id);
void logInstReturn();

void instAtExitFunc();

#endif // INSR_PROF_H
