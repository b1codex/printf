PRINTF PROJECT
PRINTF PROJECT

repository: printf



README

# printf

Team Task

Group Project


vi main.h

#ifndef MAIN_H

#define MAIN_H

#include <stdarg.h>

#include <stdio.h>

#include <unistd.h>


#define UNUSED(x) (void)(x)

#define BUFF_SIZE 1024


/* FLAGS */

#define F_MINUS 1

#define F_PLUS 2

#define F_ZERO 4

#define F_HASH 8

#define F_SPACE 16


/* SIZES */

#define S_LONG 2

#define S_SHORT 1


/**

 * struct fmt - Struct op

 *

 * @fmt: The format.

 * @fn: The function associated.

 */

struct fmt

{

        char fmt;

        int (*fn)(va_list, char[], int, int, int, int);

};



/**

 * typedef struct fmt fmt_t - Struct op

 *

 * @fmt: The format.

 * @fm_t: The function associated.

 */

typedef struct fmt fmt_t;


int _printf(const char *format, ...);

int handle_print(const char *fmt, int *i,

va_list list, char buffer[], int flags, int width, int precision, int size);


/****************** FUNCTIONS ******************/


/* Funtions to print chars and strings */

int print_char(va_list types, char buffer[],

        int flags, int width, int precision, int size);

int print_string(va_list types, char buffer[],

        int flags, int width, int precision, int size);

int print_percent(va_list types, char buffer[],

        int flags, int width, int precision, int size);


/* Functions to print numbers */

int print_int(va_list types, char buffer[],

        int flags, int width, int precision, int size);

int print_binary(va_list types, char buffer[],

        int flags, int width, int precision, int size);

int print_unsigned(va_list types, char buffer[],

        int flags, int width, int precision, int size);

int print_octal(va_list types, char buffer[],

        int flags, int width, int precision, int size);

int print_hexadecimal(va_list types, char buffer[],

        int flags, int width, int precision, int size);

int print_hexa_upper(va_list types, char buffer[],

        int flags, int width, int precision, int size);


int print_hexa(va_list types, char map_to[],

char buffer[], int flags, char flag_ch, int width, int precision, int size);


/* Function to print non printable characters */

int print_non_printable(va_list types, char buffer[],

        int flags, int width, int precision, int size);


/* Funcion to print memory address */

int print_pointer(va_list types, char buffer[],

        int flags, int width, int precision, int size);


/* Funciotns to handle other specifiers */

int get_flags(const char *format, int *i);

int get_width(const char *format, int *i, va_list list);

int get_precision(const char *format, int *i, va_list list);

int get_size(const char *format, int *i);


/*Function to print string in reverse*/

int print_reverse(va_list types, char buffer[],

        int flags, int width, int precision, int size);


/*Function to print a string in rot 13*/

int print_rot13string(va_list types, char buffer[],

        int flags, int width, int precision, int size);


/* width handler */

int handle_write_char(char c, char buffer[],

        int flags, int width, int precision, int size);

int write_number(int is_positive, int ind, char buffer[],

        int flags, int width, int precision, int size);

int write_num(int ind, char bff[], int flags, int width, int precision,

        int length, char padd, char extra_c);

int write_pointer(char buffer[], int ind, int length,

        int width, int flags, char padd, char extra_c, int padd_start);


int write_unsgnd(int is_negative, int ind,

char buffer[],

        int flags, int width, int precision, int size);


/****************** UTILS ******************/

int is_printable(char);

int append_hexa_code(char, char[], int);

int is_digit(char);


long int convert_size_number(long int num, int size);

long int convert_size_unsgnd(unsigned long int num, int size);


#endif /* MAIN_H */



vi _printf.c


#include "main.h"


void print_buffer(char buffer[], int *buff_ind);


/**

 * _printf - Printf function

 * @format: format.

 * Return: Printed chars.

 */

int _printf(const char *format, ...)

{

        int i, printed = 0, printed_chars = 0;

        int flags, width, precision, size, buff_ind = 0;

        va_list list;

        char buffer[BUFF_SIZE];


        if (format == NULL)

                return (-1);


        va_start(list, format);


        for (i = 0; format && format[i] != '\0'; i++)

        {

                if (format[i] != '%')

                {

                        buffer[buff_ind++] = format[i];

                        if (buff_ind == BUFF_SIZE)

                                print_buffer(buffer, &buff_ind);

                        /* write(1, &format[i], 1);*/

                        printed_chars++;

                }

                else

                {

                        print_buffer(buffer, &buff_ind);

                        flags = get_flags(format, &i);

                        width = get_width(format, &i, list);

                        precision = get_precision(format, &i, list);

                        size = get_size(format, &i);

                        ++i;

                        printed = handle_print(format, &i, list, buffer,

                                flags, width, precision, size);

                        if (printed == -1)

                                return (-1);

                        printed_chars += printed;

                }

        }


        print_buffer(buffer, &buff_ind);


        va_end(list);


        return (printed_chars);

}


/**

 * print_buffer - Prints the contents of the buffer if it exist

 * @buffer: Array of chars

 * @buff_ind: Index at which to add next char, represents the length.

 */

