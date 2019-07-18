/*
 * (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
 * The use of this source code is governed by the general Parasoft License Agreement.
 * Redistribution strictly prohibited.
 */


#ifndef __trigger_c_floating_type_h__
#define __trigger_c_floating_type_h__
#ifdef __KERNEL__
#else

#include <trigger/c/error_type.h>
#include <trigger/c/boolean_type.h>

TRIGGER_EXTERN_C_START

TRIGGER_DECL
TRIGGER_BOOLEAN TRIGGER_CDECL tgr_floating_type_is_floating(tgr_value_ptr value);

TRIGGER_DECL
const char* TRIGGER_CDECL tgr_floating_type_get_string(tgr_value_ptr value);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_floating_type_get_bool_value(tgr_value_ptr value);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_floating_type_is_equal(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_floating_type_is_greater(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_floating_type_is_greater_or_equal(tgr_value_ptr left, 
                                                              tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_floating_type_do_not(tgr_value_ptr operand);

TRIGGER_DECL
TRIGGER_INTEGER TRIGGER_CDECL tgr_float_type_get_integer(tgr_value_ptr value);

TRIGGER_DECL
TRIGGER_BOOLEAN TRIGGER_CDECL tgr_float_type_get_bool(tgr_value_ptr value);

TRIGGER_DECL
TRIGGER_FLOATING TRIGGER_CDECL tgr_float_type_get_floating(tgr_value_ptr value);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_float_type_do_multiply(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_float_type_do_divide(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_float_type_do_plus(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_float_type_do_minus(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_float_type_do_ref_assign(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_float_type_to_rvalue(tgr_value_ptr ref);

TRIGGER_DECL
tgr_type_ptr TRIGGER_CDECL tgr_float_type(void);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_float(float f);

TRIGGER_DECL
TRIGGER_INTEGER TRIGGER_CDECL tgr_double_type_get_integer(tgr_value_ptr value);

TRIGGER_DECL
TRIGGER_BOOLEAN TRIGGER_CDECL tgr_double_type_get_bool(tgr_value_ptr value);

TRIGGER_DECL
TRIGGER_FLOATING TRIGGER_CDECL tgr_double_type_get_floating(tgr_value_ptr value);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_double_type_do_multiply(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_double_type_do_divide(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_double_type_do_plus(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_double_type_do_minus(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_double_type_do_ref_assign(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_double_type_to_rvalue(tgr_value_ptr ref);

TRIGGER_DECL
tgr_type_ptr TRIGGER_CDECL tgr_double_type(void);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_double(double d);

TRIGGER_DECL
TRIGGER_INTEGER TRIGGER_CDECL tgr_ldouble_type_get_integer(tgr_value_ptr value);

TRIGGER_DECL
TRIGGER_BOOLEAN TRIGGER_CDECL tgr_ldouble_type_get_bool(tgr_value_ptr value);

TRIGGER_DECL
TRIGGER_FLOATING TRIGGER_CDECL tgr_ldouble_type_get_floating(tgr_value_ptr value);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_ldouble_type_do_multiply(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_ldouble_type_do_divide(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_ldouble_type_do_plus(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_ldouble_type_do_minus(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_ldouble_type_do_ref_assign(tgr_value_ptr left, tgr_value_ptr right);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_ldouble_type_to_rvalue(tgr_value_ptr ref);

TRIGGER_DECL
tgr_type_ptr TRIGGER_CDECL tgr_ldouble_type(void);

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_ldouble(TRIGGER_FLOATING ld);


#if TRIGGER_IMPLEMENTATION
/**************************************************************************************************/
/*                                       IMPLEMENTATION                                           */
/**************************************************************************************************/

TRIGGER_DECL
TRIGGER_BOOLEAN TRIGGER_CDECL tgr_floating_type_is_floating(tgr_value_ptr value)
{
  return TRIGGER_TRUE;
}

TRIGGER_DECL
const char* TRIGGER_CDECL tgr_floating_type_get_string(tgr_value_ptr value)
{
  return "<floating>";
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_floating_type_get_bool_value(tgr_value_ptr value)
{
  return tgr_boolean(value->type->vtbl->get_bool(value));
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_floating_type_is_equal(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_boolean(tgr_value_get_floating(left) == tgr_value_get_floating(right));
  } else {
    return tgr_is_equal_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_floating_type_is_greater(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_boolean(tgr_value_get_floating(left) > tgr_value_get_floating(right));
  } else {
    return tgr_is_greater_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_floating_type_is_greater_or_equal(tgr_value_ptr left, 
                                                              tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_boolean(tgr_value_get_floating(left) >= tgr_value_get_floating(right));
  } else {
    return tgr_is_greater_or_equal_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_floating_type_do_not(tgr_value_ptr operand)
{
  return tgr_boolean(!operand->type->vtbl->get_bool(operand));
}

TRIGGER_DECL
TRIGGER_INTEGER TRIGGER_CDECL tgr_float_type_get_integer(tgr_value_ptr value)
{
  return (TRIGGER_INTEGER)value->value.f;
}

TRIGGER_DECL
TRIGGER_BOOLEAN TRIGGER_CDECL tgr_float_type_get_bool(tgr_value_ptr value)
{
  return value->value.f != 0;
}

TRIGGER_DECL
TRIGGER_FLOATING TRIGGER_CDECL tgr_float_type_get_floating(tgr_value_ptr value)
{
  return value->value.f;
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_float_type_do_multiply(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_float(left->value.f * (float)(tgr_value_get_floating(right)));
  } else {
    return tgr_do_multiply_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_float_type_do_divide(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_float(left->value.f / (float)(tgr_value_get_floating(right)));
  } else {
    return tgr_do_divide_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_float_type_do_plus(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_float(left->value.f + (float)(tgr_value_get_floating(right)));
  } else {
    return tgr_do_plus_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_float_type_do_minus(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_float(left->value.f - (float)(tgr_value_get_floating(right)));
  } else {
    return tgr_do_minus_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_float_type_do_ref_assign(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    *(float*)(left->value.ptr) = (float)tgr_value_get_floating(right);
    return *left;
  } else {
    return tgr_do_ref_assign_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_float_type_to_rvalue(tgr_value_ptr ref)
{
  return tgr_float(*(float*)ref->value.ptr);
}

TRIGGER_DECL
tgr_type_ptr TRIGGER_CDECL tgr_float_type(void)
{
  static struct tgr_type_vtbl float_type_vtbl =
  {
    tgr_type_is_integer,
    tgr_floating_type_is_floating,
    tgr_type_is_pointer,
    tgr_type_is_func_pointer,
    tgr_type_is_object,
    tgr_type_is_void,
    tgr_type_is_error,
    tgr_float_type_get_integer,
    tgr_float_type_get_bool,
    tgr_float_type_get_floating,
    tgr_type_get_pointer,
    tgr_type_get_func_pointer,
    tgr_floating_type_get_string,
    tgr_floating_type_get_bool_value,
    tgr_floating_type_is_equal,
    tgr_floating_type_is_greater,
    tgr_floating_type_is_greater_or_equal,
    tgr_float_type_do_multiply,
    tgr_float_type_do_divide,
    tgr_float_type_do_plus,
    tgr_float_type_do_minus,
    tgr_do_assign_error,
    tgr_float_type_do_ref_assign,
    tgr_floating_type_do_not,
    tgr_do_inc_error,
    tgr_do_dec_error,
    tgr_get_field_error,
    tgr_do_at_error,
    tgr_float_type_to_rvalue
  };

  static struct tgr_type float_type =
  {
    &float_type_vtbl,
    /*sub_type=*/0,
    /*ptr_type=*/0,
    /*ref_type=*/0,
    /*array_type=*/0,
    /*next_type=*/0,
    /*size=*/(unsigned int)sizeof(float)
  };

  return &float_type;
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_float(float f)
{
  tgr_value value;
  
  value.type = tgr_float_type();
  value.value.f = f;
  return value;
}

TRIGGER_DECL
TRIGGER_INTEGER TRIGGER_CDECL tgr_double_type_get_integer(tgr_value_ptr value)
{
  return (TRIGGER_INTEGER)value->value.d;
}

TRIGGER_DECL
TRIGGER_BOOLEAN TRIGGER_CDECL tgr_double_type_get_bool(tgr_value_ptr value)
{
  return value->value.d != 0;
}

TRIGGER_DECL
TRIGGER_FLOATING TRIGGER_CDECL tgr_double_type_get_floating(tgr_value_ptr value)
{
  return value->value.d;
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_double_type_do_multiply(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_double(left->value.d * (double)(tgr_value_get_floating(right)));
  } else {
    return tgr_do_multiply_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_double_type_do_divide(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_double(left->value.d / (double)(tgr_value_get_floating(right)));
  } else {
    return tgr_do_divide_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_double_type_do_plus(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_double(left->value.d + (double)(tgr_value_get_floating(right)));
  } else {
    return tgr_do_plus_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_double_type_do_minus(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_double(left->value.d - (double)(tgr_value_get_floating(right)));
  } else {
    return tgr_do_minus_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_double_type_do_ref_assign(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    *(double*)(left->value.ptr) = (double)tgr_value_get_floating(right);
    return *left;
  } else {
    return tgr_do_ref_assign_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_double_type_to_rvalue(tgr_value_ptr ref)
{
  return tgr_double(*(double*)ref->value.ptr);
}

TRIGGER_DECL
tgr_type_ptr TRIGGER_CDECL tgr_double_type(void)
{
  static struct tgr_type_vtbl double_type_vtbl =
  {
    tgr_type_is_integer,
    tgr_floating_type_is_floating,
    tgr_type_is_pointer,
    tgr_type_is_func_pointer,
    tgr_type_is_object,
    tgr_type_is_void,
    tgr_type_is_error,
    tgr_double_type_get_integer,
    tgr_double_type_get_bool,
    tgr_double_type_get_floating,
    tgr_type_get_pointer,
    tgr_type_get_func_pointer,
    tgr_floating_type_get_string,
    tgr_floating_type_get_bool_value,
    tgr_floating_type_is_equal,
    tgr_floating_type_is_greater,
    tgr_floating_type_is_greater_or_equal,
    tgr_double_type_do_multiply,
    tgr_double_type_do_divide,
    tgr_double_type_do_plus,
    tgr_double_type_do_minus,
    tgr_do_assign_error,
    tgr_double_type_do_ref_assign,
    tgr_floating_type_do_not,
    tgr_do_inc_error,
    tgr_do_dec_error,
    tgr_get_field_error,
    tgr_do_at_error,
    tgr_double_type_to_rvalue
  };

  static struct tgr_type double_type =
  {
    &double_type_vtbl,
    /*sub_type=*/0,
    /*ptr_type=*/0,
    /*ref_type=*/0,
    /*array_type=*/0,
    /*next_type=*/0,
    /*size=*/(unsigned int)sizeof(double)

  };

  return &double_type;
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_double(double d)
{
  tgr_value value;
  
  value.type = tgr_double_type();
  value.value.d = d;
  return value;
}

TRIGGER_DECL
TRIGGER_INTEGER TRIGGER_CDECL tgr_ldouble_type_get_integer(tgr_value_ptr value)
{
  return (TRIGGER_INTEGER)value->value.ld;
}

TRIGGER_DECL
TRIGGER_BOOLEAN TRIGGER_CDECL tgr_ldouble_type_get_bool(tgr_value_ptr value)
{
  return value->value.ld != 0;
}

TRIGGER_DECL
TRIGGER_FLOATING TRIGGER_CDECL tgr_ldouble_type_get_floating(tgr_value_ptr value)
{
  return value->value.ld;
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_ldouble_type_do_multiply(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_ldouble(left->value.ld * tgr_value_get_floating(right));
  } else {
    return tgr_do_multiply_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_ldouble_type_do_divide(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_ldouble(left->value.ld / tgr_value_get_floating(right));
  } else {
    return tgr_do_divide_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_ldouble_type_do_plus(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_ldouble(left->value.ld + tgr_value_get_floating(right));
  } else {
    return tgr_do_plus_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_ldouble_type_do_minus(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    return tgr_ldouble(left->value.ld - tgr_value_get_floating(right));
  } else {
    return tgr_do_minus_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_ldouble_type_do_ref_assign(tgr_value_ptr left, tgr_value_ptr right)
{
  if (tgr_value_is_integer(right) || tgr_value_is_floating(right)) {
    *(TRIGGER_FLOATING*)(left->value.ptr) = tgr_value_get_floating(right);
    return *left;
  } else {
    return tgr_do_ref_assign_error(left, right);
  }
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_ldouble_type_to_rvalue(tgr_value_ptr ref)
{
  return tgr_ldouble(*(TRIGGER_FLOATING*)ref->value.ptr);
}

TRIGGER_DECL
tgr_type_ptr TRIGGER_CDECL tgr_ldouble_type(void)
{
  static struct tgr_type_vtbl ldouble_type_vtbl =
  {
    tgr_type_is_integer,
    tgr_floating_type_is_floating,
    tgr_type_is_pointer,
    tgr_type_is_func_pointer,
    tgr_type_is_object,
    tgr_type_is_void,
    tgr_type_is_error,
    tgr_ldouble_type_get_integer,
    tgr_ldouble_type_get_bool,
    tgr_ldouble_type_get_floating,
    tgr_type_get_pointer,
    tgr_type_get_func_pointer,
    tgr_floating_type_get_string,
    tgr_floating_type_get_bool_value,
    tgr_floating_type_is_equal,
    tgr_floating_type_is_greater,
    tgr_floating_type_is_greater_or_equal,
    tgr_ldouble_type_do_multiply,
    tgr_ldouble_type_do_divide,
    tgr_ldouble_type_do_plus,
    tgr_ldouble_type_do_minus,
    tgr_do_assign_error,
    tgr_ldouble_type_do_ref_assign,
    tgr_floating_type_do_not,
    tgr_do_inc_error,
    tgr_do_dec_error,
    tgr_get_field_error,
    tgr_do_at_error,
    tgr_ldouble_type_to_rvalue
  };

  static struct tgr_type ldouble_type =
  {
    &ldouble_type_vtbl,
    /*sub_type=*/0,
    /*ptr_type=*/0,
    /*ref_type=*/0,
    /*array_type=*/0,
    /*next_type=*/0,
    /*size=*/(unsigned int)sizeof(TRIGGER_FLOATING)
  };

  return &ldouble_type;
}

TRIGGER_DECL
tgr_value TRIGGER_CDECL tgr_ldouble(TRIGGER_FLOATING ld)
{
  tgr_value value;
  
  value.type = tgr_ldouble_type();
  value.value.ld = ld;
  return value;
}

/**************************************************************************************************/
#endif /* TRIGGER_IMPLEMENTATION */

TRIGGER_EXTERN_C_END
#endif /* __KERNEL__ */
#endif /* __trigger_c_floating_type_h__ */

