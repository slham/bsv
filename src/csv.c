#include "util.h"

int main(int argc, const char **argv) {
    LOAD_INIT_VARS();
    WRITE_INIT_VARS();
    int i;
    int ran = 0;
    while (1) {
        LOAD(stdin);
        if (load_stop)
            break;
        for (i = 0; i <= load_max; i++) {
            WRITE(load_columns[i], load_sizes[i], stdout);
            if (i != load_max)
                WRITE(",", 1, stdout);
        }
        WRITE("\n", 1, stdout);
        ran = 1;
    }
    if (ran == 0)
        WRITE("\n", 1, stdout);
    WRITE_FLUSH(stdout);
}