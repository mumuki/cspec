#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cspec.h"

FILE* devNull;

__attribute__((constructor)) void init() {
    devNull = fopen("/dev/null", "w");
}

#define MAX_SHOULDS_PER_IT 64

#define DESCRIBE_COLOR      "\x1b[0m"
#define FAILURE_COLOR       "\x1b[38;5;1m"
#define SUCCESS_COLOR       "\x1b[38;5;2m"
#define PENDING_COLOR       "\x1b[38;5;3m"
#define NO_COLOR            "\x1b[0m"
#define UNDERLINE           "\x1b[4m"

#define FAILURE_BULLET      "✖ "
#define SUCCESS_BULLET      "✔ "
#define PENDING_BULLET      "• "
#define DESCRIBE_BULLET     ""


typedef struct _should {
  Int     line;
  String  file;
  String  error;
} Should;

typedef struct _it {
    String _describe;
    String description;
    Bool is_failure;
    Should* shoulds[MAX_SHOULDS_PER_IT];
} It;

Int IT_COUNT            = 0;
Int SHOULD_COUNT        = 0;
Int DESCRIBE_DEEP_LEVEL = 0;

String CURRENT_DESCRIBE = "";

It**  ITS;

// ---------------------------------------------------------------------------
// ----- PRIVATE -----
// ---------------------------------------------------------------------------

    char*   __get_template (Bool neg, String format);
    It*     __it_create    (String description);
    It*     __current_it   ();
    int     __report       (void);
    Should* __should_create(String error, String file, Int line);
    void    __print_current_it_result();
    void    __print_it_result_detail(It* _it);
    char*   __get_spaces();

    #define print_description(description, add_desc, spaces_str, separator, type) { \
        char* spaces = __get_spaces();                                              \
        printf("%s%s%s%s%s%s%s%s\n", spaces, spaces_str, type##_##COLOR,            \
                type##_##BULLET, description, separator, add_desc, NO_COLOR);       \
        free(spaces);                                                               \
    }

// ---------------------------------------------------------------------------
// ----- STUB MAIN -----
// ---------------------------------------------------------------------------

    int main(void) {
        return __report();
    }

// ---------------------------------------------------------------------------
// ----- Describe -----
// ---------------------------------------------------------------------------

    void __describe_pre(String description) {
        if (DESCRIBE_DEEP_LEVEL == 0) puts("");
        DESCRIBE_DEEP_LEVEL++;
        CURRENT_DESCRIBE = description;
        print_description(description, "", "", "", DESCRIBE);
    }

    void __describe(String description, Function function) {
        __describe_pre(description);
        function();
        __describe_post(description);
    }

    void __describe_post(String description) {
        DESCRIBE_DEEP_LEVEL--;
    }

// ---------------------------------------------------------------------------
// ----- IT -----
// ---------------------------------------------------------------------------

    void __it_pre(String description) {
        ITS = realloc(ITS, sizeof(It*) * (++IT_COUNT));
        ITS[IT_COUNT - 1] = __it_create(description);
        SHOULD_COUNT = 0;
    }

    void __it(String description, Function function) {
        __it_pre(description);
        function();
        __it_post(description);
    }

    void __it_post(String description) {
        __print_current_it_result();
    }

// ---------------------------------------------------------------------------
// ----- SHOULDS -----
// ---------------------------------------------------------------------------

    #define __should_definition(suffix, type, comparator, format)                                   \
        void __should_##suffix(String file, Int line, type actual, Bool negated, type expected) {   \
            It* _it = __current_it();                                                               \
            _it->is_failure = negated ? (comparator) : !(comparator);                               \
            if (_it->is_failure) {                                                                  \
                String template = __get_template(negated, format);                                  \
                int message_size = fprintf(devNull, template, expected, actual);                    \
                String error = malloc(message_size + 1);                                            \
                sprintf(error, template, expected, actual);                                         \
                _it->shoulds[SHOULD_COUNT++] = __should_create(error, file, line);                  \
                free(template);                                                                     \
            }                                                                                       \
        }                                                                                           \

    __should_definition(bool  , String, strcmp(actual, expected) == 0 , "%s");
    __should_definition(char  , char  , actual == expected            , "%c");
    __should_definition(short , short , actual == expected            , "%i");
    __should_definition(int   , int   , actual == expected            , "%i");
    __should_definition(long  , long  , actual == expected            , "%l");
    __should_definition(double, double, actual == expected            , "%f");
    __should_definition(float , float , actual == expected            , "%f");
    __should_definition(ptr   , void* , actual == expected            , "%p");
    __should_definition(string, String, strcmp(actual, expected) == 0 , "%s");

// ---------------------------------------------------------------------------
// ----- PRIVATE -----
// ---------------------------------------------------------------------------

    It* __it_create(String description) {
        It* self = malloc(sizeof(It));
        self->description = description;
        self->_describe = CURRENT_DESCRIBE;
        self->is_failure = false;
        int i; for (i = 0; i < MAX_SHOULDS_PER_IT; self->shoulds[i++] = NULL);
        return self;
    }

    void __it_destroy(It* self) {
        free(self);
    }

    Should* __should_create(String error, String file, Int line) {
        Should* self = malloc(sizeof(Should));
        self->error = error;
        self->file = file;
        self->line = line;
        return self;
    }

    void __should_destroy(Should* self) {
        free(self->error);
        free(self);
    }

    #define VALUE UNDERLINE "%s" NO_COLOR

    char* __get_template(Bool neg, String format) {
        char* template = malloc(128);
        memset(template, '\0', 128);
        sprintf(template, "%sxpected <" VALUE "> but was <" VALUE ">", !neg ? "E" : "Not e", format, format);
        return template;
    }

    int __report(void) {
        puts("\n");
        puts("  " "\x1b[4m" "Summary\n" NO_COLOR);
        int i, failure_count = 0;
        for (i = 0; i < IT_COUNT; i++) {
            It* _it = ITS[i];
            if (_it->is_failure) {
                failure_count++;
                __print_it_result_detail(_it);
            }
            __it_destroy(_it);
        }
        free(ITS);
        fclose(devNull);
        printf(SUCCESS_COLOR "  %d success\n" NO_COLOR, IT_COUNT - failure_count);
        if (failure_count > 0) printf(FAILURE_COLOR "  %d failure\n" NO_COLOR, failure_count);
        puts("");
        return failure_count;
    }

    It* __current_it() {
        return ITS[IT_COUNT - 1];
    }

// ---------------------------------------------------------------------------
// ----- FANCY REPORT -----
// ---------------------------------------------------------------------------

    void __print_current_it_result() {
        It* _it = __current_it();
        if (_it->is_failure) {
            print_description(_it->description, "", "  ", "", FAILURE);
        } else {
            print_description(_it->description, "", "  ", "", SUCCESS);
        }
    }

    void __print_it_result_detail(It* _it) {
      print_description(_it->_describe, _it->description, "    ", " - ", FAILURE);
      int j; for (j = 0; j < MAX_SHOULDS_PER_IT && _it->shoulds[j] != NULL; j++) {
          Should* _should = _it->shoulds[j];
          char* spaces = __get_spaces();
          printf("%s      - %s [%s:%d]\n", spaces, _should->error, _should->file, _should->line);
          __should_destroy(_should);
          free(spaces);
      }
      puts("");
    }

    char* __get_spaces() {
        int spaces_count = 2 * DESCRIBE_DEEP_LEVEL;
        char* spaces = malloc(spaces_count + 1);
        memset(spaces, ' ', spaces_count);
        spaces[spaces_count] = '\0';
        return spaces;
    }
