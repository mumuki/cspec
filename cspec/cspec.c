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


typedef enum {
    cbool, cchar, cshort, cint, clong, cdouble, cfloat, cstring, cptr
} cspec_type_t;

typedef union {
    char cbool;
    char cchar;
    short cshort;
    int cint;
    long clong;
    double cdouble;
    float cfloat;
    char* cstring;
    void* cptr;
} _union_t;

typedef struct {
    char* file;
    char* format;
    cspec_type_t type;
    _union_t actual;
    _union_t expected;
} _should_t;

#define MAX_COUNT 1024
static _should_t* SHOULDS[] = { [ 0 ... (MAX_COUNT - 1)] = NULL };
static int SHOULD_COUNT = 0;

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
    void _cspec_print_should_report();

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
            _cspec_print_should_report();
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

    int _cspec_should_be() {
        return 0;
    }

    int _cspec_should_not_be() {
        return 1;
    }

    int _cspec_should_be_true() {
        return 1;
    }

    int _cspec_should_be_false() {
        return 0;
    }

    void* _cspec_should_be_null() {
        return NULL;
    }

    #define __should_create(fmt, filename, real_type, act, exp) ({              \
        _should_t* should = malloc(sizeof(_should_t));                          \
        should->file = file;                                                    \
        char* frmt = malloc(27); /* MAGIC NUMBER */                             \
        strcpy(frmt,"Expected <" fmt "> but was <" fmt ">");                    \
        should->format = frmt;                                                  \
        should->type = real_type;                                               \
        should->actual.real_type = act;                                         \
        should->expected.real_type = exp;                                       \
        should;                                                                 \
    })                                                                          \

    #define __should_def(type, actual, fmt, comparator)                                                         \
        void _cspec_should_##type(char* file, int line, t_c##type actual, int negated, t_c##type expected) {    \
            int bool = comparator;                                                                              \
            bool = negated ? !(bool) : (bool);                                                                  \
            if (!bool) SHOULDS[SHOULD_COUNT++] = __should_create(fmt, file, c##type, actual, expected);         \
            if (IT_FAILURES_SHULDS == 0 && !bool) {                                                             \
                FAILURE_SHOULDS++;                                                                              \
                IT_FAILURES_SHULDS++;                                                                           \
            }                                                                                                   \
        }                                                                                                       \

    __should_def(bool, actual, "%s", !!actual == !!expected)
    __should_def(char, actual, "%c", actual == expected)
    __should_def(short, actual, "%h", actual == expected)
    __should_def(int, actual, "%i", actual == expected)
    __should_def(long, actual, "%l", actual == expected)
    __should_def(double, actual, "%f", actual == expected)
    __should_def(float, actual, "%f", actual == expected)
    __should_def(string, actual, "%s", strcmp(actual, expected) == 0)
    __should_def(ptr, actual, "%p", actual == expected)

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

    void _cspec_print_should_report() {
        int i;
        char* spaces = get_spaces();
        for(i = 0; i < MAX_COUNT && SHOULDS[i] != NULL; i++) {
            _should_t* should = SHOULDS[i];
            printf("%s    " FAILURE_BULLET, spaces);
            char* bool(_union_t b) { return b.cbool != 0 ? "true" : "false"; }
            switch (should->type) {
                case cbool:   { printf(should->format, bool(should->expected), bool(should->actual));     break; }
                case cchar:   { printf(should->format, should->expected.cchar, should->actual.cchar);     break; }
                case cshort:  { printf(should->format, should->expected.cshort, should->actual.cshort);   break; }
                case cint:    { printf(should->format, should->expected.cint, should->actual.cint);       break; }
                case clong:   { printf(should->format, should->expected.clong, should->actual.clong);     break; }
                case cdouble: { printf(should->format, should->expected.cdouble, should->actual.cdouble); break; }
                case cfloat:  { printf(should->format, should->expected.cfloat, should->actual.cfloat);   break; }
                case cstring: { printf(should->format, should->expected.cstring, should->actual.cstring); break; }
                case cptr:    { printf(should->format, should->expected.cptr, should->actual.cptr);       break; }
                default: break;
            }
            printf(" - %s\n", should->file);
            free(should->format);
            free(should);
            SHOULDS[i] = NULL;
        }
        free(spaces);
    }
