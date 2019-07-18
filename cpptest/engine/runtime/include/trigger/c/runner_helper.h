/*
 * (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
 * The use of this source code is governed by the general Parasoft License Agreement.
 * Redistribution strictly prohibited.
 */


#ifndef __trigger_c_runner_helper_h__
#define __trigger_c_runner_helper_h__

#include <trigger/c/runner.h>

TRIGGER_EXTERN_C_START


struct tgr_runner_helper
{
  tgr_runner_helper_ptr (TRIGGER_CDECL *WithValueArg)(const char* name, tgr_value value);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithCharArg)(const char* name, char c);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithSCharArg)(const char* name, signed char sc);
 
  tgr_runner_helper_ptr (TRIGGER_CDECL *WithUCharArg)(const char* name, unsigned char uc);
  
  tgr_runner_helper_ptr (TRIGGER_CDECL *WithWCharArg)(const char* name, TRIGGER_WCHAR_T w);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithShortArg)(const char* name, short s);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithUShortArg)(const char* name, unsigned short us);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithIntArg)(const char* name, int i);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithUIntArg)(const char* name, unsigned int ui);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithLongArg)(const char* name, long l);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithULongArg)(const char* name, unsigned long ul);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithLLongArg)(const char* name, TRIGGER_INTEGER ll);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithULLongArg)(const char* name, TRIGGER_UINTEGER ull);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithFloatArg)(const char* name, float f);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithDoubleArg)(const char* name, double d);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithLDoubleArg)(const char* name, TRIGGER_FLOATING ld);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithFuncArg)(const char* name, tgr_func_ptr func);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithObjectArg)(const char* name, 
                                                       const volatile void* obj, 
                                                       tgr_type_ptr type);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithVoidPtrArg)(const char* name, const volatile void* ptr);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithCharPtrArg)(const char* name, const volatile char* ptr);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithSCharPtrArg)(const char* name, 
                                                         const volatile signed char* ptr);
 
  tgr_runner_helper_ptr (TRIGGER_CDECL *WithUCharPtrArg)(const char* name, 
                                                         const volatile unsigned char* ptr);
  
  tgr_runner_helper_ptr (TRIGGER_CDECL *WithWCharPtrArg)(const char* name, 
                                                         const volatile TRIGGER_WCHAR_T* ptr);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithShortPtrArg)(const char* name, 
                                                         const volatile short* ptr);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithUShortPtrArg)(const char* name, 
                                                          const volatile unsigned short* ptr);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithIntPtrArg)(const char* name, const volatile int* ptr);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithUIntPtrArg)(const char* name, 
                                                        const volatile unsigned int* ptr);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithLongPtrArg)(const char* name, const volatile long* ptr);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithULongPtrArg)(const char* name, 
                                                         const volatile unsigned long* ptr);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithLLongPtrArg)(const char* name, 
                                                         const volatile TRIGGER_INTEGER* ptr);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithULLongPtrArg)(const char* name, 
                                                          const volatile TRIGGER_UINTEGER* ptr);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithFloatPtrArg)(const char* name, 
                                                         const volatile float* ptr);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithDoublePtrArg)(const char* name, 
                                                          const volatile double* ptr);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithLDoublePtrArg)(const char* name, 
                                                           const volatile TRIGGER_FLOATING* ptr);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithPtrArg)(const char* name, 
                                                    const volatile void* ptr,
                                                    tgr_type_ptr element_type);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithCharRefArg)(const char* name, const volatile char* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithSCharRefArg)(const char* name, 
                                                         const volatile signed char* ref);
 
  tgr_runner_helper_ptr (TRIGGER_CDECL *WithUCharRefArg)(const char* name, 
                                                         const volatile unsigned char* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithWCharRefArg)(const char* name, 
                                                         const volatile TRIGGER_WCHAR_T* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithShortRefArg)(const char* name, 
                                                         const volatile short* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithUShortRefArg)(const char* name, 
                                                          const volatile unsigned short* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithIntRefArg)(const char* name, const volatile int* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithUIntRefArg)(const char* name, 
                                                        const volatile unsigned int* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithLongRefArg)(const char* name, const volatile long* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithULongRefArg)(const char* name, 
                                                         const volatile unsigned long* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithLLongRefArg)(const char* name, 
                                                         const volatile TRIGGER_INTEGER* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithULLongRefArg)(const char* name, 
                                                          const volatile TRIGGER_UINTEGER* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithFloatRefArg)(const char* name, 
                                                         const volatile float* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithDoubleRefArg)(const char* name, 
                                                          const volatile double* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithLDoubleRefArg)(const char* name, 
                                                           const volatile TRIGGER_FLOATING* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithVoidPtrRefArg)(const char* name, 
                                                           const volatile void* ref);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithPtrRefArg)(const char* name, 
                                                       const volatile void* ptr,
                                                       tgr_type_ptr element_type);

  tgr_runner_helper_ptr (TRIGGER_CDECL *WithRefArg)(const char* name, 
                                                    const volatile void* ptr,
                                                    tgr_type_ptr element_type);

  tgr_value (TRIGGER_CDECL *End)();
};

