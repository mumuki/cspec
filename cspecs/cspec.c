#include <stdio.h>
#include "cspec.h"

int main(void) {
    return 0;
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
        puts(description);
    }

    void __it(String description, Function function) {
        __it_pre(description);
        function();
        __it_post(description);
    }

    void __it_post(String description) {

    }

// ---------------------------------------------------------------------------
// ----- SHOULDS -----
// ---------------------------------------------------------------------------

    void __should(String file, Int line, Int actual, Bool negated, Int expected) {
      String format = negated ? "Not expected <%i> but was <%i>" : "Expected <%i> but was <%i>";
      Bool comparator = negated ? actual == expected : actual != expected;
      if (comparator) {
        printf(format, expected, actual);
        printf(" - %s:%i\n", file, line);
      }
    }
