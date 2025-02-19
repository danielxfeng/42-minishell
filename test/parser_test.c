#include "../src/include/parser.h"
#include "unity.h"
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// valgrind --leak-check=full --show-leak-kinds=all ./build/parser_test

// Setup function called before each test
void	setUp(void)
{
	// Initialize or allocate resources if needed (optional)
}

// Teardown function called after each test
void	tearDown(void)
{
	// Clean up resources if needed (optional)
}

void    testParserCreate(void)
{
    t_parser *parser = create_parser(strdup(""));
    TEST_ASSERT_NOT_NULL(parser);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, parser->capacity);
    TEST_ASSERT_NULL(parser->tokens[0]);

    close_parser(&parser, true);
}

void    testTokenAppend(void)
{
    t_parser *parser = create_parser(strdup(""));
    TEST_ASSERT_NOT_NULL(parser);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, parser->capacity);
    TEST_ASSERT_NULL(parser->tokens[0]);
    char *str = "assddd";
    append_token(parser);
    append_str_to_last_token(parser, ms_substr(str, 1, 2));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_EQUAL_STRING("ss", parser->tokens[0]->str);
    append_token(parser);
    append_str_to_last_token(parser, ms_substr(str, 2, 3));
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, parser->capacity);
    TEST_ASSERT_EQUAL_STRING("sdd", parser->tokens[1]->str);
    append_token(parser);
    append_str_to_last_token(parser, ms_substr(str, 3, 1));
    TEST_ASSERT_EQUAL_INT(3, parser->size);
    TEST_ASSERT_EQUAL_STRING("d", parser->tokens[2]->str);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY * 2, parser->capacity);
    close_parser(&parser, true);
}

void    testTokenAppendStr(void)
{
    t_parser *parser = create_parser(strdup(""));
    TEST_ASSERT_NOT_NULL(parser);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, parser->capacity);
    TEST_ASSERT_NULL(parser->tokens[0]);
    char *str = "assddd";
    append_token(parser);
    append_str_to_last_token(parser, ms_substr(str, 1, 2));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_EQUAL_STRING("ss", parser->tokens[0]->str);
    append_token(parser);
    append_str_to_last_token(parser, ms_substr(str, 2, 3));
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, parser->capacity);
    TEST_ASSERT_EQUAL_STRING("sdd", parser->tokens[1]->str);
    append_str_to_last_token(parser, strdup("ppp"));
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, parser->capacity);
    TEST_ASSERT_EQUAL_STRING("sddppp", parser->tokens[1]->str);
    close_parser(&parser, true);
}

void    testSetToken(void)
{
    t_parser *parser = create_parser(strdup(""));
    TEST_ASSERT_NOT_NULL(parser);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, parser->capacity);
    TEST_ASSERT_NULL(parser->tokens[0]);
    char *str = "assddd";
    append_token(parser);
    append_str_to_last_token(parser, ms_substr(str, 1, 2));
    set_token(parser, 0, CMD);
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_EQUAL_STRING("ss", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_TRUE(parser->has_cmd);
    append_token(parser);
    set_token(parser, 1, PIPE);
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_EQUAL_INT(PIPE, parser->tokens[0]->type);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_FALSE(parser->has_cmd);
    close_parser(&parser, true);
}

void    testSwitchToken(void)
{
    t_parser *parser = create_parser(strdup(""));
    TEST_ASSERT_NOT_NULL(parser);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, parser->capacity);
    TEST_ASSERT_NULL(parser->tokens[0]);
    char *str = "assddd";
    append_token(parser);
    append_str_to_last_token(parser, ms_substr(str, 1, 2));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_EQUAL_STRING("ss", parser->tokens[0]->str);
    append_token(parser);
    append_str_to_last_token(parser, ms_substr(str, 2, 3));
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, parser->capacity);
    TEST_ASSERT_EQUAL_STRING("sdd", parser->tokens[1]->str);
    switch_token(parser, 0, 1);
    TEST_ASSERT_EQUAL_STRING("ss", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_STRING("sdd", parser->tokens[0]->str);
    close_parser(&parser, true);
}

void    testOutputToken(void)
{
    t_parser *parser = create_parser(strdup(""));
    TEST_ASSERT_NOT_NULL(parser);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, parser->capacity);
    TEST_ASSERT_NULL(parser->tokens[0]);
    char *str = "assddd";
    append_token(parser);
    append_str_to_last_token(parser, ms_substr(str, 1, 2));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_EQUAL_STRING("ss", parser->tokens[0]->str);
    append_token(parser);
    append_str_to_last_token(parser, ms_substr(str, 2, 3));
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, parser->capacity);
    TEST_ASSERT_EQUAL_STRING("sdd", parser->tokens[1]->str);
    char **tokens = output_tokens(parser);
    TEST_ASSERT_EQUAL_STRING("ss", tokens[0]);
    TEST_ASSERT_EQUAL_STRING("sdd", tokens[1]);
    free(tokens[0]);
    tokens[0] = NULL;
    free(tokens[1]);
    tokens[1] = NULL;
    free(tokens);
}


// Main function to run the tests
int	main(void)
{
	UNITY_BEGIN();
    RUN_TEST(testParserCreate);
    RUN_TEST(testTokenAppend);
    RUN_TEST(testTokenAppendStr);
    RUN_TEST(testSetToken);
    RUN_TEST(testSwitchToken);
    RUN_TEST(testOutputToken);
	return (UNITY_END());
}