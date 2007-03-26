// Copyright 2007 Georgia Institute of Technology. All rights reserved.
// ABSOLUTELY NOT FOR DISTRIBUTION
/**
 * @file common.h
 *
 * Includes all the bare necessities, like types, debugging, and some
 * convenience items.
 *
 * @see compiler.h
 * @see debug.h
 * @see scale.h
 */

#ifndef BASE_COMMON_H
#define BASE_COMMON_H

#ifdef __cplusplus
#include "cc.h"
#endif

#include "base/basic_types.h"
#include "compiler.h"
#include "debug.h"
#include "scale.h"

#include <float.h>
#include <math.h>

EXTERN_C_START

/** A suitable no-op for macro expressions. */
#define NOP ((void)0)

/** NaN value for doubles.  Use isnan to check for this. */
extern const double DBL_NAN;

/** NaN value for floats.  Use isnanf to check for this. */
extern const double FLT_NAN;

/**
 * Currently, equivalent to C's abort().
 *
 * Note that built-in abort() already flushes all streams.
 */
COMPILER_NORETURN
void fl_abort(void);

/**
 * Waits for the user to press return.
 *
 * This function will obliterate anything in the stdin buffer.
 */
void fl_pause(void);

/**
 * Obtains a more concise filename from a full path.
 *
 * If the file is in the FASTlib directories, this will shorten the
 * path to contain only the portion beneath the c directory.
 * Otheerwise, it will give the file name and its containing
 * directory.
 */
const char *fl_filename(const char* name);

/**
 * Prints a message header for a specified location in code.
 */
void fl_msg_header(char type, const char *file, const char *func, int line);

/** Whether to treat nonfatal warnings as fatal. */
extern int abort_on_nonfatal;

/** Whether to wait for user input after nonfatal warnings. */
extern int pause_on_nonfatal;

/** Whether to print call locations for notifications. */
extern int print_notify_headers;

/** Implementation for FATAL - use FATAL instead. */
COMPILER_NORETURN
COMPILER_PRINTF(4, 5)
void fatal(const char *file, const char *func, int line,
	   const char* format, ...);

/** Implementation for NONFATAL - use NONFATAL instead. */
COMPILER_PRINTF(4, 5)
void nonfatal(const char *file, const char *func, int line,
	      const char* format, ...);

/** Implementation for NOTIFY - use NOTIFY instead. */
COMPILER_PRINTF(4, 5)
void notify(const char *file, const char *func, int line,
	    const char* format, ...);

/**
 * Aborts, printing call location and a message.
 *
 * Message is sent to stderr.  Arguments are equivalent to printf.
 */
#define FATAL(msg_params...) \
    (fatal(__FILE__, __func__, __LINE__, msg_params))

/**
 * (Possibly) aborts or pauses, printing call location and a message.
 *
 * Message is sent to stderr.  Arguments are equivalent to printf.
 */
#define NONFATAL(msg_params...) \
    (nonfatal(__FILE__, __func__, __LINE__, msg_params))

/**
 * Prints (possibly) call location and a message.
 *
 * Message is sent to stderr.  Arguments are equivalent to printf.
 */
#define NOTIFY(msg_params...) \
    (notify(__FILE__, __func__, __LINE__, msg_params))

/** The maximum number of tsprintf buffers available at any time. */
#define TSPRINTF_COUNT 16

/** The maximum length of a tsprintf string. */
#define TSPRINTF_LENGTH 512

/**
 * DEPRECATED - NOT SAFE.
 *
 * Formats a string and returns an ultra-convenient string which you
 * do not have to free, and can use for a limited time.  If you simply
 * pass this to another function you will be safe.
 *
 * This works by maintaining a pool of TSPRINTF_COUNT buffers which
 * are reused circularly.
 *
 * If the string is longer than TSPRINTF_LENGTH, it will be truncated.
 */
COMPILER_PRINTF(1, 2)
char *tsprintf(const char *format, ...);

/**
 * Standard return value for indicating success or failure.
 *
 * It is recommended to use these consistently, as they have a fixed meaning.
 * Unfortunately, many functions indicate failure with nonzero return values,
 * whereas some functions indicate success with nonzero return values.
 */
typedef enum {
  /**
   * Return value for indicating successful operation.
   */
  SUCCESS_PASS = 20,
  /**
   * Return value indicating an operation may have been successful, but
   * there are things that a careful programmer or user should be wary of.
   */
  SUCCESS_WARN = 15,
  /**
   * Return value indicating an operation failed.
   */
  SUCCESS_FAIL = 10
} success_t;

/**
 * Asserts that a particular operation passes, otherwise terminates
 * the program.
 *
 * This check will *always* occur, regardless of debug mode.  It is suitable
 * for handling functions that return error codes, where it is not worth
 * trying to recover.
 */
#define ASSERT_PASS(x) (likely((x) >= SUCCESS_PASS) ? NOP \
        : FATAL("ASSERT_PASS failed: " #x))

/**
 * Turns return values from most C standard library functions into a
 * standard success_t value, under the assumption that negative is bad.
 *
 * If x is less than 0, failure is assumed; otherwise, success is assumed.
 */
#define SUCCESS_FROM_INT(x) (unlikely((x) < 0) ? SUCCESS_FAIL : SUCCESS_PASS)

/**
 * Returns true if something passed, false if it failed or incurred warnings.
 *
 * Branch-predictor-optimized for passing case.
 */
#define PASSED(x) (likely((x) >= SUCCESS_PASS))

/**
 * Returns true if something failed, false if it passed or incurred warnings.
 *
 * Branch-predictor-optimized for non-failing case.
 */
#define FAILED(x) (unlikely((x) <= SUCCESS_FAIL))

EXTERN_C_END

#endif
