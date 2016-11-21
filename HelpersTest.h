#include "greatest/greatest.h"
#include "Helpers.h"
#include <stdio.h>

TEST trimWhitespace_EmptyString() {
    char test[] = "";
    size_t length = trimWhitespace(test);
    ASSERT_EQ(length, 0);
    ASSERT_STR_EQ(test, "");
    PASS();    
} 

TEST trimWhitespace_AllWhitespace() {
    char test[] = "     \t\t\t\n\n\n\r\n       \n\n\r\t\n     ";
    size_t length = trimWhitespace(test);
    ASSERT_EQ(length, 0);
    ASSERT_STR_EQ(test, "");
    PASS();    
} 

TEST trimWhitespace_OneWord() {
    char test[] = "     \t\t\t\n\n\n\r\nsup    \n\n\r\t\n     ";
    size_t length = trimWhitespace(test);
    ASSERT_EQ(length, strlen("sup"));
    ASSERT_STR_EQ(test, "sup");
    PASS();    
} 

TEST trimWhitespace_MultipleWords() {
    char test[] = "     \t\t\t\n\n\n\r\nsup\thello, world!   \n\n\r\t\n     ";
    size_t length = trimWhitespace(test);
    ASSERT_EQ(length, strlen("sup\thello, world!"));
    ASSERT_STR_EQ(test, "sup\thello, world!");
    PASS();    
} 

TEST trimWhitespace_MultipleLines() {
    char test[] = "     \t\t\t\n\n\n\r\nsup\thello, world!\nmy name is slim shady\n   \n\n\r\t\n     ";
    size_t length = trimWhitespace(test);
    ASSERT_EQ(length, strlen("sup\thello, world!\nmy name is slim shady"));
    ASSERT_STR_EQ(test, "sup\thello, world!\nmy name is slim shady");
    PASS();    
}

TEST parseAssignmentString_EmptyString() {
    char test[] = "";
    char *name = NULL;
    char *value = NULL;
    ASSERT(!parseAssignmentString(test, &name, &value));
    PASS();  
}

TEST parseAssignmentString_NoAssignment() {
    char test[] = "asdfklj99D*&^F(*&^(*&#IUHkjDJF:JLKJDFLKCM<VN<><j¬ÒÓ°‡ﬂ∏ˆ¨ÚÒÔÓˆ˝¨ÁÎÏG";
    char *name = NULL;
    char *value = NULL;
    ASSERT(!parseAssignmentString(test, &name, &value));
    PASS();  
}

TEST parseAssignmentString_EmptyName() {
    char test[] = "=asdfklj99D*&";
    char *name = NULL;
    char *value = NULL;
    ASSERT(!parseAssignmentString(test, &name, &value));
    //ASSERT(!parseAssignmentString("   \t     \t=asdfklj99D*&", &name, &value));
    PASS();  
}

TEST parseAssignmentString_EmptyValue() {
    char test[] = "asdfklj99D=";
    char *name = NULL;
    char *value = NULL;
    ASSERT(!parseAssignmentString(test, &name, &value));
    //ASSERT(!parseAssignmentString("asdfklj99D*&=   \t\r\n     \t\n", &name, &value));
    PASS();  
}

TEST parseAssignmentString_ValidAssignment() {
    char test[] = "HOME=/Users/bandi/Desktop";
    char *name = NULL;
    char *value = NULL;
    ASSERT(parseAssignmentString(test, &name, &value));
    ASSERT_STR_EQ(name, "HOME");
    ASSERT_STR_EQ(value, "/Users/bandi/Desktop");
    PASS();  
}

TEST strarraylen_ZeroElements()
{
    char* test[] = { NULL };
    ASSERT_EQ(0, strarraylen(test));
    PASS();
}

TEST strarraylen_OneElement()
{
    char* test[] = { "sup", NULL };
    ASSERT_EQ(1, strarraylen(test));
    PASS();
}

TEST strarraylen_MultipleElements()
{
    char* test[] = { "sup", "hello", "world", NULL };
    ASSERT_EQ(3, strarraylen(test));
    PASS();
}

TEST strarraylen_SeparatedElements()
{
    char* test[] = { "sup", "hello", NULL, "world", "name", NULL, "is" };
    ASSERT_EQ(2, strarraylen(test));
    PASS();
}

TEST splitBySpace_EmptyString() {
    char test[] = "";
    char **split = splitBySpace(test);
    ASSERT_EQ(0, strarraylen(split));
    free(split);
    PASS();  
}

TEST splitBySpace_OneWord() {
    char test[] = "test";
    char **split = splitBySpace(test);
    ASSERT_EQ(1, strarraylen(split));
    ASSERT_STR_EQ(split[0], "test");
    free(split);
    char test2[] = "\t   test ";
    split = splitBySpace(test2);
    ASSERT_EQ(1, strarraylen(split));
    ASSERT_STR_EQ(split[0], "test");
    free(split);
    PASS();  
}

TEST splitBySpace_OneWordEscapedQuote() {
    char test[] = "\tt\\\"wo";
    char** split = splitBySpace(test);
    ASSERT_EQ(1, strarraylen(split));
    ASSERT_STR_EQ(split[0], "t\"wo");
    free(split);
    PASS();  
}

TEST splitBySpace_OneWordEscapedSpace() {
    char test[] = "\tt\\ wo";
    char** split = splitBySpace(test);
    ASSERT_STR_EQ(split[0], "t wo");
    ASSERT_EQ(1, strarraylen(split));
    free(split);
    PASS();  
}

