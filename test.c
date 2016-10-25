#include "greatest/greatest.h"
#include "ArrayTest.h"
#include "HashTableTest.h"
#include "HelpersTest.h"

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(ArrayTest);
    RUN_SUITE(HashTableTest);
    RUN_SUITE(HelpersTest);

    GREATEST_MAIN_END();
}