# cspec 
> Small behavior driven development ([BDD](http://en.wikipedia.org/wiki/Behavior-driven_development)) framework for C.

## Show me an example, please!

```C
#include <stdio.h>
#include <stdbool.h>
#include <cspecs/cspec.h>

context (example) {

    describe("Hello world") {

        it("true should be true") {
            should_bool(true) be equal to(true);
        } end

        it("true shouldn't be false") {
            should_bool(true) not be equal to(false);
        } end

        it("this test will fail because 10 is not equal to 11") {
            should_int(10) be equal to(11);
        } end

        skip("this test will fail because \"Hello\" is not \"Bye\"") {
            should_string("Hello") be equal to("Bye");
        } end

    } end

}
```

* Compile: `gcc cspecExample.c -o cspecExample -lcspecs`
* Execute: `./cspecExample`

```

  Hello world
    ✔ true should be true
    ✔ true shouldn't be false
    1) this test will fail because 10 is not equal to 11
    • this test will fail because "Hello" is not "Bye"


  Summary

    1) Hello world - this test will fail because 10 is not equal to 11
      - Expected <11> but was <10> [./cspecExample.c:18]

  2 success
  1 failure
  1 pending

```

## Let's get started!
### How do I install it?
1. `git clone https://github.com/pepita-remembrance/cspec.git`
1. `cd cspec`
1. `make`
1. `sudo make install`

### Now, what should I do?
* Write your C code
* Write your specs
* Compile with the `-l`should`` option. For example: `gcc cspecExample.c -o cspecExample -lcspec`
* Run it on the console: `./cspecExample`

### What does cspec offer me?
It offers you a set of operations - like [RSpec](http://rspec.info/), [Mocha](http://visionmedia.github.io/mocha/) or  [Jasmine](http://jasmine.github.io/) - that allows you to make [oriented behavior](http://en.wikipedia.org/wiki/Behavior-driven_development) (unit and integration code) tests.

## How do I use the framework?

###context
Each behaviour to test must be declared within a `context`. The syntax to define a `context` is shown below:

```C
context(<identifier>) {
    /* You're inside the context */
}
```
Inside a `context`, you can write functions and call them in your tests, you can also include files (.h), define macros and write scenarios using `describe`.

###describe
Each scenario is written inside a `describe`, declared in this way:

```C
describe("Brief description of the scenario") {
    /* Here goes the code */
} end
```

Again, inside a `describe` you can write functions and call them in your tests, include files (.h), define macros and write the tests using `it`.

###it

Each `it` represents a test.

```C
it("Brief description of the test") {
    /* Here goes the test code, along with the assertions */
} end
```

Inside it, you have to write the assertions about the behaviour you want to test. In order to do that cspec has a set of basic operations to do that, the `should` statements.

###should

Each `should` is an assertion, that expects 2 values. The first is the actual value and the second, the expected one.

```C
should_bool(<actual_boolean>) be equal to(<expected_boolean>);
should_bool(<actual_boolean>) not be equal to(<unexpected_boolean>);

should_char(<caracter_actual>) be equal to(<caracter_esperado>);
should_char(<caracter_actual>) not be equal to(<caracter_no_esperado>);

should_short(<actual_number>) be equal to(<expected_number>);
should_short(<actual_number>) not be equal to(<unexpected_number>);

should_int(<actual_number>) be equal to(<expected_number>);
should_int(<actual_number>) not be equal to(<unexpected_number>);

should_long(<actual_number>) be equal to(<expected_number>);
should_long(<actual_number>) not be equal to(<unexpected_number>);

should_float(<actual_float>) be equal to(<expected_float>);
should_float(<actual_float>) not be equal to(<unexpected_float>);

should_double(<decimal_actual>) be equal to(<decimal_esperado>);
should_double(<decimal_actual>) not be equal to(<decimal_no_esperado>);

should_ptr(<actual_pointer>) be equal to(<expected_pointer>);
should_ptr(<actual_pointer>) not be equal to(<unexpected_pointer>);

should_string(<actual_word>) be equal to(<expected_word>);
should_string(<actual_word>) not be equal to(<unexpected_word>);
```
Also, cspec offers [syntactic sugar](http://en.wikipedia.org/wiki/Syntactic_sugar) for some of the assertions, like the following examples:

```C
should_bool(<actual_boolean>) be truthy;
should_bool(<actual_boolean>) not be truthy;

should_bool(<actual_boolean>) be falsey;
should_bool(<actual_boolean>) not be falsey;

should_ptr(<actual_pointer>) be null;
should_ptr(<actual_pointer>) not be null;
```

###Hooks - before y after

Sometimes the scenarios, initial configurations, or deallocation of the variables get repeated between tests. In order to handle that, inside each `describe`, you can add a block code to execute `before` and `after` each test (`it`).

####before
```C
before {
    /* Code to execute before each test */
} end
```

####after
```C
after {
    /* Code to execute after each test */
} end
```

**Note:** As stated before, the context and the describe are executed secuentially, that's why it's __very important__ to remember that the `before` and `after` must be declared in the beggining of the `describe` scenario, even before the first test.


##Now let's see a complete example, with the execution flow

```C
#include <stdio.h>
#include <stdlib.h>
#include <cspecs/cspec.h>

context (complete_example) {

    describe("Describe 1") {

        int *a = NULL,
             b;

        before {
            puts("before 1");
            a = malloc(sizeof(int));
            *a = 10;
            b = 20;
        } end

        after {
            puts("after 1");
            free(a);
            a = NULL;
        } end

        it("*a should be 10 and b should be 20") {
            should_int(*a) be equal to(10);
            should_int(b) be equal to(20);
        } end

        describe("Describe 2") {

            before {
                puts("before 2");
                *a = 30;
                b = 15;
            } end

            after {
                puts("after 2");
                free(a);
                a = NULL;
            } end

            it("*a should be 30 and b should be 15") {
                should_int(*a) be equal to(30);
                should_int(b) be equal to(15);
            } end

            describe("Describe 3") {

                before {
                    puts("before 3");
                    b = 150;
                } end

                after {
                    puts("after 3");
                    free(a);
                    a = NULL;
                } end

                it("*a should be 30 and b should be 150") {
                    should_int(*a) be equal to(30);
                    should_int(b) be equal to(150);
                } end

            } end

        } end

        describe("Describe 4") {

            it("*a should be 10 and b should be 20") {
                should_int(*a) be equal to(10);
                should_int(b) be equal to(20);
            } end

        } end

    } end

}
```
Once the code has been compiled and executed, it'll give us a report of all the tests like the following:

```

  Describe 1
before 1
    ✔ *a should be 10 and b should be 20
after 1
    Describe 2
before 1
before 2
      ✔ *a should be 30 and b should be 15
after 2
after 1
      Describe 3
before 1
before 2
before 3
        ✔ *a should be 30 and b should be 150
after 3
after 2
after 1
    Describe 4
before 1
      ✔ *a should be 10 and b should be 20
after 1


  Summary

  4 success

```

#### Are you using Eclipse IDE and it's showing errors all over the project?
You need to add cspec to the project dependencies.
In order to do that, follow these steps...

1. Right click on the project => `Properties`
1. In the right panel go to `C/C++ Build` => `Settings` => `Tool Settings` => `GCC C++ Linker` => `Libraries`
1. In the right pannel, above (`Libraries (-l)`), click on `add` and then write `cspec`
1. Apply changes
1. Recompile the project


##License

This framework uses the GPLv3 as license. Fork it and contribute with the project!

######Thanks!
