#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void inline output(char **out, char c)
{
  if (out != NULL && *out != NULL)
  {
     **out = c;
     (*out)++;
  }
  else
  {
    putch(c);
  }
}

void p_itoa(unsigned int value, int* counter, char **out)
{
  // 这里采用的方法是先输出最高位，还有一个办法是不用这种除法，
  // 而是可以申请一块内存，先从最低位开始输出，然后再反向的打印
  // 下面的hex的输出就是按照这种方式
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
      (*counter)++;
      if (digit)
      {
        leading_zero = false;
      }
    }
    else if (digit == 0 && sub_counter == 1)
    {
      output(out, zero);
      (*counter)++;
    }

    value = value % sub_counter;
    sub_counter /= 10;
  }
}

#define SPRINT_BUF_SIZE 1024
static char sprint_buf[SPRINT_BUF_SIZE];

void p_itoa_hex(unsigned int value, int* counter, char **out)
{
  output(out, '0');
  output(out, 'x');
  counter+=2;

  int size = 0;
  while (value) {
    int digit = value % 16;
    char digit_char = digit < 10 ? '0' + digit : 'a' + digit - 10;
    sprint_buf[size++] = digit_char;
    value /= 16;
  }
  while (size--) {
    output(out, sprint_buf[size]);
    counter++;
  }
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int counter = 0;
  int d;
  char c;
  char *s;
  char *out_ptr = out;
  char **outp = (out != NULL) ? &out_ptr : NULL;
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
          output(outp, '-');
          counter += 1;
          d = -d;
        }
        p_itoa(d, &counter, outp);
        break;
      case 'u':                      // unsigned int
        d = va_arg(ap, int);
        p_itoa(d, &counter, outp);
        break;
      case 'p':
      case 'x':
          uint32_t d = va_arg(ap, uint32_t);
          p_itoa_hex(d, &counter, outp);
          break;
      case 'c':
        /* need a cast here since va_arg only
                 takes fully promoted types */
          c = (char)va_arg(ap, int);
        output(outp, c);
        counter++;
        break;
      case 's':                      // string
        s = va_arg(ap, char*);
        for (const char *p = s; *p; p++) output(outp, *p);
        counter += strlen(s);
        break;
      }
      break;
    default:
      output(outp,*fmt);
      counter++;
      break;
    }
    fmt++;
  }

  if (outp != NULL)
  {
    output(outp, '\0');
    // 不计入 counter
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
