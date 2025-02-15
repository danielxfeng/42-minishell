#include "../src/include/mini_env.h"
#include "unity.h"
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// valgrind --leak-check=full --show-leak-kinds=all ./build/env_test

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

void    testCreateEnv_Normal(void)
{
    char *envp[] = {"HOME=ACT", NULL};
	t_env *env = create_env(envp);

	TEST_ASSERT_EQUAL_INT(1, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_EQUAL_STRING("HOME", env->items[0].key);
	TEST_ASSERT_EQUAL_STRING("ACT", env->items[0].value);
	close_env(&env);
}

void    testCreateEnv_Empty(void)
{
    char **envp = NULL;
	t_env *env = create_env(envp);

	TEST_ASSERT_EQUAL_INT(0, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	close_env(&env);
}

void    testSetEnv_SetEnvNormal(void)
{
    char **envp = NULL;
	t_env *env = create_env(envp);

	TEST_ASSERT_EQUAL_INT(0, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_TRUE(env_set(env, "A=a"));
	TEST_ASSERT_EQUAL_INT(1, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_EQUAL_STRING("A", env->items[0].key);
	TEST_ASSERT_EQUAL_STRING("a", env->items[0].value);	
	close_env(&env);
}

void    testSetEnv_SetEnvWithEqualSign(void)
{
    char **envp = NULL;
	t_env *env = create_env(envp);

	TEST_ASSERT_EQUAL_INT(0, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_TRUE(env_set(env, "A="));
	TEST_ASSERT_EQUAL_INT(1, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_EQUAL_STRING("A", env->items[0].key);
	TEST_ASSERT_EQUAL_STRING("", env->items[0].value);	
	close_env(&env);
}

void    testSetEnv_SetEnvWithOutEqualSign(void)
{
    char **envp = NULL;
	t_env *env = create_env(envp);

	TEST_ASSERT_EQUAL_INT(0, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_TRUE(env_set(env, "A"));
	TEST_ASSERT_EQUAL_INT(1, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_EQUAL_STRING("A", env->items[0].key);
	TEST_ASSERT_NULL(env->items[0].value);	
	close_env(&env);
}

void    testSetEnv_SetEnvMultipleItems(void)
{
    char **envp = NULL;
	t_env *env = create_env(envp);

	TEST_ASSERT_EQUAL_INT(0, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_TRUE(env_set(env, "A"));
	TEST_ASSERT_EQUAL_INT(1, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_EQUAL_STRING("A", env->items[0].key);
	TEST_ASSERT_NULL(env->items[0].value);	
	TEST_ASSERT_TRUE(env_set(env, "BB="));
	TEST_ASSERT_EQUAL_INT(2, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_EQUAL_STRING("BB", env->items[1].key);
	TEST_ASSERT_EQUAL_STRING("", env->items[1].value);	
	TEST_ASSERT_TRUE(env_set(env, "CC=pp"));
	TEST_ASSERT_EQUAL_INT(3, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY * 2, env->capacity);
	TEST_ASSERT_EQUAL_STRING("CC", env->items[2].key);
	TEST_ASSERT_EQUAL_STRING("pp", env->items[2].value);	
	close_env(&env);
}

void    testSetEnv_UpdateEnv(void)
{
    char **envp = NULL;
	t_env *env = create_env(envp);

	TEST_ASSERT_EQUAL_INT(0, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_TRUE(env_set(env, "A"));
	TEST_ASSERT_EQUAL_INT(1, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_EQUAL_STRING("A", env->items[0].key);
	TEST_ASSERT_NULL(env->items[0].value);	
	TEST_ASSERT_TRUE(env_set(env, "BB="));
	TEST_ASSERT_EQUAL_INT(2, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_EQUAL_STRING("BB", env->items[1].key);
	TEST_ASSERT_EQUAL_STRING("", env->items[1].value);	
	TEST_ASSERT_TRUE(env_set(env, "CC=pp"));
	TEST_ASSERT_EQUAL_INT(3, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY * 2, env->capacity);
	TEST_ASSERT_EQUAL_STRING("CC", env->items[2].key);
	TEST_ASSERT_EQUAL_STRING("pp", env->items[2].value);
	TEST_ASSERT_TRUE(env_set(env, "A=aa"));	
	TEST_ASSERT_EQUAL_INT(3, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY * 2, env->capacity);
	TEST_ASSERT_EQUAL_STRING("A", env->items[0].key);
	TEST_ASSERT_EQUAL_STRING("aa", env->items[0].value);
	TEST_ASSERT_TRUE(env_set(env, "A"));
	TEST_ASSERT_EQUAL_INT(3, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY * 2, env->capacity);
	TEST_ASSERT_EQUAL_STRING("A", env->items[0].key);
	TEST_ASSERT_EQUAL_STRING("aa", env->items[0].value);
	TEST_ASSERT_TRUE(env_set(env, "CC=oo"));
	TEST_ASSERT_EQUAL_INT(3, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY * 2, env->capacity);
	TEST_ASSERT_EQUAL_STRING("CC", env->items[2].key);
	TEST_ASSERT_EQUAL_STRING("oo", env->items[2].value);		
	close_env(&env);
}

void    testSetEnv_RemoveEnvMultipleItems(void)
{
    char **envp = NULL;
	t_env *env = create_env(envp);

	TEST_ASSERT_EQUAL_INT(0, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_TRUE(env_set(env, "A"));
	TEST_ASSERT_EQUAL_INT(1, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_EQUAL_STRING("A", env->items[0].key);
	TEST_ASSERT_NULL(env->items[0].value);	
	TEST_ASSERT_TRUE(env_set(env, "BB="));
	TEST_ASSERT_EQUAL_INT(2, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_EQUAL_STRING("BB", env->items[1].key);
	TEST_ASSERT_EQUAL_STRING("", env->items[1].value);	
	TEST_ASSERT_TRUE(env_set(env, "CC=pp"));
	TEST_ASSERT_EQUAL_INT(3, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY * 2, env->capacity);
	TEST_ASSERT_EQUAL_STRING("CC", env->items[2].key);
	TEST_ASSERT_EQUAL_STRING("pp", env->items[2].value);
	env_remove(env, "BB");
	TEST_ASSERT_EQUAL_INT(2, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY * 2, env->capacity);
	TEST_ASSERT_EQUAL_STRING("A", env->items[0].key);
	TEST_ASSERT_NULL(env->items[0].value);
	TEST_ASSERT_EQUAL_STRING("CC", env->items[1].key);
	TEST_ASSERT_EQUAL_STRING("pp", env->items[1].value);	
	env_remove(env, "II");
	TEST_ASSERT_EQUAL_INT(2, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY * 2, env->capacity);
	TEST_ASSERT_EQUAL_STRING("A", env->items[0].key);
	TEST_ASSERT_NULL(env->items[0].value);
	TEST_ASSERT_EQUAL_STRING("CC", env->items[1].key);
	TEST_ASSERT_EQUAL_STRING("pp", env->items[1].value);
	env_remove(env, "A");
	TEST_ASSERT_EQUAL_INT(1, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY * 2, env->capacity);
	TEST_ASSERT_EQUAL_STRING("CC", env->items[0].key);
	TEST_ASSERT_EQUAL_STRING("pp", env->items[0].value);
	env_remove(env, "CC");
	TEST_ASSERT_EQUAL_INT(0, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY * 2, env->capacity);
	TEST_ASSERT_NULL(env->items[0].key);
	TEST_ASSERT_NULL(env->items[0].value);
	close_env(&env);
}

void    testGetEnv(void)
{
    char **envp = NULL;
	t_env *env = create_env(envp);

	TEST_ASSERT_EQUAL_INT(0, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_TRUE(env_set(env, "A"));
	TEST_ASSERT_EQUAL_INT(1, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_EQUAL_STRING("A", env->items[0].key);
	TEST_ASSERT_NULL(env->items[0].value);	
	TEST_ASSERT_TRUE(env_set(env, "BB="));
	TEST_ASSERT_EQUAL_INT(2, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_EQUAL_STRING("BB", env->items[1].key);
	TEST_ASSERT_EQUAL_STRING("", env->items[1].value);	
	TEST_ASSERT_TRUE(env_set(env, "CC=pp"));
	TEST_ASSERT_EQUAL_INT(3, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY * 2, env->capacity);
	TEST_ASSERT_EQUAL_STRING("CC", env->items[2].key);
	TEST_ASSERT_EQUAL_STRING("pp", env->items[2].value);
	char *v = env_get(env, "BB");
	TEST_ASSERT_EQUAL_STRING("", v);
	free(v);
	v = env_get(env, "A");
	TEST_ASSERT_EQUAL_STRING("", v);
	free(v);
	v = env_get(env, "CC");
	TEST_ASSERT_EQUAL_STRING("pp", v);
	free(v);
	close_env(&env);
}

void    testOutputEnv(void)
{
    char **envp = NULL;
	t_env *env = create_env(envp);

	TEST_ASSERT_EQUAL_INT(0, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_TRUE(env_set(env, "A"));
	TEST_ASSERT_EQUAL_INT(1, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_EQUAL_STRING("A", env->items[0].key);
	TEST_ASSERT_NULL(env->items[0].value);	
	TEST_ASSERT_TRUE(env_set(env, "BB="));
	TEST_ASSERT_EQUAL_INT(2, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, env->capacity);
	TEST_ASSERT_EQUAL_STRING("BB", env->items[1].key);
	TEST_ASSERT_EQUAL_STRING("", env->items[1].value);	
	TEST_ASSERT_TRUE(env_set(env, "CC=pp"));
	TEST_ASSERT_EQUAL_INT(3, env->size);
	TEST_ASSERT_EQUAL_INT(INIT_CAPACITY * 2, env->capacity);
	TEST_ASSERT_EQUAL_STRING("CC", env->items[2].key);
	TEST_ASSERT_EQUAL_STRING("pp", env->items[2].value);
	char **res = env_output(env);
	TEST_ASSERT_EQUAL_STRING("A=", res[0]);
	free(res[0]);
	TEST_ASSERT_EQUAL_STRING("BB=", res[1]);
	free(res[1]);
	TEST_ASSERT_EQUAL_STRING("CC=pp", res[2]);
	free(res[2]);
	TEST_ASSERT_NULL(res[3]);
	free(res);
	close_env(&env);
}

// Main function to run the tests
// Change INIT_CAPACITY to 2 before test.
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(testCreateEnv_Normal);
	RUN_TEST(testCreateEnv_Empty);
	RUN_TEST(testSetEnv_SetEnvNormal);
	RUN_TEST(testSetEnv_SetEnvWithEqualSign);
	RUN_TEST(testSetEnv_SetEnvWithOutEqualSign);
	RUN_TEST(testSetEnv_SetEnvMultipleItems);
	RUN_TEST(testSetEnv_UpdateEnv);
	RUN_TEST(testSetEnv_RemoveEnvMultipleItems);
	RUN_TEST(testGetEnv);
	RUN_TEST(testOutputEnv);
	return (UNITY_END());
}
