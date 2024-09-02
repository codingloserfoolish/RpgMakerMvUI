#include"BoringFunction.h"
#include<string.h>
int StringLength(const char*str)
{
	int len = 0;
	while (str[len++]);
	return len-1;
}
char* IntToString(int num, int* count)
{
	int deal = num;
	char* memForInt = new char[12];
	memset(memForInt, 0, 12);
	if (num < 0) {
		memForInt[0] = '-'; 
		deal = -deal;
	}
	int index = 10;
	while (deal)
	{
		memForInt[index--] = (deal % 10 + '0');
		deal /= 10;
	}
	int start = 0;
	int clear = 0;
	++index;
	if (memForInt[0])
	{
		if (index == 0)return memForInt;
		start = 1;
	}
	clear = 11 - index;
	*count = clear;
	memcpy(memForInt + start, memForInt + index, clear);
	if (clear < index)clear = index;
	memset(memForInt + clear, 0, 11 - clear);
	return memForInt;
}

int StringToInt(const char*str)
{
	if (!str)return 0;
	int res = 0;
	int start = 0;
	if (str[0] == '-')start = 1;
	for (;str[start]; ++start)
	{
		res *= 10;
		res += (str[start] - '0');
	}
	if (str[0] == '-')return -res;
	return res;
}

char** split(const char*str,char symbol, int* count)
{
	int start = 0;
	int end = 0;
	int length = 0;
	int size = 0;
	int capacity = 5;
	char** strList=new char*[capacity];
	while (str[end])
	{
		if (str[end] == symbol&& end != start)
		{
			if (size == capacity)
			{
				capacity *= 2;
				char** temp = new char* [capacity];
				memcpy(temp, strList, size*sizeof(char*));
				delete[]strList;
				strList = temp;
			}
			length = end - start;
			strList[size] = new char[length +1];
			memcpy(strList[size], str + start, length);
			strList[size][length] = 0;
			start = end + 1;
			size++;
		}
		++end;
	}
	if (start != end)
	{
		if (size == capacity)
		{
			capacity *= 2;
			char** temp = new char* [capacity];
			memcpy(temp, strList, size * sizeof(char*));
			delete[]strList;
			strList = temp;
		}
		length = end - start;
		strList[size] = new char[length + 1];
		memcpy(strList[size], str + start, length);
		strList[size][length] = 0;
		size++;
	}

	(*count) = size;
	return strList;
}