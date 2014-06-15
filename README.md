cspec
=====
###Little spec framework in C/C++
#####- C/C++ specs por humans

###Why can I use cspec?
It is a little famework for C which provides a DSL, like jasmine (javascript), to make C testing friendlier.

###Install
* Checkout this repo: `git checkout https://github.com/fedescarpa/cspec.git`
* Enter into cspec folder: `cd cspec`
* Build the project:`sudo make`
* Install: `sudo make install`

###Uninstall
* Run: `sudo make clean uninstall`

###How can I use it?
It's very simple, let's see an example:

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
* Compile it with `-lcspec`
* Example: `gcc filename.(c|cpp) -o filename -lcspec`
* Run the example `./filename`

```bash
  Numbers
    ✔ 1 should be equals 1

  1 Success
```
###Should expectations
* **should_be_true**(boolen)
* **should_be_false**(boolen)
* **should_be_null**(pointer)
* **should_not_be_null**(pointer)
* **should_be_equals**(expected, actual)
* **should_not_be_equals**(expected, actual)
* **should_be_equals_strings**(expected, actual)
* **should_not_be_equals_strings**(expected, actual)

###Cspec Report
The last line in the example `return CSPEC_RESULT;` prints the report and return the exit code. Is very important that you don't forget to add this line.

###Hook methods
* Each "describe" have its own hooks.

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
It print the following report

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
