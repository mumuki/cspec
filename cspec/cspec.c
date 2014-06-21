/*
 * cspec.c
 *
 *      Author: Federico Scarpa
 */

#include "cspec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int DESCRIBE_DEEP_LEVEL = 0;

static int PENDING_SHOULDS = 0;
static int SUCCESS_SHOULDS = 0;
static int FAILURE_SHOULDS = 0;

static int IT_FAILURES_SHULDS = 0;

#define DESCRIBE_COLOR      "\x1b[0m"
#define FAILURE_COLOR       "\x1b[38;5;1m"
#define SUCCESS_COLOR       "\x1b[38;5;2m"
#define PENDING_COLOR       "\x1b[38;5;3m"
#define NO_COLOR            "\x1b[0m"

#define DESCRIBE_BULLET     ""
#define FAILURE_BULLET      "✖ "
#define SUCCESS_BULLET      "✔ "
#define PENDING_BULLET      "• "

#define print_description(description, add_desc, spaces_str, type) {                                        \
    char* spaces = get_spaces();                                                                            \
    printf("%s%s%s%s%s%s%s\n", spaces, spaces_str, type##_##COLOR,                                          \
            type##_##BULLET, description, add_desc, NO_COLOR);                                              \
    free(spaces);                                                                                           \
}                                                                                                           \

// ---------------------------------------------------------------------------
// ----- PRIVATES -----
// ---------------------------------------------------------------------------

    char* get_spaces();
    void _cspec_print_report();

// ---------------------------------------------------------------------------
// ----- "PUBLICS" -----
// ---------------------------------------------------------------------------

    void _cspec_describe_pre(const char* description) {
        if (DESCRIBE_DEEP_LEVEL == 0) puts("");
        DESCRIBE_DEEP_LEVEL++;
        print_description(description, "", "", DESCRIBE);
    }

    void _cspec_describe_post(const char* description) {
        DESCRIBE_DEEP_LEVEL--;
    }

    void _cspec_it_pre(const char* description) {
        IT_FAILURES_SHULDS = 0;
    }

    void _cspec_it_post(const char* description) {
        if (IT_FAILURES_SHULDS != 0) {
            print_description(description, "", "  ", FAILURE);
        } else {
            SUCCESS_SHOULDS++;
            print_description(description, "", "  ", SUCCESS);
        }
    }

    void _cspec_should_power(const char* filename, int line, void* actual, int(*f)(void*, void*), void* expected) {
        _cspec_should(f(actual, expected), filename, line);
    }

    void _cspec_should(int boolean, const char* filename, int line) {
        if (IT_FAILURES_SHULDS == 0 && !boolean) {
            FAILURE_SHOULDS++;
            IT_FAILURES_SHULDS++;
        }
    }

    void _cspec_skip(const char* description) {
        PENDING_SHOULDS++;
        print_description(description, " (WTF - SKIPPED)", "  ", PENDING);
    }

    #define print_result(description, type) {                                                   \
        if (type##_##SHOULDS > 0) {                                                             \
            printf("%s  %i %s%s\n", type##_##COLOR, type##_##SHOULDS, description, NO_COLOR);   \
        }                                                                                       \
    }                                                                                           \

    int _cspec_get_result(void) {
        _cspec_print_report();
        return FAILURE_SHOULDS;
    }

    int _cspec_should_is_equal(void* actual, void* expected) {
        return actual == expected;
    }

    int _cspec_should_not_equal(void* actual, void* expected) {
        return actual != expected;
    }

    int _cspec_should_string_equal(void* actual, void* expected) {
        return strcmp((char*) actual,(char*) expected) == 0;
    }

// ---------------------------------------------------------------------------
// ----- PRIVATES -----
// ---------------------------------------------------------------------------

    char* get_spaces() {
        int spaces_count = 2 * DESCRIBE_DEEP_LEVEL;
        char* spaces = malloc(spaces_count + 1);
        memset(spaces, ' ', spaces_count);
        spaces[spaces_count] = '\0';
        return spaces;
    }

    void _cspec_print_report() {
        puts("");
        print_result("Success", SUCCESS);
        print_result("Pending", PENDING);
        print_result("Failure", FAILURE);
        puts("");
    }

    int main(int argc, char **argv) {
        return CSPEC_RESULT;
    }

