
# Configure paths for Kyra
# stolen from Sam Lantinga 8/4/2002
# Sam Lantinga 9/21/99
# stolen from Manish Singh
# stolen back from Frank Belew
# stolen from Manish Singh
# Shamelessly stolen from Owen Taylor

dnl AM_PATH_KYRA([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for KYRA, and define KYRA_CFLAGS and KYRA_LIBS
dnl
AC_DEFUN(AM_PATH_KYRA,
[dnl 
dnl Get the cflags and libraries from the kyra-config script
dnl
AC_ARG_WITH(kyra-prefix,[  --with-kyra-prefix=PFX   Prefix where KYRA is installed (optional)],
            kyra_prefix="$withval", kyra_prefix="")
AC_ARG_WITH(kyra-exec-prefix,[  --with-kyra-exec-prefix=PFX Exec prefix where KYRA is installed (optional)],
            kyra_exec_prefix="$withval", kyra_exec_prefix="")
AC_ARG_ENABLE(kyratest, [  --disable-kyratest       Do not try to compile and run a test KYRA program],
		    , enable_kyratest=yes)

  if test x$kyra_exec_prefix != x ; then
     kyra_args="$kyra_args --exec-prefix=$kyra_exec_prefix"
     if test x${KYRA_CONFIG+set} != xset ; then
        KYRA_CONFIG=$kyra_exec_prefix/bin/kyra-config
     fi
  fi
  if test x$kyra_prefix != x ; then
     kyra_args="$kyra_args --prefix=$kyra_prefix"
     if test x${KYRA_CONFIG+set} != xset ; then
        KYRA_CONFIG=$kyra_prefix/bin/kyra-config
     fi
  fi

  AC_REQUIRE([AC_CANONICAL_TARGET])
  AC_PATH_PROG(KYRA_CONFIG, kyra-config, no)
  min_kyra_version=ifelse([$1], ,0.11.0,$1)
  AC_MSG_CHECKING(for KYRA - version >= $min_kyra_version)
  no_kyra=""
  if test "$KYRA_CONFIG" = "no" ; then
    no_kyra=yes
  else
    KYRA_CFLAGS=`$KYRA_CONFIG $kyraconf_args --cflags`
    KYRA_LIBS=`$KYRA_CONFIG $kyraconf_args --libs`

    kyra_major_version=`$KYRA_CONFIG $kyra_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    kyra_minor_version=`$KYRA_CONFIG $kyra_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    kyra_build_version=`$KYRA_CONFIG $kyra_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_kyratest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $KYRA_CFLAGS"
      LIBS="$LIBS $KYRA_LIBS"
      CXXFLAGS="$CXXFLAGS $CFLAGS"
dnl
dnl Now check if the installed KYRA is sufficiently new. (Also sanity
dnl checks the results of kyra-config to some extent
dnl
      rm -f conf.kyratest
AC_LANG_PUSH(C++)
	AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kyra.h"

char*
my_strdup (char *str)
{
  char *new_str;
  
  if (str)
    {
      new_str = (char *)malloc ((strlen (str) + 1) * sizeof(char));
      strcpy (new_str, str);
    }
  else
    new_str = NULL;
  
  return new_str;
}

int main (int argc, char *argv[])
{
  int major, minor, build;
  char *tmp_version;

  /* This hangs on some systems (?)
  system ("touch conf.kyratest");
  */
  { FILE *fp = fopen("conf.kyratest", "a"); if ( fp ) fclose(fp); }

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_kyra_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &build) != 3) {
     printf("%s, bad version string\n", "$min_kyra_version");
     exit(1);
   }

   if (($kyra_major_version > major) ||
      (($kyra_major_version == major) && ($kyra_minor_version > minor)) ||
      (($kyra_major_version == major) && ($kyra_minor_version == minor) && ($kyra_build_version >= build)))
    {
      return 0;
    }
  else
    {
      printf("\n*** 'kyra-config --version' returned %d.%d.%d, but the minimum version\n", $kyra_major_version, $kyra_minor_version, $kyra_build_version);
      printf("*** of KYRA required is %d.%d.%d. If kyra-config is correct, then it is\n", major, minor, build);
      printf("*** best to upgrade to the required version.\n");
      printf("*** If kyra-config was wrong, set the environment variable KYRA_CONFIG\n");
      printf("*** to point to the correct copy of kyra-config, and remove the file\n");
      printf("*** config.cache before re-running configure\n");
      return 1;
    }
}

],, no_kyra=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
AC_LANG_POP()
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_kyra" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$KYRA_CONFIG" = "no" ; then
       echo "*** The kyra-config script installed by KYRA could not be found"
       echo "*** If KYRA was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the KYRA_CONFIG environment variable to the"
       echo "*** full path to kyra-config."
     else
       if test -f conf.kyratest ; then
        :
       else
          echo "*** Could not run KYRA test program, checking why..."
          CFLAGS="$CFLAGS $KYRA_CFLAGS"
          LIBS="$LIBS $KYRA_LIBS"
dnl AC_LANG_PUSH(C++)  
	AC_TRY_LINK([
#include <stdio.h>
#include "kyra.h"

int main(int argc, char *argv[])
{ return 0; }
#undef  main
#define main K_and_R_C_main
],      [ return 0; ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding KYRA or finding the wrong"
          echo "*** version of KYRA. If it is not finding KYRA, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means KYRA was incorrectly installed"
          echo "*** or that you have moved KYRA since it was installed. In the latter case, you"
          echo "*** may want to edit the kyra-config script: $KYRA_CONFIG" ])
dnl AC_LANG_POP()
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     KYRA_CFLAGS=""
     KYRA_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(KYRA_CFLAGS)
  AC_SUBST(KYRA_LIBS)
  rm -f conf.kyratest
])
