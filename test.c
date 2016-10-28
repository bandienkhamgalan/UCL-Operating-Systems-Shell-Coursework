#include "greatest/greatest.h"
#include "HashTableTest.h"
#include "HelpersTest.h"
#include "ShellTest.h"

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();
    
    RUN_SUITE(HashTableTest);
    RUN_SUITE(HelpersTest);
    RUN_SUITE(ShellTest);

    GREATEST_MAIN_END();
}