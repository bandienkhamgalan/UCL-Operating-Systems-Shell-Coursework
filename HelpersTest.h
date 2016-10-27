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

TEST containsSpaces_EmptyString() {
    ASSERT(!containsSpaces(""));
    PASS();  
}

TEST containsSpaces_AllWhitespace() {
    ASSERT(containsSpaces("     \t\t\t\n\n\n\r\n       \n\n\r\t\n     "));
    PASS();  
}

TEST containsSpaces_NoWhitespace() {
    ASSERT(!containsSpaces("asdfklj99D*&^F(*&^(*&#IUHkjDJF:JLKJDFLKCM<VN<><j¬ÒÓ°‡ﬂ∏ˆ¨ÚÒÔÓˆ˝¨ÁÎÏG"));
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

SUITE(HelpersTest)
{
    RUN_TEST(trimWhitespace_EmptyString);
    RUN_TEST(trimWhitespace_AllWhitespace);
    RUN_TEST(trimWhitespace_OneWord);
    RUN_TEST(trimWhitespace_MultipleWords);
    RUN_TEST(trimWhitespace_MultipleLines);

    RUN_TEST(containsSpaces_EmptyString);
    RUN_TEST(containsSpaces_AllWhitespace);
    RUN_TEST(containsSpaces_NoWhitespace);

    RUN_TEST(parseAssignmentString_EmptyString);
    RUN_TEST(parseAssignmentString_NoAssignment);
    RUN_TEST(parseAssignmentString_EmptyName);
    RUN_TEST(parseAssignmentString_EmptyValue);
    RUN_TEST(parseAssignmentString_ValidAssignment);
}