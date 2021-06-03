#include "ffplay.h"
extern "C" {
    extern int ffplay(const char *argv);
}
Ffplay::Ffplay(const char *s)
{
}

Ffplay::~Ffplay()
{

}

int Ffplay::run(const char *file_name)
{
    if (!file_name)
        return -1;
    ::ffplay(file_name);
    return 0;
}
