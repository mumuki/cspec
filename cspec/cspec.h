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

    #define __f_agn(var, body)                          \
        struct Local {                                  \
            static void body;                           \
        };                                              \
        var = Local::function;                          \

    extern "C" {

#else

    #define __function(body)                void body;
    #define __f_agn(var, body)              void body; var = function;

#endif

    #include <string.h>
    #include <string.h>

    void _cspec_describe_pre(const char* description);
    void _cspec_describe_post(const char* description);

    void _cspec_it_pre(const char* description);
    void _cspec_it_post(const char* description);

    void _cspec_skip(const char* description);
    void _cspec_should(int boolean, const char* filename, int line);

    int _cspec_get_result(void);

    #define describe(description, block) {                      \
        static void(*__before)(void);                           \
        static void(*__after)(void);                            \
        { __f_agn(__before, function() {}); };                  \
        { __f_agn(__after, function() {}); };                   \
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

    #define before(block) {                                     \
        __f_agn(__before, block);                               \
    }                                                           \

    #define after(block) {                                      \
        __f_agn(__after, block);                                \
    }                                                           \

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

    #define CSPEC_RESULT        _cspec_get_result();

#ifdef __cplusplus
    }
#endif

#endif /* CSPEC_H_ */
