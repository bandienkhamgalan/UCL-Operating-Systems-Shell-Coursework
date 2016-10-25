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

TEST ParseAssignmentString_EmptyString() {
    char test[] = "";
    char *name;
    char *value;
    bool result = ParseAssignmentString(test, &name, &value);
    ASSERT_EQ(result, false);
    PASS();    
} 

TEST ParseAssignmentString_InvalidNameLength() {
    char test[] = "=fjkjeijakdfa";
    char *name;
    char *value;
    bool result = ParseAssignmentString(test, &name, &value);
    ASSERT_EQ(result, false);
    PASS();    
} 

TEST ParseAssignmentString_InvalidValueLength() {
    char test[] = "fjkjeijakdfa=";
    char *name;
    char *value;
    bool result = ParseAssignmentString(test, &name, &value);
    ASSERT_EQ(result, false);
    PASS();    
} 

TEST ParseAssignmentString_RegularString() {
    char test[] = "HOME=/Users/bandi";
    char *name;
    char *value;
    bool result = ParseAssignmentString(test, &name, &value);
    ASSERT_EQ(result, true);
    ASSERT_STR_EQ(name, "HOME");
    ASSERT_STR_EQ(value, "/Users/bandi");
    PASS();    
} 

TEST ParseAssignmentString_EscapedEqualSign() {
    char test[] = "HO/=ME=/Users/bandi";
    char *name;
    char *value;
    bool result = ParseAssignmentString(test, &name, &value);
    ASSERT_EQ(result, true);
    ASSERT_STR_EQ(name, "HO/=ME");
    ASSERT_STR_EQ(value, "/Users/bandi");
    PASS();    
} 

SUITE(HelpersTest)
{
    RUN_TEST(trimWhitespace_EmptyString);
    RUN_TEST(trimWhitespace_AllWhitespace);
    RUN_TEST(trimWhitespace_OneWord);
    RUN_TEST(trimWhitespace_MultipleWords);
    RUN_TEST(trimWhitespace_MultipleLines);

    RUN_TEST(ParseAssignmentString_EmptyString);
    RUN_TEST(ParseAssignmentString_InvalidNameLength);
    RUN_TEST(ParseAssignmentString_InvalidValueLength);
    RUN_TEST(ParseAssignmentString_RegularString);
    RUN_TEST(ParseAssignmentString_EscapedEqualSign);
}