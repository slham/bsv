#include "util.h"
#include "read_simple.h"
#include "write_simple.h"
#include "xxh3.h"

#define NUM_ARGS 0
#define DESCRIPTION "xxh3_64 hash stdin, defaults to hex, can be --int, or --stream to hex and pass stdin through\n\n"
#define USAGE "... | xxh3 [--stream|--int]\n\n"
#define EXAMPLE ">> echo abc | xxh3\nB5CA312E51D77D64\n"

int main(int argc, const char **argv) {
    HELP();
    SIGPIPE_HANDLER();
    FILE *read_files[1] = {stdin};
    READ_INIT(read_files, 1);
    FILE *write_files[1] = {stdout};
    WRITE_INIT(write_files, 1);
    XXH3_state_t state;
    ASSERT(XXH3_64bits_reset(&state) != XXH_ERROR, "xxh3 reset failed\n");
    int stream_mode = argc > 1 && strcmp(argv[1], "--stream") == 0;
    if (stream_mode) {
        while (1) {
            READ(BUFFER_SIZE, 0);
            ASSERT(XXH3_64bits_update(&state, read_buffer, read_bytes) != XXH_ERROR, "xxh3 update failed\n");
            WRITE(read_buffer, read_bytes, 0);
            if (BUFFER_SIZE != read_bytes)
                break;
        }
    } else {
        while (1) {
            READ(BUFFER_SIZE, 0);
            ASSERT(XXH3_64bits_update(&state, read_buffer, read_bytes) != XXH_ERROR, "xxh3 update failed\n");
            if (BUFFER_SIZE != read_bytes)
                break;
        }
    }
    uint64_t hash = XXH3_64bits_digest(&state);
    FILE *out = (stream_mode) ? stderr : stdout;
    if (argc > 1 && strcmp(argv[1], "--int") == 0)
        fprintf(out, "%lu\n", hash);
    else
        fprintf(out, "%08x%08x\n", (int32_t)(hash>>32), (int32_t)hash);
    if (argc > 1 && strcmp(argv[1], "--stream") == 0)
        WRITE_FLUSH(0);
}
