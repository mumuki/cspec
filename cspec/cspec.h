/*
 * cspec.h
 *
 *      Author: Federico Scarpa
 */

#ifndef CSPEC_H_
#define CSPEC_H_

#ifdef __cplusplus
extern "C" {
	#define __function(body)                           	\
		struct Local {                                  \
			static void body;                       	\
		};                                              \
		Local::
#else
	#define __function(body)                 void body;
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

	#define describe(description, block) {						\
		_cspec_describe_pre(description);                       \
		__function(block) function();                           \
		_cspec_describe_post(description);                      \
	}                                                           \

	#define it(description, block) {                      		\
		_cspec_it_pre(description);                             \
		__function(block) function();                           \
		_cspec_it_post(description);                            \
	}                                                           \

	#define skip(description, block) {                          \
		_cspec_skip(description);                               \
	}                                                           \

	#define before(block) {                                   	\
		_cspec_skip(description);                               \
	}                                                           \

	#define after(block)                 {                      \
		_cspec_skip(description);                               \
	}                                                           \

	#define _should(boolean) {                 					\
		_cspec_should(boolean, __FILE__, __LINE__); 			\
	}                                                           \

	#define should_be_true(bool)		_should((bool))
	#define should_be_false(bool)       _should(!(bool))

	#define should_be_null(ptr)         _should((ptr) == NULL)
	#define should_not_be_null(ptr)     _should((ptr) != NULL)

	#define should_be_equals(expected, actual)  		_should((actual) == (expected))
	#define should_not_be_equals(expected, actual)      _should((actual) != (expected))

	#define should_be_equals_strings(expected, actual)  	_should(strcmp(actual, expected) == 0)
	#define should_not_be_equals_strings(expected, actual)  _should(strcmp(actual, expected) != 0)

	#define CSPEC_RESULT        _cspec_get_result();

#ifdef __cplusplus
}
#endif

#endif /* CSPEC_H_ */
