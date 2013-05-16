#include "profiler.h"
#include <ctime>
#include <vector>
#include <utility>
#include <iostream>

typedef std::vector<std::pair<const char *, float> > TimingVec;

static TimingVec& get_tvec()
{
    static TimingVec tvec;
    return tvec;
}

static void dump_to_stdout()
{
    TimingVec tvec = get_tvec();
    for (TimingVec::const_iterator it=tvec.begin(); it != tvec.end(); ++it) {
        std::cout << it->first << "|" << it->second << std::endl;
    }
}

void init_profiler()
{
    TimingVec tvec = get_tvec();
    tvec.clear();
    start("__START__");
}

void stop_profiler()
{
    stop("__STOP__");
    dump_to_stdout();
}

void start(const char *name)
{
    get_tvec().push_back(std::pair<const char *, float>(name, (float)clock()/CLOCKS_PER_SEC));
}

void stop(const char *name)
{
    get_tvec().push_back(std::pair<const char *, float>(name, (float)clock()/CLOCKS_PER_SEC));
}
