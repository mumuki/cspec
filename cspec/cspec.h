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
    void _cspec_should_power(const char* filename, int line, void* actual, int(*f)(void*, void*), void* expected);

    int _cspec_should_is_equal(void* actual, void* expected);
    int _cspec_should_not_equal(void* actual, void* expected);
    int _cspec_should_string_equal(void* actual, void* expected);

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

    #define should(actual)  _cspec_should_power(__FILE__, __LINE__, (void*)(actual),
    #define be              _cspec_should_is_equal, (void*)
    #define not_be          _cspec_should_not_equal, (void*)
    #define be_string       _cspec_should_string_equal, (void*)

    #define equal(expected) (expected))
    #define null            (NULL))

    #define truthy          (1))
    #define falsey          (0))

    #define CSPEC_RESULT    _cspec_get_result();

#ifdef __cplusplus
    }
#endif

#endif /* CSPEC_H_ */
