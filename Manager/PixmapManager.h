#ifndef PIXMAPMANAGER_H
#define PIXMAPMANAGER_H

#include<qpixmap.h>
#include<qlist.h>
#include<qvariant.h>
#define MAX_SIZE 50


struct Pixmap
{
	QString _name;
	QPixmap _pixmap;
	Pixmap* _next=0;
	~Pixmap()
	{
		if (_next)
		{
			delete _next;
		}
	}
	int width()const { return _pixmap.width(); }
	int height()const { return _pixmap.height(); }
};

class PixmapManager
{
private:
	PixmapManager();
	~PixmapManager();
public:
	static PixmapManager* instance();
	static void destroy();
	Pixmap* loadPixmap(const QString&name);
	Pixmap* createEmptyPixmap() { return &m_empty; }
protected:
	Pixmap m_empty;
	Pixmap* m_head;
	Pixmap* m_last;
	int m_size;
	static PixmapManager* m_instance;
};
#endif // !PIXMAPMANAGER_H