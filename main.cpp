#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h> //Библиотека для использования функций динамического распределения памяти
#include <stdlib.h>
#include <locale.h>
#define YES 1
#define NO 0
#define MAXL 256 //Максимальная длина строки
#define NLIN 100 //Максимальное число строк

int wcs(char lines[][MAXL], int n);
int wcr(char** lines, int n);
int main(void)
{
	setlocale(LC_ALL, "Rus"); //Включение локализации.
	FILE* fpin; //Входной файл
	FILE* fpout; //Выходной файл
	int nls, nlr; //Число строк в файле
	static char lines[NLIN][MAXL]; //Статический массив для хранения строк
	char** lp = NULL; //Указатель на динамический массив
	int np; //Уисло указателей в динамическом массиве
	int a, b;
	int nws, nwr; //Число слов в массиве строк

	//Открытие файлов для чтения и записи
	printf("Открытие файлов...\n\n");
	fopen_s(&fpin, "test.txt", "rt");
	fopen_s(&fpout, "result.txt", "w");
	if (fpin == NULL)
		return; //Ошибка при открытии файла

	//Заказать и обнулить динамический массив указателей
#if 1
	lp = (char**)malloc(NLIN * sizeof(char*)); //Выделение блока памяти для массива указателей и дальнейшее обнуление
	memset(lp, 0, NLIN * sizeof(char*)); //Заполнение кол-ва строчек в специально зарезервированное место
#else
	lines_ptr = (char**)calloc(NLIN, sizeof(char*)); //Возвращение указателя на динамический массив из NLIN элементов
#endif
	np = NLIN;
	nls = 0;
	nlr = 0;
	while (!feof(fpin)) //Цикл до конца файла
	{
		char line[MAXL]; //Чтение строки
		char* ptr = fgets(line, MAXL, fpin);
		int len;
		if (ptr == NULL)
			break; //Файл исчерпан
		if (nls < NLIN)	//Пока статический массив не исчерпан, сохраняем строку в статическом массиве.
		{
			strcpy(&lines[nls][0], line); //Копирование строки в буфер.
			nls++;
		}

		len = (int)strlen(line); //Определить длину строки
		if (nlr == np) //Динамический массив исчерпан
		{
			np += NLIN; //Увеличить число указателей в массиве
			lp = (char**)realloc(lp, np * sizeof(char*)); //Перезаказать память для массива указателей
			memset(&lp[nlr], 0, NLIN * sizeof(char*)); //Обнулить новую часть массива указателе
		}
		// заказать в памяти место для строки плюс один байт
		lp[nlr] = (char*)malloc(len + 1); //Сохранить строку в динамическом массиве
		b = 0;
		for (a = 0; a < nlr; a++) //ЗДЕСЬ ИДЕТ ПРОВЕРКА НА ДУБЛИКАТ ТЕКСТА
		{
			if (strcmp(line, lp[a]) == 0) //Сравнение 2 строк
			{
				printf("Есть похожие строки! Все строки, которые похожи, будут удалены!\n");
				b = 1;
				break;
			}
		}
		if (b == 0)
		{
			strcpy(lp[nlr], line);
			nlr++;
		}
	}

	for (a = 0; a < nlr; a++)
	{
		fputs(lp[a], fpout);
		printf("\nlineptr [%d]= %s", a, lp[a]);
	}
	nws = wcs(lines, nls);
	nwr = wcr(lp, nlr);
	printf("\nЧисло слов: %d\n", nws);
	printf("Число слов на вывод: %d\n", nwr);

	//Отказаться от памяти для строк
	for (a = 0; a < nlr; a++)
	{
		if (lp[a] != NULL)
			free(lp[a]);
	}
	//Отказаться от динамического массива указателей
	if (lp != NULL)
		free(lp);
	printf("Закрытие файлов...\n");
	fclose(fpin);
	fclose(fpout);
	return 0;
}

int wcs(char lines[][MAXL], int n)
{
	int i, j;
	int c; //Счётчик слов
	int fl; //Признак слова
	c = 0;
	fl = NO;
	for (i = 0; i < n; i++)	//Цикл по строкам
	{
		char* ptr = &lines[i][0]; //Указатель на текущую строку
		for (j = 0; ptr[j] != '\0'; j++)
		{
			if (ptr[j] == ' ' || ptr[j] == ',' || ptr[j] == '.' || ptr[j] == '\n') //Текущий символ - разделитель
				fl = NO;
			else //Текущий символ - буква
			{
				if (fl == NO) //Первая буква в слове
					c++;
				fl = YES;
			}
		}
		if (fl == YES) //Строка закончилась буквой
			c++;
	}
	return c;
}

int wcr(char** lines, int n)
{
	int i, j;
	int c; //Счётчик слов
	int fl; //Признак слова
	c = 0;
	fl = NO;
	for (i = 0; i < n; i++)
	{
		//Цикл по строкам получить адрес текущей строки
#if 1
		char* ptr = *(lines + i); //Указатель на текущую строку
#else
		char* ptr = lines[i]; //Указатель на текущую строку

#endif
		for (j = 0; ptr[j] != '\0'; j++)
		{
			if (ptr[j] == ' ' || ptr[j] == ',' || ptr[j] == '.' || ptr[j] == '\n') //Текущий символ - разделитель
				fl = NO;
			else //Текущий символ - буква
			{
				if (fl == NO) //Первая буква в слове
					c++;
				fl = YES;
			}
		}
	}
	return c;
}
