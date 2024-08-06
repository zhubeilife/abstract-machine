#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

// copy from
// https://github.com/Kingfish404/am-kernels/blob/ics2021/tests/klib-tests/src/main.c

__attribute__((noinline)) void check(bool cond)
{
    if (!cond)
        halt(1);
}

#define N 32
uint8_t data[N];

// 检查[l,r)区间中的值是否依次为val, val + 1, val + 2...
void check_seq(int l, int r, int val)
{
    int i;
    for (i = l; i < r; i++)
    {
        check(data[i] == val + i - l);
    }
}

// 检查[l,r)区间中的值是否均为val
void check_eq(int l, int r, int val)
{
    int i;
    for (i = l; i < r; i++)
    {
        check(data[i] == val);
    }
}

void reset()
{
    int i;
    for (i = 0; i < N; i++)
    {
        data[i] = i + 1;
    }
}

void test_memset()
{
    int l, r;
    for (l = 0; l < N; l++)
    {
        for (r = l + 1; r <= N; r++)
        {
            reset();
            uint8_t val = (l + r) / 2;
            memset(data + l, val, r - l);
            check_seq(0, l, 1);
            check_eq(l, r, val);
            check_seq(r, N, r + 1);
        }
    }
}

void test_memmove()
{
    char str[] = "1234567890";
    memmove(str + 4, str + 3, 3); // copies from [4, 5, 6] to [5, 6, 7]
    if (strcmp(str, "1234456890") != 0) putstr("memmove fail with 1234567890\n");

    char buf[] = "0123456789";
    int buf_len = sizeof(buf) / sizeof(char) - 1;
    for (size_t i = 0; i < 10; i++)
    {
        check(memmove(data + i, buf, buf_len) == data + i);
        check_seq(i, i + buf_len, buf[0]);
    }

    memmove(data, buf, buf_len);
    for (size_t i = 1; i < 10; i++)
    {
        check(memmove(data + i, data + i - 1, buf_len) == data + i);
        check_seq(i, i + buf_len, buf[0]);
    }
}

void test_memcpy()
{
    char buf[] = "0123456789";
    int buf_len = sizeof(buf) / sizeof(char) - 1;
    for (size_t i = 0; i < 10; i++)
    {
        check(memmove(data + i, buf, buf_len) == data + i);
        check_seq(i, i + buf_len, buf[0]);
    }
}

void test_memcmp()
{
    char buf[] = "0123456789";
    int buf_len = sizeof(buf) / sizeof(char) - 1;
    data[0] = '\0';
    check(memcmp(buf, buf, buf_len) == 0);
    check(memcmp(buf, (char *)data, buf_len) > 0);
    data[0] = '1';
    check(memcmp(buf, (char *)data, buf_len) < 0);
}

// end of steal

char *s[] = {
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",      // 0
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab",      // 1
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",      // 2
    ", World!\n",                                  // 3
    "Hello, World!\n",                             // 4
    "#####",                                       // 5
    "dog cat\0mouse"                               // 6
};

void test_strcmp(){
    if (strcmp("ABC", "ABC") != 0) putstr("ABC ABC fail\n");
    if (strcmp("ABC", "AB") != 67) putstr("ABC AB fail\n");
    if (strcmp("ABA", "ABZ") != -25) putstr("ABA ABZ fail\n");
    if (strcmp("ABJ", "ABC") != 7) putstr("ABJ ABC fail\n");
    if (strcmp("ABJ", "ABC") != 7) putstr("ABJ ABC fail\n");
    if (strcmp("\201", "A") != 64) putstr("'\201' A fail\n");

    if (strcmp(s[0], s[2]) != 0) putstr("s[0] s[2] fail\n");
    if (strcmp(s[0], s[1]) != -1) putstr("s[0] s[1] fail\n");
    if (strcmp(s[0] + 1, s[1] + 1) != -1) putstr("s[0]+1 s[1]+1 fail\n");
    if (strcmp(s[0] + 2, s[1] + 2) != -1) putstr("s[0]+2 s[1]+2 fail\n");
    if (strcmp(s[0] + 3, s[1] + 3) != -1) putstr("s[0]+3 s[1]+3 fail\n");
}

void test_strncmp()
{
    if (strncmp("ABC", "ABC", 50) != 0) putstr("ABC ABC fail\n");
    if (strncmp("ABC", "AB", 50) != 67) putstr("ABC AB fail\n");
    if (strncmp("ABA", "ABZ", 50) != -25) putstr("ABA ABZ fail\n");
    if (strncmp("ABJ", "ABC", 50) != 7) putstr("ABJ ABC fail\n");
    if (strncmp("ABJ", "ABC", 50) != 7) putstr("ABJ ABC fail\n");
    if (strncmp("\201", "A", 50) != 64) putstr("'\201' A fail\n");

    if (strncmp(s[0], s[2], 50) != 0) putstr("s[0] s[2] fail\n");
    if (strncmp(s[0], s[1], 50) != -1) putstr("s[0] s[1] fail\n");
    if (strncmp(s[0], s[1], 10) != 0) putstr("s[0] s[1] fail\n");
    if (strncmp(s[0] + 1, s[1] + 1, 50) != -1) putstr("s[0]+1 s[1]+1 fail\n");
    if (strncmp(s[0] + 2, s[1] + 2, 50) != -1) putstr("s[0]+2 s[1]+2 fail\n");
    if (strncmp(s[0] + 3, s[1] + 3, 50) != -1) putstr("s[0]+3 s[1]+3 fail\n");
}

void test_strlen(){
    if (strlen(s[0]) != 38) putstr("aaaa: size fail\n");
    if (strlen(s[6]) != 7) putstr("dog cat\0mouse: size fail\n");
    if (strlen(s[3]) != 9) putstr(", World!\n: size fail\n");
}

void test_strcpy(){
    char *s1 = "Hello, World!\n";
    char s2[30] = "";
    strcpy(s2, s1);
    if (strcmp(s2, s1) != 0) putstr("test_strcpy fail\n");
}

void test_strncpy(){
    char *s1 = "Hello, World!\n";
    char s2[30] = "";
    strncpy(s2, s1, 4);
    if (strlen(s2) != 4) putstr("test_strncpy fail\n");
}

void test_strcat(){
    char str1[50] = "Hello ";
    char str2[50] = "World!";
    strcat(str1, str2);
    strcat(str1, " Goodbye World!");
    strcat(str1, "\n");
    if (strcmp(str1, "Hello World! Goodbye World!\n") != 0) putstr("test_strcat fail\n");


    char *s[] = {
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab",
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        ", World!\n",
        "Hello, World!\n",
        "#####"
    };

    char str3[] = "Hello";
    char str4[20] = "";
    if (strcmp(strcat(strcpy(str4, str3), s[3]), s[4]) != 0 ) putstr("test_strcat fail\n");
}

void test_printf(){
    char buf[128] = "";

    sprintf(buf, "%s", "Hello world!\n");
    check(strcmp(buf, "Hello world!\n") == 0);

    sprintf(buf, "%d + %d = %d\n", 1, 1, 2);
    check(strcmp(buf, "1 + 1 = 2\n") == 0);

    sprintf(buf, "%d + %d = %d\n", 2, 10, 12);
    check(strcmp(buf, "2 + 10 = 12\n") == 0);
}

int main() {
    putstr("========== Test Start ===========\n");

    test_strlen();
    test_strcmp();
    test_strncmp();
    test_strcpy();
    test_strncpy();
    test_strcat();

    test_memset();
    test_memmove();
    test_memcpy();
    test_memcmp();

    test_printf();

    putstr("========== Test end ===========\n");
    return 0;
}