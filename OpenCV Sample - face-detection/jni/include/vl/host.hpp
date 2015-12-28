/** @file host.h
 ** @brief Host
 ** @author Andrea Vedaldi
 ** @sa @ref portability
 **/

/*
Copyright (C) 2007-12 Andrea Vedaldi and Brian Fulkerson.
All rights reserved.

This file is part of the VLFeat library and is made available under
the terms of the BSD license (see the COPYING file).
*/

#ifndef VL_HOST_H
#define VL_HOST_H

/** ------------------------------------------------------------------
 ** @name Configuration options
 ** @{ */

/** ------------------------------------------------------------------
 ** @name Defining functions
 ** @{ */

#define VL_EXPORT
#define VL_INLINE

/** @} */

/** ------------------------------------------------------------------
 ** @name C preprocessor helper macros
 ** @{ */

/** @brief Convert the argument to a string
 ** @param x value to be stringified.
 **
 ** This macro stringifies the argument @a x by means of the
 ** <code>#</code> prerpocessor operator.
 **
 ** The standard C preprocessor does not prescan arguments which are
 ** stringified, so
 **
 ** @code
 ** #define A B
 ** char const * str = VL_STRINGIFY(A) ;
 ** @endcode
 **
 ** initializes <code>str</code> with a pointer to the string
 ** <code>"A"</code>, which mihgt be unexpected. To fix this issue,
 ** you can use ::VL_XSTRINGIFY.
 **
 ** @sa ::VL_XSTRINGIFY
 **/

#define VL_STRINGIFY(x) # x

/** @brief Expand and then convert the argument to a string
 ** @param x value to be macro-expanded and converted.
 **
 ** This macro macro-expands the argument @a x and stringifies the
 ** result of the expansion. For instance
 **
 ** @code
 ** #define A B
 ** char const * str = VL_STRINGIFY(A) ;
 ** @endcode
 **
 ** initializes <code>str</code> with a pointer to the string
 ** <code>"B"</code>.
 **
 ** @sa ::VL_STRINGIFY
 **/

#define VL_XSTRINGIFY(x) VL_STRINGIFY(x)

/** @brief Concatenate two arguments into a lexical unit
 ** @param x first argument to be concatenated.
 ** @param y second argument to be concatenated.
 **
 ** This macro concatenates its arguments into a single lexical unit
 ** by means of the <code>##</code> preprocessor operator. Notice that
 ** arguments concatenated by <code>##</code> are not pre-expanded by
 ** the C preprocessor. To macro-expand the arguments and then
 ** concatenate them,use ::VL_XCAT.
 **
 ** @see ::VL_XCAT
 **/

#define VL_CAT(x,y) x ## y

/** @brief Expand and then concatenate two arguments into a lexical unit
 ** @param x first argument to be concatenated.
 ** @param y second argument to be concatenated.
 **
 ** This macro is the same as ::VL_CAT, except that the arguments are
 ** macro expanded before being concatenated.
 **
 ** @see ::VL_CAT
 **/

#define VL_XCAT(x,y) VL_CAT(x,y)

/** @brief Expand and then concatenate three arguments into a lexical unit
 ** @param x first argument to be concatenated.
 ** @param y second argument to be concatenated.
 ** @param z third argument to be concatenated.
 **
 ** This macro is the same as ::VL_XCAT, except that it has three arguments.
 **
 ** @see ::VL_XCAT
 **/

#define VL_XCAT3(x,y,z) VL_XCAT(VL_XCAT(x,y),z)

/** @brief Expand and then concatenate four arguments into a lexical unit
 ** @param x first argument to be concatenated.
 ** @param y second argument to be concatenated.
 ** @param z third argument to be concatenated.
 ** @param u fourth argument to be concatenated.
 **
 ** This macro is the same as ::VL_XCAT, except that it has four arguments.
 **
 ** @see ::VL_XCAT
 **/

#define VL_XCAT4(x,y,z,u) VL_XCAT(VL_XCAT3(x,y,z),u)

/** @brief Expand and then concatenate five arguments into a lexical unit
 ** @param x first argument to be concatenated.
 ** @param y second argument to be concatenated.
 ** @param z third argument to be concatenated.
 ** @param u fourth argument to be concatenated.
 ** @param v fifth argument to be concatenated.
 **
 ** This macro is the same as ::VL_XCAT, except that it has five arguments.
 **
 ** @see ::VL_XCAT
 **/

#define VL_XCAT5(x,y,z,u,v) VL_XCAT(VL_XCAT4(x,y,z,u),v)

/** @brief Convert a boolean to "yes" or "no" strings
 ** @param x boolean to convert.
 **
 ** A pointer to either the string "yes" (if @a x is true)
 ** or the string "no".
 **
 ** @par Example
 ** @code
 ** VL_PRINTF("Is x true? %s.", VL_YESNO(x))
 ** @endcode
 **/

