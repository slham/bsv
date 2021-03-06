#include "csv.h"
#include "dump.h"

#define NUM_ARGS 1
#define DESCRIPTION "convert csv to bsv\n\n"
#define USAGE "... | bsv\n\n"
#define EXAMPLE ">> echo a,b,c | bsv | bcut 3,2,1 | csv\nc,b,a\n"

#define PARSE_NUMERICS()                                            \
    for (i = 0; i <= csv_max; i++) {                                \
        csv_types[i] = BSV_CHAR;                                    \
        if (csv_sizes[i] > 0 && csv_num_alphas[i] == 0) {           \
            csv_char = csv_columns[i][csv_sizes[i]];                \
            csv_columns[i][csv_sizes[i]] = '\0';                    \
            if (csv_num_dots[i] == 0) {                             \
                csv_types[i] = BSV_INT;                             \
                csv_ints[i] = (bsv_int_t)atol(csv_columns[i]);      \
                csv_sizes[i] = sizeof(bsv_int_t);                   \
                csv_columns[i] = (uint8_t*)(csv_ints + i);          \
            }                                                       \
            else if (csv_num_dots[i] == 1 && csv_sizes[i] > 1) {    \
                csv_types[i] = BSV_FLOAT;                           \
                csv_floats[i] = (bsv_float_t)atof(csv_columns[i]);  \
                csv_sizes[i] = sizeof(bsv_float_t);                 \
                csv_columns[i] = (uint8_t*)(csv_floats + i);        \
            }                                                       \
            csv_columns[i][csv_sizes[i]] = csv_char;                \
        }                                                           \
    }

int main(int argc, const char **argv) {
    HELP();
    SIGPIPE_HANDLER();
    CSV_INIT();
    FILE *files[1] = {stdout};
    int32_t csv_types[MAX_COLUMNS];
    bsv_int_t csv_ints[MAX_COLUMNS];
    bsv_float_t csv_floats[MAX_COLUMNS];
    int32_t i;
    uint8_t csv_char;
    DUMP_INIT(files, 1);
    while (1) {
        CSV_READ_LINE(stdin);
        if (csv_stop)
            break;
        if (csv_max > 0 || csv_sizes[0] > 0) {
            PARSE_NUMERICS();
            if (csv_types[0] != BSV_CHAR)
                DEBUG("warn: first column value is numeric, which will sort incorrectly. first column is the sort key, and is interpreted as bytes\n");
            DUMP(0, csv_max, csv_columns, csv_types, csv_sizes);
        }
    }
    DUMP_FLUSH(0);
}
