#ifndef LINETEXT_H_
#define LINETEXT_H_
#include<qstring.h>
#include<vector>
#include<qvariant.h>
struct LineText
{
	int _x;
	int _y;
	QString _text;
};

struct LineText_Data
{
	LineText* _data;
	LineText_Data():_data(0){}
	LineText_Data(LineText*data):_data(data){}
	LineText* operator->()const { return _data; }
};
Q_DECLARE_METATYPE(LineText_Data);
#define LINETEXT_ROLE Qt::UserRole+2

struct LineTextList_Data
{
	std::vector<LineText*>* _data;
	LineTextList_Data():_data(0){}
	LineTextList_Data(std::vector<LineText*>*data) :_data(data) {}
};
Q_DECLARE_METATYPE(LineTextList_Data);
#endif // !LINETEXT_H_
