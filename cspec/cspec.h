/*
 * cspec.h
 *
 *      Author: Federico Scarpa
 */

#ifndef CSPEC_H_
#define CSPEC_H_

#ifdef __cplusplus

    #define __function(body)                            \
        struct Local {                                  \
            static void body;                           \
        };                                              \
        Local::                                         \

    #define __lambda(ret, body) ({                      \
        struct Local {                                  \
            static ret _$ body;                         \
        };                                              \
        (void*)(Local::_$;)                             \
    })                                                  \

    #define __f_agn(var, body)                          \
        struct Local {                                  \
            static void body;                           \
        };                                              \
        var = Local::function;                          \

    extern "C" {

#else

    #define __function(body)                void body;
    #define __lambda(ret, body)             (void*)({ ret _$ body; _$; })
    #define __f_agn(var, body)              void body; var = function;

#endif

    #include <string.h>
    #include <stdlib.h>

    #ifndef DEFAULT_MAX_SUITES
        #define DEFAULT_MAX_SUITES 1024
    #endif

    #define CSPEC(name, block)                                                          \
        __attribute__ ((constructor))                                                   \
        void _suite_name_##name(void) {                                                 \
            __function(block); function();                                              \
        }                                                                               \

    void _cspec_describe_pre(const char* description);
    void _cspec_describe_post(const char* description);

    void _cspec_it_pre(const char* description);
    void _cspec_it_post(const char* description);

    void _cspec_skip(const char* description);
    void _cspec_should(int boolean, const char* filename, int line);

    int _cspec_get_result(void);

    static void __cspec_noop() {}

    static void(*__before)(void) = __cspec_noop;
    static void(*__after)(void) = __cspec_noop;

    #define describe(description, block) {                      \
        _cspec_describe_pre(description);                       \
        __function(block) function();                           \
        _cspec_describe_post(description);                      \
    }                                                           \

    #define it(description, block) {                            \
        __before();                                             \
        _cspec_it_pre(description);                             \
        __function(block) function();                           \
        _cspec_it_post(description);                            \
        __after();                                              \
    }                                                           \

    #define skip(description, block) {                          \
        _cspec_skip(description);                               \
    }                                                           \

    #define before(block)                                       \
        static void(*__before)(void);                           \
        { __f_agn(__before, block); }                           \

    #define after(block)                                        \
        static void(*__after)(void);                            \
        { __f_agn(__after, block); }                            \

    #define _should(boolean) {                                  \
        _cspec_should(boolean, __FILE__, __LINE__);             \
    }                                                           \

    #define should_be_true(bool)        _should((bool))
    #define should_be_false(bool)       _should(!(bool))

    #define should_be_null(ptr)         _should((ptr) == NULL)
    #define should_not_be_null(ptr)     _should((ptr) != NULL)

    #define should_be_equals(expected, actual)          _should((actual) == (expected))
    #define should_not_be_equals(expected, actual)      _should((actual) != (expected))

    #define should_be_equals_strings(expected, actual)      _should(strcmp(actual, expected) == 0)
    #define should_not_be_equals_strings(expected, actual)  _should(strcmp(actual, expected) != 0)


    // ---------------------------------------------------------------------------
    // ----- DSL -----
    // ---------------------------------------------------------------------------

    #include <stdint.h>

    int _cspec_should_be();
    int _cspec_should_not_be();

    int _cspec_should_be_true();
    int _cspec_should_be_false();
    void* _cspec_should_be_null();

    typedef int8_t  t_cbool;
    typedef int8_t  t_cchar;
    typedef int16_t t_cshort;
    typedef int32_t t_cint;
    typedef int64_t t_clong;
    typedef double  t_cdouble;
    typedef float   t_cfloat;
    typedef char*   t_cstring;
    typedef void*   t_cptr;

    #define __should_type(type)                                                                             \
        void _cspec_should_##type(char* file, int line, t_c##type actual, int negated, t_c##type expected);

    #define __should_call(type, actual) \
        _cspec_should_##type(__FILE__, __LINE__, (actual),

    __should_type(bool)
    __should_type(char)
    __should_type(short)
    __should_type(int)
    __should_type(long)
    __should_type(double)
    __should_type(float)
    __should_type(string)
    __should_type(ptr)

    #define should_bool(actual)     __should_call(bool, actual)
    #define should_char(actual)     __should_call(char, actual)
    #define should_short(actual)    __should_call(short, actual)
    #define should_int(actual)      __should_call(int, actual)
    #define should_long(actual)     __should_call(long, actual)
    #define should_double(actual)   __should_call(double, actual)
    #define should_float(actual)    __should_call(float, actual)
    #define should_string(actual)   __should_call(string, actual)
    #define should_ptr(actual)      __should_call(ptr, actual)
    #define should(actual)          __should_call(ptr, actual)

    #define be                      _cspec_should_be(),
    #define not_be                  _cspec_should_not_be(),

    #define null                    _cspec_should_be_null())
    #define truthy                  _cspec_should_be_true())
    #define falsey                  _cspec_should_be_false())

    #define equal                   (
    #define to(expected)            expected))

    #define CSPEC_RESULT            _cspec_get_result();

#ifdef __cplusplus
    }
#endif

#endif /* CSPEC_H_ */
