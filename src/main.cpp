#include <cstdlib>
#include <iostream>
#include <string>

#include <allegro5/allegro.h>

const char * const boolstr(bool);

int main(int argc, char * argv[])
{
    bool ret;

    if (!(ret = al_init())) {
        fprintf(stderr, "Failed to initialize allegro (%s).\n", boolstr(ret));
        return 1;
    }

    return 0;
}

const char * const boolstr(bool value)
{
    return value ? "true" : "false";
}
