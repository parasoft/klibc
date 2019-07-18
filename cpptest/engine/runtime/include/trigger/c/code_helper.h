/*
 * (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
 * The use of this source code is governed by the general Parasoft License Agreement.
 * Redistribution strictly prohibited.
 */

#ifndef __trigger_c_code_helper_h__
#define __trigger_c_code_helper_h__

#include <trigger/c/code.h>
#include <trigger/c/manager.h>

TRIGGER_EXTERN_C_START


struct tgr_code_helper
{
  tgr_code_helper_ptr (TRIGGER_CDECL *Assert)(const char* message);

  tgr_code_helper_ptr (TRIGGER_CDECL *Fail)(const char* message);
  
  tgr_code_helper_ptr (TRIGGER_CDECL *Report)(const char* message);

  tgr_code_helper_ptr (TRIGGER_CDECL *Arg)(const char *argName);

  tgr_code_helper_ptr (TRIGGER_CDECL *ArgByExpr)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Field)(const char *field_name);

  tgr_code_helper_ptr (TRIGGER_CDECL *FieldByExpr)(void);
  
  tgr_code_helper_ptr (TRIGGER_CDECL *If)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Else)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Assign)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Or)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *And)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Equal)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *NotEqual)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Greater)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *GreaterOrEqual)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Less)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *LessOrEqual)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Multiply)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Divide)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Plus)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Minus)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Not)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *At)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Inc)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Dec)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *LP)(void);
  
  tgr_code_helper_ptr (TRIGGER_CDECL *RP)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *RunId)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *Execute)(tgr_external_func_ptr function);

  tgr_code_helper_ptr (TRIGGER_CDECL *WithArg)(const char* name);
  
  tgr_code_helper_ptr (TRIGGER_CDECL *Return)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *LB)(void);

  tgr_code_helper_ptr (TRIGGER_CDECL *RB)(void);
  
  tgr_code_helper_ptr (TRIGGER_CDECL *FileLine)(const char* file, unsigned line);
  
  tgr_code_helper_ptr (TRIGGER_CDECL *Line)(unsigned line);

  tgr_code_helper_ptr (TRIGGER_CDECL *Value)(tgr_value value);

  tgr_code_helper_ptr (TRIGGER_CDECL *Char)(char c);

  tgr_code_helper_ptr (TRIGGER_CDECL *SChar)(signed char sc);
 
  tgr_code_helper_ptr (TRIGGER_CDECL *UChar)(unsigned char uc);
  
  tgr_code_helper_ptr (TRIGGER_CDECL *WChar)(TRIGGER_WCHAR_T w);

  tgr_code_helper_ptr (TRIGGER_CDECL *Short)(short s);

  tgr_code_helper_ptr (TRIGGER_CDECL *UShort)(unsigned short us);

  tgr_code_helper_ptr (TRIGGER_CDECL *Int)(int i);

  tgr_code_helper_ptr (TRIGGER_CDECL *UInt)(unsigned int ui);

  tgr_code_helper_ptr (TRIGGER_CDECL *Long)(long l);

  tgr_code_helper_ptr (TRIGGER_CDECL *ULong)(unsigned long ul);

  tgr_code_helper_ptr (TRIGGER_CDECL *LLong)(TRIGGER_INTEGER ll);

  tgr_code_helper_ptr (TRIGGER_CDECL *ULLong)(TRIGGER_UINTEGER ull);

  tgr_code_helper_ptr (TRIGGER_CDECL *Float)(float f);

  tgr_code_helper_ptr (TRIGGER_CDECL *Double)(double d);

  tgr_code_helper_ptr (TRIGGER_CDECL *LDouble)(TRIGGER_FLOATING ld);
  
  tgr_code_helper_ptr (TRIGGER_CDECL *Func)(tgr_func_ptr func);
  
  tgr_code_helper_ptr (TRIGGER_CDECL *VoidPtr)(const volatile void* ptr);

  tgr_code_helper_ptr (TRIGGER_CDECL *CharPtr)(const volatile char* ptr);

  tgr_code_helper_ptr (TRIGGER_CDECL *SCharPtr)(const volatile signed char* ptr);
 
  tgr_code_helper_ptr (TRIGGER_CDECL *UCharPtr)(const volatile unsigned char* ptr);
  
  tgr_code_helper_ptr (TRIGGER_CDECL *WCharPtr)(const volatile TRIGGER_WCHAR_T* ptr);

  tgr_code_helper_ptr (TRIGGER_CDECL *ShortPtr)(const volatile short* ptr);

  tgr_code_helper_ptr (TRIGGER_CDECL *UShortPtr)(const volatile unsigned short* ptr);

  tgr_code_helper_ptr (TRIGGER_CDECL *IntPtr)(const volatile int* ptr);

  tgr_code_helper_ptr (TRIGGER_CDECL *UIntPtr)(const volatile unsigned int* ptr);

  tgr_code_helper_ptr (TRIGGER_CDECL *LongPtr)(const volatile long* ptr);

  tgr_code_helper_ptr (TRIGGER_CDECL *ULongPtr)(const volatile unsigned long* ptr);

  tgr_code_helper_ptr (TRIGGER_CDECL *LLongPtr)(const volatile TRIGGER_INTEGER* ptr);

  tgr_code_helper_ptr (TRIGGER_CDECL *ULLongPtr)(const volatile TRIGGER_UINTEGER* ptr);

  tgr_code_helper_ptr (TRIGGER_CDECL *FloatPtr)(const volatile float* ptr);

  tgr_code_helper_ptr (TRIGGER_CDECL *DoublePtr)(const volatile double* ptr);

  tgr_code_helper_ptr (TRIGGER_CDECL *LDoublePtr)(const volatile TRIGGER_FLOATING* ptr);

  tgr_code_helper_ptr (TRIGGER_CDECL *CharRef)(const volatile char* ref);

  tgr_code_helper_ptr (TRIGGER_CDECL *SCharRef)(const volatile signed char* ref);
 
  tgr_code_helper_ptr (TRIGGER_CDECL *UCharRef)(const volatile unsigned char* ref);
  
  tgr_code_helper_ptr (TRIGGER_CDECL *WCharRef)(const volatile TRIGGER_WCHAR_T* ref);

  tgr_code_helper_ptr (TRIGGER_CDECL *ShortRef)(const volatile short* ref);

  tgr_code_helper_ptr (TRIGGER_CDECL *UShortRef)(const volatile unsigned short* ref);

  tgr_code_helper_ptr (TRIGGER_CDECL *IntRef)(const volatile int* ref);

  tgr_code_helper_ptr (TRIGGER_CDECL *UIntRef)(const volatile unsigned int* ref);

  tgr_code_helper_ptr (TRIGGER_CDECL *LongRef)(const volatile long* ref);

  tgr_code_helper_ptr (TRIGGER_CDECL *ULongRef)(const volatile unsigned long* ref);

  tgr_code_helper_ptr (TRIGGER_CDECL *LLongRef)(const volatile TRIGGER_INTEGER* ref);

  tgr_code_helper_ptr (TRIGGER_CDECL *ULLongRef)(const volatile TRIGGER_UINTEGER* ref);

  tgr_code_helper_ptr (TRIGGER_CDECL *FloatRef)(const volatile float* ref);

  tgr_code_helper_ptr (TRIGGER_CDECL *DoubleRef)(const volatile double* ref);

  tgr_code_helper_ptr (TRIGGER_CDECL *LDoubleRef)(const volatile TRIGGER_FLOATING* ref);

  tgr_code_helper_ptr (TRIGGER_CDECL *VoidPtrRef)(const volatile void* ref);

  tgr_code_helper_ptr (TRIGGER_CDECL *Object)(const volatile void* obj, tgr_type_ptr class_type);

  tgr_code_helper_ptr (TRIGGER_CDECL *Ptr)(const volatile void* value, tgr_type_ptr element_type);

  tgr_code_helper_ptr (TRIGGER_CDECL *Ref)(const volatile void* value, tgr_type_ptr element_type);

};

