#include "Test.h"

#include <stdio.h>
#include <stdlib.h>

int main()
{
    Test test;

    if (!test.IsOk()) {
        return EXIT_FAILURE;
    }

    bool passed = test.RunThreads();

    printf("=========== %s\n", passed ? "PASSED" : "FAILED");

    return EXIT_SUCCESS;
}
