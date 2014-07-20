#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cspec.h"

FILE* devNull;

__attribute__((constructor)) void init() {
    devNull = fopen("/dev/null", "w");
}

#define MAX_SHOULDS_PER_IT 64

typedef struct _should {
  Int     line;
  String  file;
  char*  error;
} Should;

typedef struct _it {
    char* description;
    Should* shoulds[MAX_SHOULDS_PER_IT];
} It;

Int   IT_COUNT      = 0;
Int   SHOULD_COUNT  = 0;

It**  ITS;

// ---------------------------------------------------------------------------
// ----- PRIVATE -----
// ---------------------------------------------------------------------------

    char*   __get_template (Bool neg, String format);
    It*     __it_create   (String description);
    int     __report      (void);
    Should* __should_create(char* error, String file, Int line);

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
        puts(description);
    }

    void __describe(String description, Function function) {
        __describe_pre(description);
        function();
        __describe_post(description);
    }

    void __describe_post(String description) {

    }

// ---------------------------------------------------------------------------
// ----- IT -----
// ---------------------------------------------------------------------------

    void __it_pre(String description) {
        SHOULD_COUNT = 0;
        ITS = realloc(ITS, sizeof(It*) * (++IT_COUNT));
        ITS[IT_COUNT - 1] = __it_create(description);
    }

    void __it(String description, Function function) {
        __it_pre(description);
        function();
        __it_post(description);
    }

    void __it_post(String description) {
        puts(description);
    }

// ---------------------------------------------------------------------------
// ----- SHOULDS -----
// ---------------------------------------------------------------------------

    void __should(String format, String file, Int line, Int actual, Bool negated, Int expected) {
      Bool comparator = negated ? actual == expected : actual != expected;
      if (comparator) {
          char* template = __get_template(negated, format);
          int message_size = fprintf(devNull, template, expected, actual);
          char* error = malloc(message_size + 1);
          sprintf(error, template, expected, actual);
          ITS[IT_COUNT - 1]->shoulds[SHOULD_COUNT++] = __should_create(error, file, line);
          free(template);
          free(error);
      }
    }

// ---------------------------------------------------------------------------
// ----- PRIVATE -----
// ---------------------------------------------------------------------------

    It* __it_create(String description) {
        It* self = malloc(sizeof(It));
        self->description = strdup(description);
        int i; for (i = 0; i < MAX_SHOULDS_PER_IT; self->shoulds[i++] = NULL);
        return self;
    }

    Should* __should_create(char* error, String file, Int line) {
        Should* self = malloc(sizeof(Should));
        self->error = strdup(error);
        self->file = file;
        self->line = line;
        return self;
    }

    char* __get_template(Bool neg, String format) {
        char* template = malloc(64);
        memset(template, '\0', 64);
        sprintf(template, "%sxpected <%s> but was <%s>", !neg ? "E" : "Not e", format, format);
        return template;
    }

    int __report(void) {
        puts("");
        int i; for (i = 0; i < IT_COUNT; i++) {
            It* _it = ITS[i];
            puts(_it->description);
            int j; for (j = 0; j < MAX_SHOULDS_PER_IT && _it->shoulds[j] != NULL; j++) {
                Should* _should = _it->shoulds[j];
                printf("%s - <%s:%d>\n", _should->error, _should->file, _should->line);
                free(_should->error);
                free(_should);
            }
            free(_it->description);
            free(_it);
        }
        free(ITS);
        fclose(devNull);
        return 0;
    }

