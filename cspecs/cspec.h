/*
 * cspec.h
 *
 *      Author: Federico Scarpa
 */

#ifndef CSPEC_H_
#define CSPEC_H_

    #include <stdbool.h>

    // ---------------------------------------------------------------------------
    // ----- TYPES -----
    // ---------------------------------------------------------------------------

        typedef const char* String;
        typedef void(*Function)(void);
        typedef bool Bool;
        typedef int Int;

    // ---------------------------------------------------------------------------
    // ----- DECLARATION -----
    // ---------------------------------------------------------------------------

        void __describe_pre (String description);
        void __describe     (String description, Function function);
        void __describe_post(String description);

        void __it_pre (String description);
        void __it     (String description, Function function);
        void __it_post(String description);

        void __should(String file, Int line, Int actual, Bool negated, Int expected);

    // ---------------------------------------------------------------------------
    // ----- MACROS API -----
    // ---------------------------------------------------------------------------

        #define context     __attribute__((constructor)) void

        #define describe    __describe(
        #define it          __it      (

        #define begin       , ({ void __$__() {
        #define end         }; __$__;}));

    // ---------------------------------------------------------------------------
    // ----- MACROS SHOULD -----
    // ---------------------------------------------------------------------------

        #define should(actual)    __should(__FILE__, __LINE__, actual,
        #define be                false
        #define not               !
        #define equal             ,
        #define to(expected)      expected)


#endif /* CSPEC_H_ */