void print_buffer(char buffer[], int *buff_ind)

{

        if (*buff_ind > 0)

                write(1, &buffer[0], *buff_ind);


        *buff_ind = 0;

}


vi putchar


#include <unistd.h>

#include "main.h"

/**

 * _putchar - writes the character c to stdout

 * @c: The character to print

 *

 * Return: On success 1.

 * On error, -1 is returned, and errno is set appropriately.

 */



int _putchar(char c)

{

        return (write(1, &c, 1));

}




vi functions.c


#include "main.h"


/************************* PRINT CHAR *************************/


/**

 * print_char - Prints a char

 * @types: List a of arguments

 * @buffer: Buffer array to handle print

 * @flags:  Calculates active flags

 * @width: Width

 * @precision: Precision specification

 * @size: Size specifier

 * Return: Number of chars printed

 */

int print_char(va_list types, char buffer[],

        int flags, int width, int precision, int size)

{

        char c = va_arg(types, int);


        return (handle_write_char(c, buffer, flags, width, precision, size));

}

/************************* PRINT A STRING *************************/

/**

 * print_string - Prints a string

 * @types: List a of arguments

 * @buffer: Buffer array to handle print

 * @flags:  Calculates active flags

 * @width: get width.

 * @precision: Precision specification

 * @size: Size specifier

 * Return: Number of chars printed

 */

int print_string(va_list types, char buffer[],

        int flags, int width, int precision, int size)

{

        int length = 0, i;

        char *str = va_arg(types, char *);


        UNUSED(buffer);

        UNUSED(flags);

        UNUSED(width);

        UNUSED(precision);

        UNUSED(size);

        if (str == NULL)

        {

                str = "(null)";

                if (precision >= 6)

                        str = "      ";

        }


        while (str[length] != '\0')

                length++;


        if (precision >= 0 && precision < length)

                length = precision;


        if (width > length)

        {

                if (flags & F_MINUS)

                {

                        write(1, &str[0], length);

                        for (i = width - length; i > 0; i--)

                                write(1, " ", 1);

                        return (width);

                }

                else

                {

                        for (i = width - length; i > 0; i--)

                                write(1, " ", 1);

                        write(1, &str[0], length);

                        return (width);

                }

        }


        return (write(1, str, length));

}

/************************* PRINT PERCENT SIGN *************************/

/**

 * print_percent - Prints a percent sign

 * @types: Lista of arguments

 * @buffer: Buffer array to handle print

 * @flags:  Calculates active flags

 * @width: get width.

 * @precision: Precision specification

 * @size: Size specifier

 * Return: Number of chars printed

 */

int print_percent(va_list types, char buffer[],

        int flags, int width, int precision, int size)

{

        UNUSED(types);

        UNUSED(buffer);

        UNUSED(flags);

        UNUSED(width);

        UNUSED(precision);

        UNUSED(size);

        return (write(1, "%%", 1));

}


/************************* PRINT INT *************************/

/**

 * print_int - Print int

 * @types: Lista of arguments

 * @buffer: Buffer array to handle print

 * @flags:  Calculates active flags

 * @width: get width.

 * @precision: Precision specification

 * @size: Size specifier

 * Return: Number of chars printed

 */

int print_int(va_list types, char buffer[],

        int flags, int width, int precision, int size)

{

        int i = BUFF_SIZE - 2;

        int is_negative = 0;

        long int n = va_arg(types, long int);

        unsigned long int num;


        n = convert_size_number(n, size);


        if (n == 0)

                buffer[i--] = '0';


        buffer[BUFF_SIZE - 1] = '\0';

        num = (unsigned long int)n;


        if (n < 0)

        {

                num = (unsigned long int)((-1) * n);

                is_negative = 1;

        }


        while (num > 0)

        {

                buffer[i--] = (num % 10) + '0';

                num /= 10;

        }


        i++;


        return (write_number(is_negative, i, buffer, flags, width, precision, size));

}


/************************* PRINT BINARY *************************/

/**

 * print_binary - Prints an unsigned number

 * @types: Lista of arguments

 * @buffer: Buffer array to handle print

 * @flags:  Calculates active flags

 * @width: get width.

 * @precision: Precision specification

 * @size: Size specifier

 * Return: Numbers of char printed.

 */

int print_binary(va_list types, char buffer[],

        int flags, int width, int precision, int size)

{

        unsigned int n, m, i, sum;

        unsigned int a[32];

        int count;


        UNUSED(buffer);

        UNUSED(flags);

        UNUSED(width);

        UNUSED(precision);

        UNUSED(size);


        n = va_arg(types, unsigned int);

        m = 2147483648; /* (2 ^ 31) */

        a[0] = n / m;

        for (i = 1; i < 32; i++)

        {

                m /= 2;

                a[i] = (n / m) % 2;

        }

        for (i = 0, sum = 0, count = 0; i < 32; i++)

        {

                sum += a[i];

                if (sum || i == 31)

                {

                        char z = '0' + a[i];


                        write(1, &z, 1);

                        count++;

                }

        }

        return (count);

}