#define VL_YESNO(x) ((x)?"yes":"no")

/** @} */

/*
 The following macros identify the host OS, architecture and compiler.
 They are derived from http://predef.sourceforge.net/
 */

/** @name Identifying the host operating system
 ** @{ */
#define VL_OS_LINUX 1

#define VL_COMPILER_LP64

/** @name Identifying the host endianness
 ** @{ */
#if defined(__LITTLE_ENDIAN__) || \
    defined(VL_ARCH_IX86)      || \
    defined(VL_ARCH_IA64)      || \
    defined(VL_ARCH_X64)       || \
    defined(__DOXYGEN__)
#define VL_ARCH_LITTLE_ENDIAN
#endif

#if  defined(__DOXYGEN__) || \
    !defined(VL_ARCH_LITTLE_ENDIAN)
#define VL_ARCH_BIG_ENDIAN
#endif
/** @} */

VL_EXPORT char * vl_static_configuration_to_string_copy () ;

/** ------------------------------------------------------------------
 ** @name Atomic data types
 ** @{
 **/

#define VL_TRUE 1   /**< @brief @c true (1) constant */
#define VL_FALSE 0  /**< @brief @c false (0) constant */

typedef long long           vl_int64 ;   /**< @brief Signed 64-bit integer. */
typedef int                 vl_int32 ;   /**< @brief Signed 32-bit integer. */
typedef short               vl_int16 ;   /**< @brief Signed 16-bit integer. */
typedef char                vl_int8  ;   /**< @brief Signed  8-bit integer. */

typedef long long unsigned  vl_uint64 ;  /**< @brief Unsigned 64-bit integer. */
typedef int       unsigned  vl_uint32 ;  /**< @brief Unsigned 32-bit integer. */
typedef short     unsigned  vl_uint16 ;  /**< @brief Unsigned 16-bit integer. */
typedef char      unsigned  vl_uint8 ;   /**< @brief Unsigned  8-bit integer. */

typedef int                 vl_int ;     /**< @brief Same as @c int. */
typedef unsigned int        vl_uint ;    /**< @brief Same as <code>unsigned int</code>. */

typedef int                 vl_bool ;    /**< @brief Boolean. */
typedef long long           vl_intptr ;  /**< @brief Integer holding a pointer. */
typedef long long unsigned  vl_uintptr ; /**< @brief Unsigned integer holding a pointer. */
typedef long long unsigned  vl_size ;    /**< @brief Unsigned integer holding the size of a memory block. */
typedef long long           vl_index ;   /**< @brief Signed version of ::vl_size and ::vl_uindex */
typedef long long unsigned  vl_uindex ;  /**< @brief Same as ::vl_size */


#define VL_INT8_C(x) x
#define VL_INT16_C(x) x
#define VL_INT32_C(x) x
#define VL_INT64_C(x) x ## L

#define VL_UINT8_C(x) x
#define VL_UINT16_C(x) x
#define VL_UINT32_C(x) x ## U
#define VL_UINT64_C(x) x ## UL

/** ------------------------------------------------------------------
 ** @name Printing the atomic data types
 ** @{ */

/* Lengths only: */

/** @def VL_FL_INT64
 ** @brief @c printf length flag for ::vl_int64 and ::vl_uint64.
 **/

/** @def VL_FL_INT32
 ** @brief @c printf length flag for ::vl_int32 and ::vl_uint32.
 **/

/** @def VL_FL_INT16
 ** @brief @c printf length flag for ::vl_int16 and ::vl_uint16.
 **/

/** @def VL_FL_INT8
 ** @brief @c printf length flag for ::vl_int8 and ::vl_uint8.
 **/

/** @def VL_FL_INDEX
 ** @brief @c printf length flag for ::vl_index and ::vl_uindex
 **/

#define VL_FL_INT64  "ll"
#define VL_FL_INT32  ""
#define VL_FL_INT16  "h"
#define VL_FL_INT8   "hh"

#define VL_FL_INDEX VL_FL_INT64

/* Formats (but not conversions!): */

/** @def VL_FMT_SIZE
 ** @brief @c printf flag for ::vl_size
 **/

/** @def VL_FMT_INDEX
 ** @brief @c printf flag for ::vl_index
 **/

/** @def VL_FMT_UINDEX
 ** @brief @c printf flag for ::vl_uindex
 **/

/** @def VL_FMT_INTPTR
 ** @brief @c printf flag for ::vl_intptr
 **/

/** @def VL_FMT_UINTPTR
 ** @brief @c printf flag for ::vl_uintptr
 **/

#define VL_FMT_INDEX   VL_FL_INDEX "d"
#define VL_FMT_INTPTR  VL_FMT_INDEX
#define VL_FMT_UINDEX  VL_FL_INDEX "u"
#define VL_FMT_SIZE    VL_FMT_UINDEX
#define VL_FMT_UINTPTR VL_FMT_UINDEX

/* VL_HOST_H */
#endif
