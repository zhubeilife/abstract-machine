#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void inline output(char *out, char c)
{
  if (out != NULL)
  {
     *out = c;
     out++;
  }
  else
  {
    putch(c);
  }
}

void p_itoa(unsigned int value, int* counter, char *out)
{
  //TODO: simply way think the max int is 4294967295, so max len is 10
  unsigned int sub_counter = 1000000000;
  bool leading_zero = true;
  while (sub_counter)
  {
    char zero = '0';
    unsigned int digit = value / sub_counter;

    if (digit || !leading_zero)
    {
      output(out, zero + digit);
      counter++;
      if (digit)
      {
        leading_zero = false;
      }
    }
    else if (digit == 0 && sub_counter == 1)
    {
      output(out, zero);
      counter++;
    }

    value = value % sub_counter;
    sub_counter /= 10;
  }
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int counter = 0;
  int d;
  char c;
  char *s;
  while (*fmt){
    switch (*fmt)
    {
    case '%':
      fmt++;
      switch (*fmt)
      {
      case 'd':                      // int
        d = va_arg(ap, int);
        if (d < 0)
        {
          output(out, '-');
          counter += 1;
          d = -d;
        }
        p_itoa(d, &counter, out);
        break;
      case 'u':                      // unsigned int
        d = va_arg(ap, int);
        p_itoa(d, &counter, out);
        break;
      case 'c':
        /* need a cast here since va_arg only
                 takes fully promoted types */
          c = (char)va_arg(ap, int);
        output(out, c);
        counter++;
        break;
      case 's':                      // string
        s = va_arg(ap, char*);
        for (const char *p = s; *p; p++) output(out, *p);
        counter += strlen(s);
        break;
      }
      break;
    default:
      output(out,*fmt);
      counter++;
      break;
    }
    fmt++;
  }

  if (out != NULL)
  {
    *out = '\0';
    counter++;
  }

  return counter;
}

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int counter = vsprintf(NULL, fmt, ap);

  va_end(ap);

  return counter;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int counter = vsprintf(out, fmt, ap);

  va_end(ap);

  return counter;
}


// The functions snprintf() and vsnprintf() write at most size bytes
// (including the terminating null byte ('\0')) to str.
int snprintf(char *out, size_t n, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int counter = vsnprintf(out, n, fmt, ap);

  va_end(ap);

  return counter;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
