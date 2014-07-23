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

        typedef char* String;
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

        void __skip   (String description, Function function);

        void __after  (Function function);
        void __before (Function function);

        #define __should_declaration(suffix, type)                                                  \
            void __should_##suffix(String file, Int line, type actual, Bool negated, type expected)

        __should_declaration(bool  , Bool);
        __should_declaration(char  , char  );
        __should_declaration(short , short );
        __should_declaration(int   , int   );
        __should_declaration(long  , long  );
        __should_declaration(double, double);
        __should_declaration(float , float );
        __should_declaration(ptr   , void* );
        __should_declaration(string, char* );

    // ---------------------------------------------------------------------------
    // ----- MACROS API -----
    // ---------------------------------------------------------------------------

        #define context(name)     __attribute__((constructor)) void cspec_context_##name()

        #define describe(desc)    __describe(desc, ({ void __$__()
        #define it(desc)          __it      (desc, ({ void __$__()
        #define skip(desc)        __skip    (desc, ({ void __$__()

        #define after             __after (({ void __$__()
        #define before            __before(({ void __$__()

        #define end               __$__;}));

    // ---------------------------------------------------------------------------
    // ----- MACROS SHOULD -----
    // ---------------------------------------------------------------------------

        #define __should_call(suffix, actual)   __should_##suffix(__FILE__, __LINE__, (actual),

        #define should_bool(actual)             __should_call(bool  , actual)
        #define should_char(actual)             __should_call(char  , actual)
        #define should_short(actual)            __should_call(short , actual)
        #define should_int(actual)              __should_call(int   , actual)
        #define should_long(actual)             __should_call(long  , actual)
        #define should_double(actual)           __should_call(double, actual)
        #define should_float(actual)            __should_call(float , actual)
        #define should_ptr(actual)              __should_call(ptr   , actual)
        #define should_string(actual)           __should_call(string, actual)

        #define should(actual)    should_int(actual)

        #define not               !
        #define be                false

        #define truthy            , true)
        #define falsey            , false)
        #define null              , NULL)
        #define equal             ,
        #define to(expected)      expected)

#endif /* CSPEC_H_ */
