#include <stdio.h>
#include <stdlib.h>

#include "../src/muten.h"
#include "../deps/logging/logging.h"
#include "minunit.h"

MU_TEST(insert_ok) {
    int res = Muten_Command(MUTEN_INSERT_SUFFIX, MUTEN_DELETE_SUFFIX);
    mu_check(res == MUTEN_OK);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(insert_ok);
}

int main(int argc, char **argv) {
    LOGGING_INIT(L_DEBUG);
    LG_DEBUG("Testing muten...");

    MU_RUN_SUITE(test_suite);
    MU_REPORT();

    exit(EXIT_SUCCESS);
}
