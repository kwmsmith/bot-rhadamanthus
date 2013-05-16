#ifndef _PROFILER_H_
#define _PROFILER_H_

void init_profiler();
void stop_profiler();

void start(const char *name);

void stop(const char *name);

#endif
