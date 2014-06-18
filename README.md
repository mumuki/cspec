cspec - C/C++ specs for humans
==============================

### In a nutshell

```c
#include <cspec/cspec.h>

int main(int argc, char **argv) {

    describe("Numbers", function () {

        it("1 should be equals 1", function () {
           should_be_equals(1,1);
        });

    });

    return CSPEC_RESULT;

}
```
* `gcc numbers.c -o numbers -lcspec`
* Run the example `./numbers`

```bash
  Numbers
    ✔ 1 should be equals 1

  1 Success
```

### Getting started

#### How to install it?
* Checkout this repo: `git checkout https://github.com/fedescarpa/cspec.git`
* Enter into cspec folder: `cd cspec`
* Build the project:`sudo make`
* Install: `sudo make install`

#### How to uninstall it?
* Run: `sudo make clean uninstall`

#### What's next?
* Write some code and some tests
* Compile your files using `-lcspec` to link the library, e.g. `gcc helloWorld.cpp -o helloWorld -lcspec`
* Run it, e.g. `./helloWorld`
* Add more features!

#### Help me, Eclipse shows error everywhere!
Have you added cspec as a library? Probably not, so let me help you with that:

* Right click on your project and choose `Properties`
* Go to `C/C++ Build -> Settings -> Tool Settings -> GCC C++ Linker -> Libraries`
* On the upper panel, `Libraries (-l)` press the add button and add `cspec`

### Usage

#### Expectations functions
* `should_be_true(boolean)`
* `should_be_false(boolean)`
* `should_be_null(pointer)`
* `should_not_be_null(pointer)`
* `should_be_equals(expected, actual)`
* `should_not_be_equals(expected, actual)`
* `should_be_equals_strings(expected, actual)`
* `should_not_be_equals_strings(expected, actual)`

#### Fluent DSLish expectations
* In order
 1. `should(value)`
 2. `be | not_be | be_string`
 3. `truthy | falsey | equal(expected) | null`

```c
#include <cspec/cspec.h>
#include <stdio.h>

int main(int argc, char **argv) {

    describe("describe 1", function () {

        char* world = "World";

        it("it 1", function () {
            should(0) be falsey;
        });

        it("it 2", function () {
            should(1) be truthy;
        });

        describe("describe 2", function () {

            it("it 3", function () {
                should(1) be equal(2);
            });

            it("it 4", function () {
                should(world) be_string equal("World");
            });

        });

    });

    return CSPEC_RESULT;

}
```

**WARNING!** The last line in the example (`return CSPEC_RESULT;`) prints the report and returns the exit code. If you forget to add it, you won't see anything

#### Hook methods (aka Setup / TearDown)
Each "describe" have its own hooks.

```c
#include <cspec/cspec.h>
#include <stdio.h>

int main(int argc, char **argv) {

    describe("describe 1", function () {

        static int mundo;

        before(function () {
            puts("BEFORE");
            mundo = 1;
        });

        after(function () {
            puts("AFTER");
        });

        it("it 1", function () {
            should_be_false(1);
        });

        it("it 2", function () {
            should_be_true(1);
        });

        describe("describe 2", function () {

            it("it 3", function () {
                should_be_equals(1, mundo);
            });

            skip("it 4", function () {
                should_be_equals_strings("MUNDO", mundo);
            });

        });

    });

    return CSPEC_RESULT;

}
```

It prints the following report:


```
  describe 1
BEFORE
    ✖ it 1
AFTER
BEFORE
    ✔ it 2
AFTER
    describe 2
      ✔ it 3
      • it 4 (WTF - SKIPPED)

  2 Success
  1 Pending
  1 Failure
```
