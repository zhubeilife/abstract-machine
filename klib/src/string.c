#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

// The strlen() function calculates the length of the string pointed
// to by s, excluding the terminating null byte ('\0').
size_t strlen(const char *s) {
  if (s == NULL) {
    return 0;
  }

  size_t len = 0;
  while (*s++ != '\0') {
    len++;
  }
  return len;
}

// The strcpy() function copies the string pointed to by src,
// including the terminating null byte ('\0'), to the buffer pointed
// to by dest.  The strings may not overlap, and the destination
// string dest must be large enough to receive the copy.  Beware of
// buffer overruns!
char *strcpy(char *dst, const char *src) {
  if (dst == NULL || src == NULL) {
    return dst;
  }

  char *originalDst = dst; // 保存dst的原始值
  while ((*dst++ = *src++) != '\0') {
    ;
  }
  return originalDst;
}

// The strncpy() function is similar, except that at most n bytes of
// src are copied.  Warning: If there is no null byte among the
// first n bytes of src, the string placed in dest will not be null-
// terminated.
char *strncpy(char *dst, const char *src, size_t n) {
  if (dst == NULL || src == NULL) {
    return dst;
  }

  char *originalDst = dst; // 保存dst的原始值
  size_t i = 0;

  for (i = 0; i < n && src[i] != '\0'; i++)
    dst[i] = src[i];
  for ( ; i < n; i++)
    dst[i] = '\0';

  return originalDst;
}

// The strcat() function appends the src string to the dest string,
// overwriting the terminating null byte ('\0') at the end of dest,
// and then adds a terminating null byte.  The strings may not
// overlap, and the dest string must have enough space for the
// result.  If dest is not large enough, program behavior is
// unpredictable; buffer overruns are a favorite avenue for
// attacking secure programs.
char *strcat(char *dst, const char *src) {
  if (dst == NULL || src == NULL) {
    return dst;
  }

  char *originalDst = dst; // 保存dst的原始值
  while (*dst != '\0') {
    dst++;
  }

  while ((*dst++ = *src++) != '\0') {
    ;
  }
  return originalDst;
}

/*
   The strcmp() function compares the two strings s1 and s2.  The
   locale is not taken into account (for a locale-aware comparison,
   see strcoll(3)).  The comparison is done using unsigned
   characters.

   strcmp() returns an integer indicating the result of the
   comparison, as follows:

   • 0, if the s1 and s2 are equal;
   • a negative value if s1 is less than s2;
   • a positive value if s1 is greater than s2.

   The strncmp() function is similar, except it compares only the
   first (at most) n bytes of s1 and s2.
 */
int strcmp(const char *s1, const char *s2) {
  while (*s2 != '\0' && *s1 != '\0') {
     int sub = *s1 - *s2;
     if (sub != 0) {
       return sub;
     }
     s1++;
     s2++;
  }

  if (*s1 == '\0' && *s2 != '\0') {
    return 0;
  }
  else {
     return *s1 - *s2;
  }
}

int strncmp(const char *s1, const char *s2, size_t n) {
  size_t counter = 0;
  while (*s2 != '\0' && *s1 != '\0' && counter < n) {
    int sub = *s1 - *s2;
    if (sub != 0) {
      return sub;
    }
    s1++;
    s2++;
    counter++;
  }

  if ((*s1 == '\0' && *s2 != '\0') || counter == n) {
    return 0;
  }
  else {
    return *s1 - *s2;
  }
}

// The memset() function fills the first n bytes of the memory area
// pointed to by s with the constant byte c.
void *memset(void *s, int c, size_t n) {
  unsigned char *ptr = (unsigned char *)s; // 将void*转换为unsigned char*以安全递增
  while (n--) {
    *ptr++ = (unsigned char)c; // 将c的低8位赋值给ptr指向的字节，并递增ptr
  }
  return s; // 返回原始的s指针
}


// The memmove() function copies n bytes from memory area src to
// memory area dest.  The memory areas may overlap: copying takes
// place as though the bytes in src are first copied into a
// temporary array that does not overlap src or dest, and the bytes
// are then copied from the temporary array to dest.
void *memmove(void *dst, const void *src, size_t n) {
  char *originalDst = dst; // 保存dst的原始值
  // if src == dst, do nothing
  if (src > dst) {
    while(n--) {
      *(char *)dst = *(char *)src; // 将src指向的字节赋值给dst指向的字节，并递增dst和src
      dst++;
      src++;
    }
  } else if (src < dst) {
    dst += n-1;
    src += n-1;
    while(n--) {
      *(char *)dst = *(char *)src; // 将src指向的字节赋值给dst指向的字节，并递增dst和src
      dst--;
      src--;
    }
  }
  return originalDst;
}

// The memcpy() function copies n bytes from memory area src to
// memory area dest.  The memory areas must not overlap.  Use
// memmove(3) if the memory areas do overlap.
void *memcpy(void *out, const void *in, size_t n) {
  char *originalDst = out; // 保存dst的原始值
  while(n--) {
    *(char *)out = *(char *)in; // 将src指向的字节赋值给dst指向的字节，并递增dst和src
    out++;
    in++;
  }
  return originalDst;
}

// The memcmp() function compares the first n bytes (each
// interpreted as unsigned char) of the memory areas s1 and s2.
int memcmp(const void *s1, const void *s2, size_t n) {
  return strncmp((const char *)s1, (const char *)s2, n);
}

#endif
