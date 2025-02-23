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
    t_parser *parser = create_parser(strdup(""), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, parser->capacity);
    TEST_ASSERT_NULL(parser->tokens[0]);

    close_parser(&parser, true);
}

void    testTokenAppend(void)
{
    t_parser *parser = create_parser(strdup(""), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
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
    t_parser *parser = create_parser(strdup(""), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
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
    t_parser *parser = create_parser(strdup(""), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
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
    TEST_ASSERT_EQUAL_INT(PIPE, parser->tokens[1]->type);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_FALSE(parser->has_cmd);
    close_parser(&parser, true);
}

void    testShiftToken(void)
{
    t_parser *parser = create_parser(strdup(""), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
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
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY * 2, parser->capacity);
    TEST_ASSERT_EQUAL_STRING("d", parser->tokens[2]->str);
    int position = 1;
    shift_token(parser, 2, &position);
    TEST_ASSERT_EQUAL_STRING("d", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_STRING("sdd", parser->tokens[2]->str);
    TEST_ASSERT_EQUAL_INT(2, position);
    close_parser(&parser, true);
}

void    testOutputToken(void)
{
    t_parser *parser = create_parser(strdup(""), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
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

void    testParser_SingleCmd(void)
{
    t_parser *parser = create_parser(strdup("cmd"), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    close_parser(&parser, true);
}

void    testParser_SingleCmdWithArgs(void)
{
    t_parser *parser = create_parser(strdup("cmd arg1 arg2"), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(3, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("arg1", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(ARG, parser->tokens[1]->type);
    TEST_ASSERT_TRUE(parser->tokens[2]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[2]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("arg2", parser->tokens[2]->str);
    TEST_ASSERT_EQUAL_INT(ARG, parser->tokens[2]->type);
    close_parser(&parser, true);
}

void    testParser_SingleCmdWithArgsAndSpace(void)
{
    t_parser *parser = create_parser(strdup("  cmd    arg1   arg2   "), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(3, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("arg1", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(ARG, parser->tokens[1]->type);
    TEST_ASSERT_TRUE(parser->tokens[2]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[2]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("arg2", parser->tokens[2]->str);
    TEST_ASSERT_EQUAL_INT(ARG, parser->tokens[2]->type);
    close_parser(&parser, true);
}

void    testParser_SingleRedIn(void)
{
    t_parser *parser = create_parser(strdup("< infile"), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_FALSE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("<", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(RED, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("infile", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(AFILE, parser->tokens[1]->type);
    close_parser(&parser, true);
}

void    testParser_SingleRedOut(void)
{
    t_parser *parser = create_parser(strdup("> outfile"), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_FALSE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING(">", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(RED, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("outfile", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(AFILE, parser->tokens[1]->type);
    close_parser(&parser, true);
}

void    testParser_DoubleRedIn(void)
{
    t_parser *parser = create_parser(strdup("<< infile"), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_FALSE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("<<", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(RED, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("infile", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(AFILE, parser->tokens[1]->type);
    close_parser(&parser, true);
}

void    testParser_DoubleRedOut(void)
{
    t_parser *parser = create_parser(strdup(">> outfile"), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_FALSE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING(">>", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(RED, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("outfile", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(AFILE, parser->tokens[1]->type);
    close_parser(&parser, true);
}

void    testParser_DoubleRedOutNoSpace(void)
{
    t_parser *parser = create_parser(strdup(">>outfile"), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_FALSE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING(">>", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(RED, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("outfile", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(AFILE, parser->tokens[1]->type);
    close_parser(&parser, true);
}

void    testParser_DoubleRedOutWithSpace(void)
{
    t_parser *parser = create_parser(strdup("    >>outfile    "), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_FALSE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING(">>", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(RED, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("outfile", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(AFILE, parser->tokens[1]->type);
    close_parser(&parser, true);
}

void    testParser_Pipe(void)
{
    t_parser *parser = create_parser(strdup("cmd1|cmd2"), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(3, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd1", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("|", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(PIPE, parser->tokens[1]->type);
    TEST_ASSERT_TRUE(parser->tokens[2]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[2]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd2", parser->tokens[2]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[2]->type);
    close_parser(&parser, true);
}

void    testParser_PipeWithSpace(void)
{
    t_parser *parser = create_parser(strdup("    cmd1      |  cmd2    "), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(3, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd1", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("|", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(PIPE, parser->tokens[1]->type);
    TEST_ASSERT_TRUE(parser->tokens[2]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[2]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd2", parser->tokens[2]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[2]->type);
    close_parser(&parser, true);
}

void    testParser_PipeWithArgs(void)
{
    t_parser *parser = create_parser(strdup("    cmd1     |  cmd2   arg "), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(4, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd1", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("|", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(PIPE, parser->tokens[1]->type);
    TEST_ASSERT_TRUE(parser->tokens[2]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[2]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd2", parser->tokens[2]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[2]->type);
    TEST_ASSERT_TRUE(parser->tokens[3]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[3]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("arg", parser->tokens[3]->str);
    TEST_ASSERT_EQUAL_INT(ARG, parser->tokens[3]->type);
    close_parser(&parser, true);
}

void    testParser_Combine(void)
{
    t_parser *parser = create_parser(strdup("    cmd1   arg arg1 < infile   |  cmd2  > outfile  "), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(9, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd1", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("arg", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(ARG, parser->tokens[1]->type);
    TEST_ASSERT_TRUE(parser->tokens[2]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[2]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("arg1", parser->tokens[2]->str);
    TEST_ASSERT_EQUAL_INT(ARG, parser->tokens[2]->type);
    TEST_ASSERT_TRUE(parser->tokens[3]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[3]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("<", parser->tokens[3]->str);
    TEST_ASSERT_EQUAL_INT(RED, parser->tokens[3]->type);
    TEST_ASSERT_TRUE(parser->tokens[4]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[4]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("infile", parser->tokens[4]->str);
    TEST_ASSERT_EQUAL_INT(AFILE, parser->tokens[4]->type);
    TEST_ASSERT_TRUE(parser->tokens[5]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[5]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("|", parser->tokens[5]->str);
    TEST_ASSERT_EQUAL_INT(PIPE, parser->tokens[5]->type);
    TEST_ASSERT_TRUE(parser->tokens[6]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[6]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd2", parser->tokens[6]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[6]->type);
    TEST_ASSERT_TRUE(parser->tokens[7]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[7]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING(">", parser->tokens[7]->str);
    TEST_ASSERT_EQUAL_INT(RED, parser->tokens[7]->type);
    TEST_ASSERT_TRUE(parser->tokens[8]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[8]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("outfile", parser->tokens[8]->str);
    TEST_ASSERT_EQUAL_INT(AFILE, parser->tokens[8]->type);
    close_parser(&parser, true);
}

void    testParser_CombineNoSpace(void)
{
    t_parser *parser = create_parser(strdup("cmd1 arg arg1<infile|cmd2>outfile"), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(9, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd1", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("arg", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(ARG, parser->tokens[1]->type);
    TEST_ASSERT_TRUE(parser->tokens[2]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[2]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("arg1", parser->tokens[2]->str);
    TEST_ASSERT_EQUAL_INT(ARG, parser->tokens[2]->type);
    TEST_ASSERT_TRUE(parser->tokens[3]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[3]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("<", parser->tokens[3]->str);
    TEST_ASSERT_EQUAL_INT(RED, parser->tokens[3]->type);
    TEST_ASSERT_TRUE(parser->tokens[4]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[4]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("infile", parser->tokens[4]->str);
    TEST_ASSERT_EQUAL_INT(AFILE, parser->tokens[4]->type);
    TEST_ASSERT_TRUE(parser->tokens[5]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[5]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("|", parser->tokens[5]->str);
    TEST_ASSERT_EQUAL_INT(PIPE, parser->tokens[5]->type);
    TEST_ASSERT_TRUE(parser->tokens[6]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[6]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd2", parser->tokens[6]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[6]->type);
    TEST_ASSERT_TRUE(parser->tokens[7]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[7]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING(">", parser->tokens[7]->str);
    TEST_ASSERT_EQUAL_INT(RED, parser->tokens[7]->type);
    TEST_ASSERT_TRUE(parser->tokens[8]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[8]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("outfile", parser->tokens[8]->str);
    TEST_ASSERT_EQUAL_INT(AFILE, parser->tokens[8]->type);
    close_parser(&parser, true);
}

void    testParser_RedWithoutFile(void)
{
    t_parser *parser = create_parser(strdup("<"), NULL);
    TEST_ASSERT_EQUAL_INT(2, parse(parser));
}

void    testParser_RedWithoutFileWithSpace(void)
{
    t_parser *parser = create_parser(strdup("  <   "), NULL);
    TEST_ASSERT_EQUAL_INT(2, parse(parser));
}

void    testParser_RedWithoutFileWithAnotherRed(void)
{
    t_parser *parser = create_parser(strdup("  <   <"), NULL);
    TEST_ASSERT_EQUAL_INT(2, parse(parser));
}

void    testParser_RedWithoutFileWithAnotherRedWithoutSpace(void)
{
    t_parser *parser = create_parser(strdup("<>"), NULL);
    TEST_ASSERT_EQUAL_INT(2, parse(parser));
}

void    testParser_PipeWithoutLeft(void)
{
    t_parser *parser = create_parser(strdup("  |cmd"), NULL);
    TEST_ASSERT_EQUAL_INT(2, parse(parser));
}

void    testParser_PipeDoublePipe(void)
{
    t_parser *parser = create_parser(strdup(" cmd | |"), NULL);
    TEST_ASSERT_EQUAL_INT(2, parse(parser));
}

void    testParser_PipeDoublePipeWithoutSpace(void)
{
    t_parser *parser = create_parser(strdup(" cmd||"), NULL);
    TEST_ASSERT_EQUAL_INT(2, parse(parser));
}

void    testParser_Expander(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("$AA"), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("aa", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_ExpanderWithPrefix(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("AA$AA"), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("AAaa", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_ExpanderNotExist(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("$dd"), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_ExpanderNotExistWithPrefix(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("AA$dd"), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("AA", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_ExpanderAsArg(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("cmd1 $AA "), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd1", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("aa", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(ARG, parser->tokens[1]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_ExpanderAsFile(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("<$AA|cmd2"), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(4, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("<", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(RED, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("aa", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(AFILE, parser->tokens[1]->type);
    TEST_ASSERT_TRUE(parser->tokens[2]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[2]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("|", parser->tokens[2]->str);
    TEST_ASSERT_EQUAL_INT(PIPE, parser->tokens[2]->type);
    TEST_ASSERT_TRUE(parser->tokens[3]->is_end);
    TEST_ASSERT_EQUAL_INT(1, parser->tokens[3]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("cmd2", parser->tokens[3]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[3]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_ExpanderMultiDollar(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("$$"), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("$$", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_ExpanderStatusCode(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    env->prev_status = 99;
    t_parser *parser = create_parser(strdup("$?"), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("99", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_SingleQuote(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("'sdj<>>|$sd \ndjskf'"), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("sdj<>>|$sd \ndjskf", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_SingleQuoteWithPrefixPostFix(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("aa'sdj<>>|$sd \ndjskf'aa"), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("aasdj<>>|$sd \ndjskfaa", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_SingleQuoteUnclosed(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("aa\'sdf"), env);
    TEST_ASSERT_EQUAL_INT(2, parse(parser));
    close_env(&env);
}

void    testParser_DoubleQuote(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("\"sdj<>>|sd \ndjskf\" aaa"), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("sdj<>>|sd \ndjskf", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("aaa", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(ARG, parser->tokens[1]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_DoubleQuoteWithMultiTokens(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("\"a\" b"), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(2, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("a", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    TEST_ASSERT_TRUE(parser->tokens[1]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[1]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("b", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(ARG, parser->tokens[1]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_DoubleQuoteWithPrefixPostFix(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("aa\"sdj<>>|sd \ndjskf\"aa"), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("aasdj<>>|sd \ndjskfaa", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_DoubleQuoteWithExpander(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("aa\"sdj<>>$AA $BB |sd \ndjskf\"aa"), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("aasdj<>>aa bb |sd \ndjskfaa", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_QuotesWithExpander(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("aa\"sdj<>>$AA $BB |sd \ndjskf\"aa\"666\"'yy'zz"), env);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(1, parser->size);
    TEST_ASSERT_TRUE(parser->has_cmd);
    TEST_ASSERT_TRUE(parser->tokens[0]->is_end);
    TEST_ASSERT_EQUAL_INT(0, parser->tokens[0]->pipe_idx);
    TEST_ASSERT_EQUAL_STRING("aasdj<>>aa bb |sd \ndjskfaa666yyzz", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    close_parser(&parser, true);
    close_env(&env);
}

void    testParser_DoubleQuoteUnclosed(void)
{
    char *envp[] = {"AA=aa", "BB=bb", NULL};
	t_env *env = create_env(envp);
    t_parser *parser = create_parser(strdup("aa\"sdf"), env);
    TEST_ASSERT_EQUAL_INT(2, parse(parser));
    close_env(&env);
}

void    testParser_EmptyOne(void)
{
    t_parser *parser = create_parser(strdup(" "), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    TEST_ASSERT_EQUAL_INT(0, parser->size);
    close_parser(&parser, true);
}

void    testParser_Bug1(void)
{
    t_parser *parser = create_parser(strdup("<< eof << eof2 << eof3 | << eof4 cat"), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    char **tokens = output_tokens(parser);
    TEST_ASSERT_EQUAL_STRING("eof4", tokens[8]);
}

void    testParser_Reorder(void)
{
    t_parser *parser = create_parser(strdup("< infile cmd1 | cmd2 > outfile arg < infile2"), NULL);
    TEST_ASSERT_EQUAL_INT(0, parse(parser));
    re_order_tokens(parser);
    TEST_ASSERT_EQUAL_INT(10, parser->size);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[0]->type);
    TEST_ASSERT_EQUAL_STRING("cmd1", parser->tokens[0]->str);
    TEST_ASSERT_EQUAL_INT(RED, parser->tokens[1]->type);
    TEST_ASSERT_EQUAL_STRING("<", parser->tokens[1]->str);
    TEST_ASSERT_EQUAL_INT(AFILE, parser->tokens[2]->type);
    TEST_ASSERT_EQUAL_STRING("infile", parser->tokens[2]->str);
    TEST_ASSERT_EQUAL_INT(1, parser->pipe_count);
    TEST_ASSERT_EQUAL_INT(PIPE, parser->tokens[3]->type);
    TEST_ASSERT_EQUAL_INT(CMD, parser->tokens[4]->type);
    TEST_ASSERT_EQUAL_STRING("cmd2", parser->tokens[4]->str);
    TEST_ASSERT_EQUAL_INT(ARG, parser->tokens[5]->type);
    TEST_ASSERT_EQUAL_STRING("arg", parser->tokens[5]->str);
    TEST_ASSERT_EQUAL_INT(RED, parser->tokens[6]->type);
    TEST_ASSERT_EQUAL_STRING(">", parser->tokens[6]->str);
    TEST_ASSERT_EQUAL_INT(AFILE, parser->tokens[7]->type);
    TEST_ASSERT_EQUAL_STRING("outfile", parser->tokens[7]->str);
    TEST_ASSERT_EQUAL_INT(RED, parser->tokens[8]->type);
    TEST_ASSERT_EQUAL_STRING("<", parser->tokens[8]->str);
    TEST_ASSERT_EQUAL_INT(AFILE, parser->tokens[9]->type);
    TEST_ASSERT_EQUAL_STRING("infile2", parser->tokens[9]->str);
    close_parser(&parser, true);
}

// Main function to run the tests
int	main(void)
{
	UNITY_BEGIN();
    RUN_TEST(testParserCreate);
    RUN_TEST(testTokenAppend);
    RUN_TEST(testTokenAppendStr);
    RUN_TEST(testSetToken);
    RUN_TEST(testShiftToken);
    RUN_TEST(testOutputToken);
    RUN_TEST(testParser_SingleCmd);
    RUN_TEST(testParser_SingleCmdWithArgs);
    RUN_TEST(testParser_SingleCmdWithArgsAndSpace);
    RUN_TEST(testParser_SingleRedIn);
    RUN_TEST(testParser_SingleRedOut);
    RUN_TEST(testParser_DoubleRedIn);
    RUN_TEST(testParser_DoubleRedOut);
    RUN_TEST(testParser_DoubleRedOutNoSpace);
    RUN_TEST(testParser_DoubleRedOutWithSpace);
    RUN_TEST(testParser_Pipe);
    RUN_TEST(testParser_PipeWithSpace);
    RUN_TEST(testParser_PipeWithArgs);
    RUN_TEST(testParser_Combine);
    RUN_TEST(testParser_CombineNoSpace);
    RUN_TEST(testParser_RedWithoutFile);
    RUN_TEST(testParser_RedWithoutFileWithSpace);
    RUN_TEST(testParser_RedWithoutFileWithAnotherRed);
    RUN_TEST(testParser_RedWithoutFileWithAnotherRedWithoutSpace);
    RUN_TEST(testParser_PipeWithoutLeft);
    RUN_TEST(testParser_PipeDoublePipe);
    RUN_TEST(testParser_PipeDoublePipeWithoutSpace);
    RUN_TEST(testParser_Expander);
    RUN_TEST(testParser_ExpanderWithPrefix);
    RUN_TEST(testParser_ExpanderNotExist);
    RUN_TEST(testParser_ExpanderNotExistWithPrefix);
    RUN_TEST(testParser_ExpanderAsArg);
    RUN_TEST(testParser_ExpanderAsFile);
    RUN_TEST(testParser_ExpanderMultiDollar);
    RUN_TEST(testParser_ExpanderStatusCode);
    RUN_TEST(testParser_SingleQuote);
    RUN_TEST(testParser_SingleQuoteWithPrefixPostFix);
    RUN_TEST(testParser_SingleQuoteUnclosed);
    RUN_TEST(testParser_DoubleQuote);
    RUN_TEST(testParser_DoubleQuoteWithPrefixPostFix);
    RUN_TEST(testParser_DoubleQuoteUnclosed);
    RUN_TEST(testParser_DoubleQuoteWithExpander);
    RUN_TEST(testParser_DoubleQuoteWithMultiTokens);
    RUN_TEST(testParser_QuotesWithExpander);
    RUN_TEST(testParser_EmptyOne);
    RUN_TEST(testParser_Reorder);
    RUN_TEST(testParser_Bug1);
	return (UNITY_END());
}