/* Declarations for common convenience functions.
   Copyright (C) 2006-2011 Red Hat, Inc.
   This file is part of elfutils.

   This file is free software; you can redistribute it and/or modify
   it under the terms of either

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at
       your option) any later version

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at
       your option) any later version

   or both in parallel, as here.

   elfutils is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see <http://www.gnu.org/licenses/>.  */

#ifndef LIB_SYSTEM_H
#define LIB_SYSTEM_H	1

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/param.h>
#include "portable_endian.h"
#include <unistd.h>
#include <string.h>

# define LE32(n)	le32toh (n)
# define LE64(n)	le64toh (n)
# define BE32(n)	be32toh (n)
# define BE64(n)	be64toh (n)

#ifndef MAX
#define MAX(m, n) ((m) < (n) ? (n) : (m))
#endif

#ifndef MIN
#define MIN(m, n) ((m) < (n) ? (m) : (n))
#endif

#if !HAVE_DECL_MEMPCPY
#define mempcpy(dest, src, n) \
	((void *) ((char *) memcpy (dest, src, n) + (size_t) n))
#endif

/* A special gettext function we use if the strings are too short.  */
#define sgettext(Str) \
  ({ const char *__res = strrchr (gettext (Str), '|');			      \
     __res ? __res + 1 : Str; })

#define gettext_noop(Str) Str

#ifndef TEMP_FAILURE_RETRY
#define TEMP_FAILURE_RETRY(expression) \
  ({ ssize_t __res; \
     do \
       __res = expression; \
     while (__res == -1 && errno == EINTR); \
     __res; })
#endif

static inline ssize_t __attribute__ ((unused))
pwrite_retry (int fd, const void *buf, size_t len, off_t off)
{
  ssize_t recvd = 0;

  do
    {
      ssize_t ret = TEMP_FAILURE_RETRY (pwrite (fd, buf + recvd, len - recvd,
						off + recvd));
      if (ret <= 0)
	return ret < 0 ? ret : recvd;

      recvd += ret;
    }
  while ((size_t) recvd < len);

  return recvd;
}

static inline ssize_t __attribute__ ((unused))
write_retry (int fd, const void *buf, size_t len)
{
  ssize_t recvd = 0;

  do
    {
      ssize_t ret = TEMP_FAILURE_RETRY (write (fd, buf + recvd, len - recvd));
      if (ret <= 0)
	return ret < 0 ? ret : recvd;

      recvd += ret;
    }
  while ((size_t) recvd < len);

  return recvd;
}

static inline ssize_t __attribute__ ((unused))
pread_retry (int fd, void *buf, size_t len, off_t off)
{
  ssize_t recvd = 0;

  do
    {
      ssize_t ret = TEMP_FAILURE_RETRY (pread (fd, buf + recvd, len - recvd,
					       off + recvd));
      if (ret <= 0)
	return ret < 0 ? ret : recvd;

      recvd += ret;
    }
  while ((size_t) recvd < len);

  return recvd;
}

/* The demangler from libstdc++.  */
extern char *__cxa_demangle (const char *mangled_name, char *output_buffer,
			     size_t *length, int *status);

/* A static assertion.  This will cause a compile-time error if EXPR,
   which must be a compile-time constant, is false.  */

#define eu_static_assert(expr)						\
  extern int never_defined_just_used_for_checking[(expr) ? 1 : -1]	\
    __attribute__ ((unused))

#endif /* system.h */