TRIGGER_DECL
tgr_runner_ptr* TRIGGER_CDECL tgr_get_curr_runner(void);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_value_arg(const char* name, 
                                                                     tgr_value arg);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_char_arg(const char* name, char c);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_schar_arg(const char* name, 
                                                                     signed char sc);
TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_uchar_arg(const char* name, 
                                                                     unsigned char uc);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_wchar_t_arg(const char* name, 
                                                                       TRIGGER_WCHAR_T w);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_short_arg(const char* name, short s);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ushort_arg(const char* name, 
                                                                      unsigned short us);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_int_arg(const char* name, int i);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_uint_arg(const char* name, 
                                                                    unsigned int ui);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_long_arg(const char* name, long l);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ulong_arg(const char* name, 
                                                                     unsigned long ul);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_llong_arg(const char* name, 
                                                                     TRIGGER_INTEGER ll);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ullong_arg(const char* name, 
                                                                      TRIGGER_UINTEGER ull);
#ifndef __KERNEL__
TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_float_arg(const char* name, float f);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_double_arg(const char* name, double d);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ldouble_arg(const char* name, 
                                                                       TRIGGER_FLOATING ld);
#endif

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_func_arg(const char* name, 
                                                                    tgr_func_ptr func);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_object_arg(const char* name, 
                                                                      const volatile void* obj, 
                                                                      tgr_type_ptr class_type);
TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_void_ptr_arg(const char* name, 
                                                                        const volatile void* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_char_ptr_arg(const char* name, 
                                                                        const volatile char* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_schar_ptr_arg(
                                                                  const char* name, 
                                                                  const volatile signed char* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_uchar_ptr_arg(
                                                                const char* name, 
                                                                const volatile unsigned char* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_wchar_t_ptr_arg(
                                                              const char* name, 
                                                              const volatile TRIGGER_WCHAR_T* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_short_ptr_arg(const char* name, 
                                                                         const volatile short* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ushort_ptr_arg(
                                                                const char* name, 
                                                                const volatile unsigned short* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_int_ptr_arg(const char* name, 
                                                                       const volatile int* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_uint_ptr_arg(
                                                                  const char* name, 
                                                                  const volatile unsigned int* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_long_ptr_arg(const char* name, 
                                                                        const volatile long* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ulong_ptr_arg(
                                                                const char* name, 
                                                                const volatile unsigned long* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_llong_ptr_arg(
                                                              const char* name, 
                                                              const volatile TRIGGER_INTEGER* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ullong_ptr_arg(
                                                            const char* name, 
                                                            const volatile TRIGGER_UINTEGER* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_float_ptr_arg(const char* name, 
                                                                         const volatile float* ptr);

#ifndef __KERNEL__
TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_double_ptr_arg(
                                                                        const char* name, 
                                                                        const volatile double* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ldouble_ptr_arg(
                                                              const char* name, 
                                                              const volatile TRIGGER_FLOATING* ptr);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ptr_arg(const char* name, 
                                                                   const volatile void* ptr,
                                                                   tgr_type_ptr element_type);
#endif

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_char_ref_arg(const char* name, 
                                                                        const volatile char* ref);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_schar_ref_arg(
                                                                  const char* name, 
                                                                  const volatile signed char* ref);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_uchar_ref_arg(
                                                                const char* name, 
                                                                const volatile unsigned char* ref);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_wchar_t_ref_arg(
                                                              const char* name, 
                                                              const volatile TRIGGER_WCHAR_T* ref);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_short_ref_arg(const char* name, 
                                                                         const volatile short* ref);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ushort_ref_arg(
                                                                const char* name, 
                                                                const volatile unsigned short* ref);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_int_ref_arg(const char* name, 
                                                                       const volatile int* ref);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_uint_ref_arg(
                                                                  const char* name, 
                                                                  const volatile unsigned int* ref);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_long_ref_arg(const char* name, 
                                                                        const volatile long* ref);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ulong_ref_arg(
                                                                const char* name, 
                                                                const volatile unsigned long* ref);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_llong_ref_arg(
                                                              const char* name, 
                                                              const volatile TRIGGER_INTEGER* ref);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ullong_ref_arg(
                                                              const char* name, 
                                                              const volatile TRIGGER_UINTEGER* ref);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_float_ref_arg(const char* name, 
                                                                         const volatile float* ref);

#ifndef __KERNEL__
TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_double_ref_arg(
                                                                        const char* name, 
                                                                        const volatile double* ref);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ldouble_ref_arg(
                                                              const char* name,
                                                              const volatile TRIGGER_FLOATING* ref);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_void_ptr_ref_arg(
                                                                          const char* name, 
                                                                          const volatile void* ref);
#endif

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ptr_ref_arg(const char* name, 
                                                                       const volatile void* ref, 
                                                                       tgr_type_ptr element_type);


TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ref_arg(const char* name, 
                                                                   const volatile void* ref, 
                                                                   tgr_type_ptr element_type);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_runner_helper_end(void);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_instance(void);

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_run(const char* name, tgr_run_mode run_mode);

#if TRIGGER_IMPLEMENTATION
/**************************************************************************************************/
/*                                       IMPLEMENTATION                                           */
/**************************************************************************************************/

TRIGGER_DECL
tgr_runner_ptr* TRIGGER_CDECL tgr_get_curr_runner(void)
{
  static tgr_runner_ptr runner = 0;
  return &runner;
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_value_arg(const char* name, 
                                                                     tgr_value arg)
{
  tgr_runner_ptr runner = *tgr_get_curr_runner();
  if (runner) {
    tgr_runner_add_arg(runner, name, arg);
  }
  return tgr_runner_helper_instance();
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_char_arg(const char* name, char c)
{
  return tgr_runner_helper_with_value_arg(name, tgr_char(c));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_schar_arg(const char* name, 
                                                                     signed char sc)
{
  return tgr_runner_helper_with_value_arg(name, tgr_schar(sc));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_uchar_arg(const char* name, 
                                                                     unsigned char uc)
{
  return tgr_runner_helper_with_value_arg(name, tgr_uchar(uc));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_wchar_t_arg(const char* name, 
                                                                       TRIGGER_WCHAR_T w)
{
  return tgr_runner_helper_with_value_arg(name, tgr_wchar_t(w));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_short_arg(const char* name, short s)
{
  return tgr_runner_helper_with_value_arg(name, tgr_short(s));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ushort_arg(const char* name, 
                                                                      unsigned short us)
{
  return tgr_runner_helper_with_value_arg(name, tgr_ushort(us));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_int_arg(const char* name, int i)
{
  return tgr_runner_helper_with_value_arg(name, tgr_int(i));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_uint_arg(const char* name, 
                                                                    unsigned int ui)
{
  return tgr_runner_helper_with_value_arg(name, tgr_uint(ui));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_long_arg(const char* name, long l)
{
  return tgr_runner_helper_with_value_arg(name, tgr_long(l));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ulong_arg(const char* name, 
                                                                     unsigned long ul)
{
  return tgr_runner_helper_with_value_arg(name, tgr_ulong(ul));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_llong_arg(const char* name, 
                                                                     TRIGGER_INTEGER ll)
{
  return tgr_runner_helper_with_value_arg(name,  tgr_llong(ll));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ullong_arg(const char* name, 
                                                                      TRIGGER_UINTEGER ull)
{
  return tgr_runner_helper_with_value_arg(name, tgr_ullong(ull));
}

#ifndef __KERNEL__
TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_float_arg(const char* name, float f)
{
  return tgr_runner_helper_with_value_arg(name, tgr_float(f));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_double_arg(const char* name, double d)
{
  return tgr_runner_helper_with_value_arg(name,  tgr_double(d));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ldouble_arg(const char* name, 
                                                                       TRIGGER_FLOATING ld)
{
  return tgr_runner_helper_with_value_arg(name, tgr_ldouble(ld));
}
#endif

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_func_arg(const char* name, 
                                                                    tgr_func_ptr func)
{
  return tgr_runner_helper_with_value_arg(name, tgr_func(func));
}


TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_object_arg(const char* name, 
                                                                      const volatile void* obj, 
                                                                      tgr_type_ptr class_type)
{
  tgr_runner_ptr runner = *tgr_get_curr_runner();
  if (runner) {
    tgr_runner_add_arg(runner, name, tgr_object(obj, class_type));
  }
  return tgr_runner_helper_instance();
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_void_ptr_arg(const char* name, 
                                                                        const volatile void* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_void_ptr(ptr));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_char_ptr_arg(const char* name, 
                                                                        const volatile char* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_char_ptr(ptr));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_schar_ptr_arg(
                                                                    const char* name, 
                                                                    const volatile signed char* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_schar_ptr(ptr));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_uchar_ptr_arg(
                                                                  const char* name, 
                                                                  const volatile unsigned char* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_uchar_ptr(ptr));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_wchar_t_ptr_arg(
                                                                const char* name, 
                                                                const volatile TRIGGER_WCHAR_T* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_wchar_t_ptr(ptr));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_short_ptr_arg(const char* name, 
                                                                         const volatile short* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_short_ptr(ptr));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ushort_ptr_arg(
                                                                const char* name, 
                                                                const volatile unsigned short* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_ushort_ptr(ptr));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_int_ptr_arg(const char* name, 
                                                                       const volatile int* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_int_ptr(ptr));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_uint_ptr_arg(
                                                                  const char* name, 
                                                                  const volatile unsigned int* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_uint_ptr(ptr));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_long_ptr_arg(const char* name, 
                                                                        const volatile long* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_long_ptr(ptr));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ulong_ptr_arg(
                                                                  const char* name, 
                                                                  const volatile unsigned long* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_ulong_ptr(ptr));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_llong_ptr_arg(
                                                                const char* name, 
                                                                const volatile TRIGGER_INTEGER* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_llong_ptr(ptr));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ullong_ptr_arg(
                                                              const char* name, 
                                                              const volatile TRIGGER_UINTEGER* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_ullong_ptr(ptr));
}

#ifndef __KERNEL__
TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_float_ptr_arg(const char* name, 
                                                                         const volatile float* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_float_ptr(ptr));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_double_ptr_arg(
                                                                        const char* name, 
                                                                        const volatile double* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_double_ptr(ptr));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ldouble_ptr_arg(
                                                              const char* name, 
                                                              const volatile TRIGGER_FLOATING* ptr)
{
  return tgr_runner_helper_with_value_arg(name, tgr_ldouble_ptr(ptr));
}
#endif

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ptr_arg(const char* name, 
                                                                   const volatile void* ptr,
                                                                   tgr_type_ptr element_type)
{
  return tgr_runner_helper_with_value_arg(name, tgr_object(ptr, tgr_ptr_type(element_type)));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_char_ref_arg(const char* name, 
                                                                        const volatile char* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_char_ref(ref));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_schar_ref_arg(
                                                                    const char* name, 
                                                                    const volatile signed char* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_schar_ref(ref));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_uchar_ref_arg(
                                                                  const char* name, 
                                                                  const volatile unsigned char* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_uchar_ref(ref));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_wchar_t_ref_arg(
                                                                const char* name, 
                                                                const volatile TRIGGER_WCHAR_T* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_wchar_t_ref(ref));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_short_ref_arg(const char* name, 
                                                                         const volatile short* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_short_ref(ref));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ushort_ref_arg(
                                                                const char* name, 
                                                                const volatile unsigned short* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_ushort_ref(ref));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_int_ref_arg(const char* name, 
                                                                       const volatile int* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_int_ref(ref));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_uint_ref_arg(
                                                                  const char* name, 
                                                                  const volatile unsigned int* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_uint_ref(ref));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_long_ref_arg(const char* name, 
                                                                        const volatile long* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_long_ref(ref));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ulong_ref_arg(
                                                                  const char* name, 
                                                                  const volatile unsigned long* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_ulong_ref(ref));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_llong_ref_arg(
                                                                const char* name, 
                                                                const volatile TRIGGER_INTEGER* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_llong_ref(ref));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ullong_ref_arg(
                                                              const char* name, 
                                                              const volatile TRIGGER_UINTEGER* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_ullong_ref(ref));
}

#ifndef __KERNEL__
TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_float_ref_arg(const char* name, 
                                                                         const volatile float* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_float_ref(ref));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_double_ref_arg(
                                                                        const char* name, 
                                                                        const volatile double* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_double_ref(ref));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ldouble_ref_arg(
                                                              const char* name,
                                                              const volatile TRIGGER_FLOATING* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_ldouble_ref(ref));
}
#endif

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_void_ptr_ref_arg(
                                                                          const char* name, 
                                                                          const volatile void* ref)
{
  return tgr_runner_helper_with_value_arg(name, tgr_void_ptr_ref(ref));
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ptr_ref_arg(const char* name, 
                                                                       const volatile void* ref, 
                                                                       tgr_type_ptr element_type)
{
  tgr_runner_ptr runner = *tgr_get_curr_runner();
  if (runner) {
    tgr_runner_add_arg(runner, name, tgr_object(ref, tgr_ref_type(tgr_ptr_type(element_type))));
  }
  return tgr_runner_helper_instance();
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_with_ref_arg(const char* name, 
                                                                   const volatile void* ref, 
                                                                   tgr_type_ptr element_type)
{
  tgr_runner_ptr runner = *tgr_get_curr_runner();
  if (runner) {
    tgr_runner_add_arg(runner, name, tgr_object(ref, tgr_ref_type(element_type)));
  }
  return tgr_runner_helper_instance();
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_runner_helper_end(void)
{
  tgr_runner_ptr runner = *tgr_get_curr_runner();
  if (runner) {
    tgr_runner_ptr prev_runner = runner->prev_runner;
    tgr_value result = tgr_runner_run(runner);

    tgr_runner_destroy(runner);
    *tgr_get_curr_runner() = prev_runner;
    return result;
  } else {
    return tgr_void();
  }
}
TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_runner_helper_instance(void)
{
  static struct tgr_runner_helper runner_helper =
  {
    tgr_runner_helper_with_value_arg,
    tgr_runner_helper_with_char_arg,
    tgr_runner_helper_with_schar_arg,
    tgr_runner_helper_with_uchar_arg,
    tgr_runner_helper_with_wchar_t_arg,
    tgr_runner_helper_with_short_arg,
    tgr_runner_helper_with_ushort_arg,
    tgr_runner_helper_with_int_arg,
    tgr_runner_helper_with_uint_arg,
    tgr_runner_helper_with_long_arg,
    tgr_runner_helper_with_ulong_arg,
    tgr_runner_helper_with_llong_arg,
    tgr_runner_helper_with_ullong_arg,
#ifndef __KERNEL__
    tgr_runner_helper_with_float_arg,
    tgr_runner_helper_with_double_arg,
    tgr_runner_helper_with_ldouble_arg,
#endif
    tgr_runner_helper_with_func_arg,
    tgr_runner_helper_with_object_arg,
    tgr_runner_helper_with_void_ptr_arg,
    tgr_runner_helper_with_char_ptr_arg,
    tgr_runner_helper_with_schar_ptr_arg,
    tgr_runner_helper_with_uchar_ptr_arg,
    tgr_runner_helper_with_wchar_t_ptr_arg,
    tgr_runner_helper_with_short_ptr_arg,
    tgr_runner_helper_with_ushort_ptr_arg,
    tgr_runner_helper_with_int_ptr_arg,
    tgr_runner_helper_with_uint_ptr_arg,
    tgr_runner_helper_with_long_ptr_arg,
    tgr_runner_helper_with_ulong_ptr_arg,
    tgr_runner_helper_with_llong_ptr_arg,
    tgr_runner_helper_with_ullong_ptr_arg,
#ifndef __KERNEL__
    tgr_runner_helper_with_float_ptr_arg,
    tgr_runner_helper_with_double_ptr_arg,
    tgr_runner_helper_with_ldouble_ptr_arg,
#endif
    tgr_runner_helper_with_ptr_arg,
    tgr_runner_helper_with_char_ref_arg,
    tgr_runner_helper_with_schar_ref_arg,
    tgr_runner_helper_with_uchar_ref_arg,
    tgr_runner_helper_with_wchar_t_ref_arg,
    tgr_runner_helper_with_short_ref_arg,
    tgr_runner_helper_with_ushort_ref_arg,
    tgr_runner_helper_with_int_ref_arg,
    tgr_runner_helper_with_uint_ref_arg,
    tgr_runner_helper_with_long_ref_arg,
    tgr_runner_helper_with_ulong_ref_arg,
    tgr_runner_helper_with_llong_ref_arg,
    tgr_runner_helper_with_ullong_ref_arg,
#ifndef __KERNEL__
    tgr_runner_helper_with_float_ref_arg,
    tgr_runner_helper_with_double_ref_arg,
    tgr_runner_helper_with_ldouble_ref_arg,
#endif
    tgr_runner_helper_with_void_ptr_ref_arg,
    tgr_runner_helper_with_ptr_ref_arg,
    tgr_runner_helper_with_ref_arg,
    tgr_runner_helper_end
  };
  return &runner_helper;
}

TRIGGER_DECL
tgr_runner_helper_ptr TRIGGER_CDECL tgr_run(const char* name, tgr_run_mode run_mode)
{
#if TRIGGER_RPORT_IF_MORE_THAN_ONE_C_RUNNER_AT_THE_SAME_TIME
  if (*tgr_get_curr_runner()) {
    tgr_handle_report(name, "More than one c runner at the same time", "runner_helper.h", __LINE__);
  }
#endif /* TRIGGER_RPORT_IF_MORE_THAN_ONE_C_RUNNER_AT_THE_SAME_TIME */
  *tgr_get_curr_runner() = tgr_runner_alloc(name, run_mode, *tgr_get_curr_runner());
  return tgr_runner_helper_instance();
}

/**************************************************************************************************/
#endif /* TRIGGER_IMPLEMENTATION */

TRIGGER_EXTERN_C_END

#endif /* __trigger_c_runner_helper_h__ */