TRIGGER_DECL
tgr_code_ptr* TRIGGER_CDECL tgr_get_curr_code(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_assert(const char* message);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_fail(const char* message);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_report(const char* message);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_arg(const char *argName);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_arg_by_expr(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_field(const char *field_name);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_field_by_expr(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_if(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_else(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_assign(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_or(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_and(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_equal(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_not_equal(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_greater(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_greater_or_equal(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_less(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_less_or_equal(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_multiply(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_divide(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_plus(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_minus(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_not(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_at(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_inc(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_dec(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_lp(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_rp(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_run_id(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_execute(tgr_external_func_ptr function);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_with_arg(const char* name);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_return(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_lb(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_rb(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_file_line(const char* file, unsigned line);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_line(unsigned line);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_value(tgr_value value);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_char(char c);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_schar(signed char sc);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_uchar(unsigned char uc);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_wchar_t(TRIGGER_WCHAR_T w);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_short(short s);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ushort(unsigned short us);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_int(int i);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_uint(unsigned int ui);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_long(long l);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ulong(unsigned long ul);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_llong(TRIGGER_INTEGER ll);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ullong(TRIGGER_UINTEGER ull);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_float(float f);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_double(double d);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ldouble(TRIGGER_FLOATING ld);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_func(tgr_func_ptr func);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_void_ptr(const volatile void* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_char_ptr(const volatile char* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_schar_ptr(const volatile signed char* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_uchar_ptr(const volatile unsigned char* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_wchar_t_ptr(const volatile TRIGGER_WCHAR_T* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_short_ptr(const volatile short* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ushort_ptr(const volatile unsigned short* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_int_ptr(const volatile int* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_uint_ptr(const volatile unsigned int* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_long_ptr(const volatile long* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ulong_ptr(const volatile unsigned long* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_llong_ptr(const volatile TRIGGER_INTEGER* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ullong_ptr(const volatile TRIGGER_UINTEGER* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_float_ptr(const volatile float* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_double_ptr(const volatile double* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ldouble_ptr(const volatile TRIGGER_FLOATING* ptr);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_char_ref(const volatile char* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_schar_ref(const volatile signed char* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_uchar_ref(const volatile unsigned char* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_wchar_t_ref(const volatile TRIGGER_WCHAR_T* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_short_ref(const volatile short* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ushort_ref(const volatile unsigned short* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_int_ref(const volatile int* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_uint_ref(const volatile unsigned int* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_long_ref(const volatile long* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ulong_ref(const volatile unsigned long* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_llong_ref(const volatile TRIGGER_INTEGER* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ullong_ref(const volatile TRIGGER_UINTEGER* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_float_ref(const volatile float* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_double_ref(const volatile double* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ldouble_ref(const volatile TRIGGER_FLOATING* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_void_ptr_ref(const volatile void* ref);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_object(const volatile void* obj, 
                                                         tgr_type_ptr class_type);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_pointer(const volatile void* value, 
                                                          tgr_type_ptr element_type);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_reference(const volatile void* value, 
                                                            tgr_type_ptr element_type);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_instance(void);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_create_full(const char* name, 
                                                  tgr_run_mode run_mode, 
                                                  const char* file, 
                                                  unsigned line);

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_create(const char* name, tgr_run_mode run_mode);

#if TRIGGER_IMPLEMENTATION
/**************************************************************************************************/
/*                                       IMPLEMENTATION                                           */
/**************************************************************************************************/

TRIGGER_DECL
tgr_code_ptr* TRIGGER_CDECL tgr_get_curr_code(void)
{
  static tgr_code_ptr curr_code = 0;
  return &curr_code;
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_assert(const char* message)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_assert, 
                                    tgr_char_ptr(message));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_fail(const char* message)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(),
                                    c_tgr_instruction_fail,
                                    tgr_char_ptr(message));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_report(const char* message)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_report, 
                                    tgr_char_ptr(message));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_arg(const char *argName)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_arg, 
                                    tgr_char_ptr(argName));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_arg_by_expr(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_arg);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_field(const char *field_name)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_field, 
                                    tgr_char_ptr(field_name));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_field_by_expr(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_field);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_if(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_if);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_else(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_else);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_assign(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_assign);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_or(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_or);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_and(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_and);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_equal(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_equal);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_not_equal(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_not_equal);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_greater(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_greater);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_greater_or_equal(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_greater_or_equal);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_less(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_less);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_less_or_equal(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_less_or_equal);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_multiply(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_multiply);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_divide(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_divide);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_plus(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_plus);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_minus(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_minus);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_not(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_not);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_at(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_at);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_inc(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_inc);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_dec(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_dec);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_lp(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_left_paren);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_rp(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_right_paren);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_run_id(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_run_id);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_execute(tgr_external_func_ptr function)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_execute, 
                                    Func_c(function));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_with_arg(const char* name)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_with_arg, 
                                    tgr_char_ptr(name));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_return(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_return);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_lb(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_left_brace);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_rb(void)
{
  tgr_code_add_instruction(*tgr_get_curr_code(), c_tgr_instruction_right_brace);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_file_line(const char* file, unsigned line)
{ 
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_line_directive, 
                                    tgr_pos(file, line));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_line(unsigned line)
{
  return tgr_code_helper_file_line(0, line);
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_value(tgr_value value)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    value);
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_char(char c)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_char(c));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_schar(signed char sc)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_schar(sc));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_uchar(unsigned char uc)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_uchar(uc));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_wchar_t(TRIGGER_WCHAR_T w)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_wchar_t(w));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_short(short s)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_short(s));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ushort(unsigned short us)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_ushort(us));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_int(int i)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_int(i));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_uint(unsigned int ui)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_uint(ui));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_long(long l)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_long(l));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ulong(unsigned long ul)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_ulong(ul));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_llong(TRIGGER_INTEGER ll)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_llong(ll));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ullong(TRIGGER_UINTEGER ull)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_ullong(ull));
  return tgr_code_helper_instance();
}

#ifndef __KERNEL__
TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_float(float f)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_float(f));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_double(double d)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_double(d));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ldouble(TRIGGER_FLOATING ld)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_ldouble(ld));
  return tgr_code_helper_instance();
}
#endif

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_func(tgr_func_ptr func)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_func(func));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_void_ptr(const volatile void* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_void_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_char_ptr(const volatile char* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_char_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_schar_ptr(const volatile signed char* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_schar_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_uchar_ptr(const volatile unsigned char* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_uchar_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_wchar_t_ptr(const volatile TRIGGER_WCHAR_T* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_wchar_t_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_short_ptr(const volatile short* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_short_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ushort_ptr(const volatile unsigned short* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_ushort_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_int_ptr(const volatile int* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_int_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_uint_ptr(const volatile unsigned int* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_uint_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_long_ptr(const volatile long* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_long_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ulong_ptr(const volatile unsigned long* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_ulong_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_llong_ptr(const volatile TRIGGER_INTEGER* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_llong_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ullong_ptr(const volatile TRIGGER_UINTEGER* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_ullong_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_float_ptr(const volatile float* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                        c_tgr_instruction_value, 
                                        tgr_float_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_double_ptr(const volatile double* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_double_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ldouble_ptr(const volatile TRIGGER_FLOATING* ptr)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_ldouble_ptr(ptr));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_char_ref(const volatile char* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_char_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_schar_ref(const volatile signed char* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_schar_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_uchar_ref(const volatile unsigned char* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_uchar_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_wchar_t_ref(const volatile TRIGGER_WCHAR_T* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_wchar_t_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_short_ref(const volatile short* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_short_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ushort_ref(const volatile unsigned short* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_ushort_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_int_ref(const volatile int* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_int_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_uint_ref(const volatile unsigned int* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_uint_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_long_ref(const volatile long* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_long_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ulong_ref(const volatile unsigned long* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_ulong_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_llong_ref(const volatile TRIGGER_INTEGER* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_llong_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ullong_ref(const volatile TRIGGER_UINTEGER* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_ullong_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_float_ref(const volatile float* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_float_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_double_ref(const volatile double* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_double_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_ldouble_ref(const volatile TRIGGER_FLOATING* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_ldouble_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_void_ptr_ref(const volatile void* ref)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_void_ptr_ref(ref));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_object(const volatile void* obj, 
                                                         tgr_type_ptr class_type)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_object(obj, class_type));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_pointer(const volatile void* value, 
                                                          tgr_type_ptr element_type)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_object(value, tgr_ptr_type(element_type)));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_reference(const volatile void* value, 
                                                            tgr_type_ptr element_type)
{
  tgr_code_add_instruction_with_arg(*tgr_get_curr_code(), 
                                    c_tgr_instruction_value, 
                                    tgr_object(value, tgr_ref_type(element_type)));
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_code_helper_instance(void)
{
  static struct tgr_code_helper code_helper = 
  {
    tgr_code_helper_assert,
    tgr_code_helper_fail,
    tgr_code_helper_report,
    tgr_code_helper_arg,
    tgr_code_helper_arg_by_expr,
    tgr_code_helper_field,
    tgr_code_helper_field_by_expr,
    tgr_code_helper_if,
    tgr_code_helper_else,
    tgr_code_helper_assign,
    tgr_code_helper_or,
    tgr_code_helper_and,
    tgr_code_helper_equal,
    tgr_code_helper_not_equal,
    tgr_code_helper_greater,
    tgr_code_helper_greater_or_equal,
    tgr_code_helper_less,
    tgr_code_helper_less_or_equal,
    tgr_code_helper_multiply,
    tgr_code_helper_divide,
    tgr_code_helper_plus,
    tgr_code_helper_minus,
    tgr_code_helper_not,
    tgr_code_helper_at,
    tgr_code_helper_inc,
    tgr_code_helper_dec,
    tgr_code_helper_lp,
    tgr_code_helper_rp,
    tgr_code_helper_run_id,
    tgr_code_helper_execute,
    tgr_code_helper_with_arg,
    tgr_code_helper_return,
    tgr_code_helper_lb,
    tgr_code_helper_rb,
    tgr_code_helper_file_line,
    tgr_code_helper_line,
    tgr_code_helper_value,
    tgr_code_helper_char,
    tgr_code_helper_schar,
    tgr_code_helper_uchar,
    tgr_code_helper_wchar_t,
    tgr_code_helper_short,
    tgr_code_helper_ushort,
    tgr_code_helper_int,
    tgr_code_helper_uint,
    tgr_code_helper_long,
    tgr_code_helper_ulong,
    tgr_code_helper_llong,
    tgr_code_helper_ullong,
    tgr_code_helper_float,
    tgr_code_helper_double,
    tgr_code_helper_ldouble,
    tgr_code_helper_func,
    tgr_code_helper_void_ptr,
    tgr_code_helper_char_ptr,
    tgr_code_helper_schar_ptr,
    tgr_code_helper_uchar_ptr,
    tgr_code_helper_wchar_t_ptr,
    tgr_code_helper_short_ptr,
    tgr_code_helper_ushort_ptr,
    tgr_code_helper_int_ptr,
    tgr_code_helper_uint_ptr,
    tgr_code_helper_long_ptr,
    tgr_code_helper_ulong_ptr,
    tgr_code_helper_llong_ptr,
    tgr_code_helper_ullong_ptr,
    tgr_code_helper_float_ptr,
    tgr_code_helper_double_ptr,
    tgr_code_helper_ldouble_ptr,
    tgr_code_helper_char_ref,
    tgr_code_helper_schar_ref,
    tgr_code_helper_uchar_ref,
    tgr_code_helper_wchar_t_ref,
    tgr_code_helper_short_ref,
    tgr_code_helper_ushort_ref,
    tgr_code_helper_int_ref,
    tgr_code_helper_uint_ref,
    tgr_code_helper_long_ref,
    tgr_code_helper_ulong_ref,
    tgr_code_helper_llong_ref,
    tgr_code_helper_ullong_ref,
    tgr_code_helper_float_ref,
    tgr_code_helper_double_ref,
    tgr_code_helper_ldouble_ref,
    tgr_code_helper_void_ptr_ref,
    tgr_code_helper_object,
    tgr_code_helper_pointer,
    tgr_code_helper_reference
  };
  return &code_helper;
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_create_full(const char* name, 
                                                  tgr_run_mode run_mode, 
                                                  const char* file, 
                                                  unsigned line)
{
  tgr_trigger_ptr trigger = tgr_add(name, run_mode);
  *tgr_get_curr_code() = &trigger->code;
  if (file) {
    tgr_code_helper_instance()->FileLine(file, line);
  }
  return tgr_code_helper_instance();
}

TRIGGER_DECL
tgr_code_helper_ptr TRIGGER_CDECL tgr_create(const char* name, tgr_run_mode run_mode)
{
  return tgr_create_full(name, run_mode, 0, 0);
}

/**************************************************************************************************/
#endif /* TRIGGER_IMPLEMENTATION */

TRIGGER_EXTERN_C_END

#endif /* __trigger_c_code_helper_h__ */

