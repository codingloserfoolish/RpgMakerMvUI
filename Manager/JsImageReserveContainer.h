#ifndef JSIMAGERESERVECONTAINER_H_
#define JSIMAGERESERVECONTAINER_H_
#include<qtextstream.h>
#include<qstringlist.h>
struct SplitImgFloder
{
	bool _exist_ref;
	QString _orginal_Str;
	QString _floder;
	QString _filename;
	SplitImgFloder* _next;
	SplitImgFloder* _back;
	SplitImgFloder(QString&str);
};

class SplitFloderList
{
public:
	SplitFloderList() { m_current = 0; m_head = 0; m_last = 0; }
	~SplitFloderList();
	SplitImgFloder* addFloder(QString& str);
	void declineAll();
	void clearFalse();
	void reset() { m_current = m_head; }
	void move_next() { m_current = m_current->_next; }
	SplitImgFloder* current() { return m_current; }
private:
	SplitImgFloder* m_current;
	SplitImgFloder* m_head;
	SplitImgFloder* m_last;
};

class JsImageReserveContainer
{
public:
	JsImageReserveContainer(const JsImageReserveContainer&) = delete;
	JsImageReserveContainer& operator=(const JsImageReserveContainer&) = delete;
	static JsImageReserveContainer* instance() { if (!m_instance)m_instance = new JsImageReserveContainer; return m_instance; }
	static void destroy() { if (m_instance)delete m_instance; }

	void declineAll() { m_floder_list.declineAll(); }
	SplitImgFloder* addFloder(QString&floder) { return m_floder_list.addFloder(floder); }
	void removeFalseFloders() { m_floder_list.clearFalse(); }
	void JsPreReserveBitmapWriter(QTextStream& stream);
private:
	JsImageReserveContainer() {}
	static JsImageReserveContainer* m_instance;
	SplitFloderList m_floder_list;
};
#endif // !JSIMAGERESERVECONTAINER_H_
