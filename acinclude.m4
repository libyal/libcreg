dnl Function to detect if libcreg dependencies are available
AC_DEFUN([AX_LIBCREG_CHECK_LOCAL],
 [dnl Headers included in libcreg/libcreg_file.h, libcreg/libcreg_key.h
 dnl libcreg/libcreg_key_item_values.h and libcreg/libcreg_value_item_values.h
 AC_CHECK_HEADERS([wctype.h])

 dnl Functions used in libcreg/libcreg_file.h, libcreg/libcreg_key.h
 dnl libcreg/libcreg_key_item_values.h and libcreg/libcreg_value_item_values.h
 AC_CHECK_FUNCS([towupper])

 AS_IF(
  [test "x$ac_cv_func_towupper" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: towupper],
   [1])
  ])

 dnl Check for internationalization functions in libcreg/libcreg_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

