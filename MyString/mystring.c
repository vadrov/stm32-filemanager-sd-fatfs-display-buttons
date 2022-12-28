/*
 * mystring.c
 *
 *  Author: VadRov
 *
 *  https://www.youtube.com/@VadRov
 *  https://dzen.ru/vadrov
 *  https://vk.com/vadrov
 *  https://t.me/vadrov_channel
 *
 */

#include "../MyString/mystring.h"
#include "stdarg.h"
#include "string.h"
#include "stdlib.h"

//копирует строку 2 в строку 1, перенося максимум len символов и не более длины строки str2
void my_strcpy_fixed_sym(char *str1, const char *str2, int len)
{
	while (len > 0 && *str2)
	{
		*str1 = *str2;
		str1++;
		str2++;
		len--;
	}
	*str1 = '\0';
}

//присоединяет строку 2 к строке 1, перенося максимум len символов и не более длины строки str2
void my_strcat_fixed_sym (char *str1, const char *str2, int len)
{
	str1 += my_strlen(str1);
	while (len > 0 && *str2)
	{
		*str1 = *str2;
		str1++;
		str2++;
		len--;
	}
	*str1 = '\0';
}

//присоединяет строку 2 к строке 1
void my_strcat (char *str1, const char *str2)
{
	str1 += my_strlen(str1);
	while (*str2)
	{
		*str1 = *str2;
		str1++;
		str2++;
	}
	*str1 = '\0';
}

//копирует строку 2 в строку 1
void my_strcpy(char *str1, const char *str2)
{
   while(*str2)
   {
      *str1 = *str2;
      str1++;
      str2++;
   }
   *str1 = '\0';
}

//выделяет память под строку и копирует строку в эту область памяти
//возвращает начальный адрес памяти этой строки
char* my_strcpy_mem(const char *str)
{
	char *mem = (char *)calloc(my_strlen(str) + 1, 1);
	if (mem) my_strcpy(mem, str);
	return mem;
}

//возвращает длину строки
uint32_t my_strlen(const char *str1)
{
	uint32_t i = 0;
	while (*str1) {
		str1++;
		i++;
	}
	return i;
}

//заполняет строку заданным значением (0...255)
void my_strset(char *str1, uint8_t code, uint32_t len)
{
	while (len)
	{
		*str1 = code;
		str1++;
		len--;
	}
}

//сшивает несколько строк в одну строку с разделителем
//n - число строк, separator - строка-разделитель, дальше строки количеством n
//пустые строки игнорирует
char* my_strsplice (int n, const char *separator, ...)
{
	va_list spisok;
	va_start(spisok, separator);
	int len = 0, i;
	i = n;
	while (i)
	{
		char *st = va_arg(spisok, char *);
		if (my_strlen(st))
		{
			len+=my_strlen(st);
			if (i != 1) len+=my_strlen(separator);
		}
		i--;
	}
	char *text = calloc(len+1, sizeof(char));
	if (text==0) return 0;
	va_start(spisok, separator);
	i = n;
	while (i)
	{
		char *st = va_arg(spisok, char *);
		if (my_strlen(st))
		{
			my_strcat(text, st);
			if (i != 1) my_strcat(text, separator);
		}
		i--;
	}
	return text;
}

//поиск строки str в строке buffer
//размер буфера size
char *memstr(char *buffer, char *str, uint32_t size)
{
	char *pos;
	char lenstr = my_strlen(str);
	for (pos = buffer; pos <= (buffer - lenstr + size); pos++)
	{
		if (memcmp(pos, str, lenstr) == 0)
			return pos;
	}
	return NULL;
}