TEST splitBySpace_OneWordQuoted() {
    char test[] = "\"hello my name is\"";
    char** split = splitBySpace(test);
    ASSERT_EQ(1, strarraylen(split));
    ASSERT_STR_EQ(split[0], "hello my name is");
    free(split);
    PASS();      
}

TEST splitBySpace_TwoWords() {
    char test2[] = "\t   test\t two";
    char** split = splitBySpace(test2);
    ASSERT_EQ(2, strarraylen(split));
    ASSERT_STR_EQ(split[0], "test");
    ASSERT_STR_EQ(split[1], "two");
    free(split);
    PASS();  
}

TEST splitBySpace_InvalidQuoted() {
    char test[] = "\"hello \"my name is\"";
    char** split = splitBySpace(test);
    ASSERT_EQ(0, strarraylen(split));
    free(split);
    PASS();      
}

TEST splitBySpace_InvalidQuoted2() {
    char test[] = "\"hello \\ my name is\"";
    char** split = splitBySpace(test);
    ASSERT_EQ(0, strarraylen(split));
    free(split);
    PASS();      
}

TEST splitBySpace_InvalidQuoted3() {
    char test[] = "\"hello \\ my name is";
    char** split = splitBySpace(test);
    ASSERT_EQ(0, strarraylen(split));
    free(split);
    PASS();      
}

TEST splitBySpace_Long() {
    char test[] = "ls\t \"/Users/bandi/Desktop/Opera\\\"ting Systems\"";
    char** split = splitBySpace(test);
    ASSERT_EQ(2, strarraylen(split));
    ASSERT_STR_EQ(split[0], "ls");
    ASSERT_STR_EQ(split[1], "/Users/bandi/Desktop/Opera\"ting Systems");
    free(split);
    PASS();      
}

TEST splitBySpace_Long2() {
    char test[] = "ls\t /Users/bandi/Desktop/Operating\\ Systems\t  ";
    char** split = splitBySpace(test);
    ASSERT_EQ(2, strarraylen(split));
    ASSERT_STR_EQ(split[0], "ls");
    ASSERT_STR_EQ(split[1], "/Users/bandi/Desktop/Operating Systems");
    free(split);
    PASS();      
}

TEST splitBySpace_OneCharacters() {
    char test[] = "\ta    b c\t\td        e";
    char** split = splitBySpace(test);
    ASSERT_EQ(5, strarraylen(split));
    ASSERT_STR_EQ(split[0], "a");
    ASSERT_STR_EQ(split[1], "b");
    ASSERT_STR_EQ(split[2], "c");
    ASSERT_STR_EQ(split[3], "d");
    ASSERT_STR_EQ(split[4], "e");
    free(split);
    PASS();      
}
    
TEST splitBySpace_ConcatenatedQuotes() {
    char test[] = "ls\t \"/Users/bandi /\"Desktop/Operating\\ Systems\t  ";
    char** split = splitBySpace(test);
    ASSERT_EQ(2, strarraylen(split));
    ASSERT_STR_EQ(split[0], "ls");
    ASSERT_STR_EQ(split[1], "/Users/bandi /Desktop/Operating Systems");
    free(split);
    PASS();  
}
    
TEST splitBySpace_ConcatenatedQuotes2() {
    char test[] = "\"\"a\"sup hello\"\" \"world";
    char** split = splitBySpace(test);
    ASSERT_EQ(1, strarraylen(split));
    ASSERT_STR_EQ(split[0], "asup hello world");
    free(split);
    PASS();  
}
    
TEST splitBySpace_ConcatenatedQuotes3() {
    char test[] = "\"\"\"\"\"\"\"\" hello";
    char** split = splitBySpace(test);
    ASSERT_EQ(2, strarraylen(split));
    ASSERT_STR_EQ(split[0], "");
    ASSERT_STR_EQ(split[1], "hello");
    free(split);
    PASS();  
}

SUITE(HelpersTest)
{
    RUN_TEST(trimWhitespace_EmptyString);
    RUN_TEST(trimWhitespace_AllWhitespace);
    RUN_TEST(trimWhitespace_OneWord);
    RUN_TEST(trimWhitespace_MultipleWords);
    RUN_TEST(trimWhitespace_MultipleLines);

    RUN_TEST(parseAssignmentString_EmptyString);
    RUN_TEST(parseAssignmentString_NoAssignment);
    RUN_TEST(parseAssignmentString_EmptyName);
    RUN_TEST(parseAssignmentString_EmptyValue);
    RUN_TEST(parseAssignmentString_ValidAssignment);

    RUN_TEST(strarraylen_ZeroElements);
    RUN_TEST(strarraylen_OneElement);
    RUN_TEST(strarraylen_MultipleElements);
    RUN_TEST(strarraylen_SeparatedElements);

    RUN_TEST(splitBySpace_EmptyString);
    RUN_TEST(splitBySpace_OneWord);
    RUN_TEST(splitBySpace_OneWordEscapedSpace);
    RUN_TEST(splitBySpace_OneWordEscapedQuote);
    RUN_TEST(splitBySpace_OneWordQuoted);
    RUN_TEST(splitBySpace_TwoWords);
    RUN_TEST(splitBySpace_InvalidQuoted);
    RUN_TEST(splitBySpace_InvalidQuoted2);
    RUN_TEST(splitBySpace_InvalidQuoted3);
    RUN_TEST(splitBySpace_Long);
    RUN_TEST(splitBySpace_Long2);
    RUN_TEST(splitBySpace_OneCharacters);
    RUN_TEST(splitBySpace_ConcatenatedQuotes);
    RUN_TEST(splitBySpace_ConcatenatedQuotes2);
    RUN_TEST(splitBySpace_ConcatenatedQuotes3);
}