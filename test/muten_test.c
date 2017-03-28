#include <stdio.h>
#include <stdlib.h>

#include "../src/muten.h"
#include "../deps/redismodule.h"
#include "minunit.h"

MU_TEST(insert_ok) {
    int res = Muten_Validate("", 0, "xyz");
    mu_check(res == MUTEN_OK);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(insert_ok);
}

int main(int argc, char **argv) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();

    exit(EXIT_SUCCESS);
}
