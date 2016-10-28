#include "greatest/greatest.h"
#include "Shell.h"
#include "Helpers.h"

TEST Shell_Make_EmptySearchPaths() {
    Shell* shell = Shell_Make();
    ASSERT_EQ(0, strarraylen(shell->searchPaths));
    Shell_Free(shell);
    PASS();
} 

TEST Shell_LoadSearchPathsFromString_ThreePaths() {
    Shell* shell = Shell_Make();
    ASSERT_EQ(0, strarraylen(shell->searchPaths));
    char paths[] = "/bin:/usr/bin:/usr/local/bin";
    Shell_LoadSearchPathsFromString(shell, paths);
    ASSERT_EQ(3, strarraylen(shell->searchPaths));
    ASSERT_STR_EQ("/bin", shell->searchPaths[0]);
    ASSERT_STR_EQ("/usr/bin", shell->searchPaths[1]);
    ASSERT_STR_EQ("/usr/local/bin", shell->searchPaths[2]);
    ASSERT_EQ(14, shell->searchPathMaxLength);
    Shell_Free(shell);
    PASS();
}

TEST Shell_LoadSearchPathsFromString_ClearsPreviousPaths() {
    Shell* shell = Shell_Make();
    char paths[] = "/usr/local/bin:/usr/bin";
    Shell_LoadSearchPathsFromString(shell, paths);
    char paths2[] = "/bin";
    Shell_LoadSearchPathsFromString(shell, paths2);
    ASSERT_EQ(1, strarraylen(shell->searchPaths));
    ASSERT_STR_EQ("/bin", shell->searchPaths[0]);
    ASSERT_EQ(4, shell->searchPathMaxLength);
    Shell_Free(shell);
    PASS();
}

SUITE(ShellTest)
{
    RUN_TEST(Shell_Make_EmptySearchPaths);
    RUN_TEST(Shell_LoadSearchPathsFromString_ThreePaths);
    RUN_TEST(Shell_LoadSearchPathsFromString_ClearsPreviousPaths);
}