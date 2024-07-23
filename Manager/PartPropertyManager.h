#ifndef PARTPROPERTYMANAGER_H_
#define PARTPROPERTYMANAGER_H_
#include<qmetaobject.h>
#include"../QtPropertyBrowser/qtvariantproperty.h"
#include<vector>

struct MetaPropertyPair
{
	const QMetaObject* _meta_ptr=0;
	QtProperty* _groupProperty=0;
	std::vector<QtVariantProperty*> _variant_properties;

	MetaPropertyPair* _next=0;
};

class PartPropertyManager
{
public:
	PartPropertyManager(const PartPropertyManager&) = delete;
	PartPropertyManager& operator=(const PartPropertyManager&) = delete;
	~PartPropertyManager()
	{
		if (!m_head)return;
		MetaPropertyPair* item = m_head->_next;
		MetaPropertyPair* before = m_head;
		while (item)
		{
			delete before;
			before = item;
			item = item->_next;
		}
		delete before;
	}

	static PartPropertyManager* instance() { if (!m_instance) { m_instance = new PartPropertyManager; }return m_instance; }
	static void destroy() { if (m_instance)delete m_instance; }
	void setManager(QtVariantPropertyManager* manager) { m_manager = manager; }


	/*MainWindow下m_propertyWidget直接获取数据*/
	QtProperty* property_data(QObject* obj);
	MetaPropertyPair* contain(const QMetaObject* metaobj);

private:
	PartPropertyManager():m_manager(0),m_result(0),m_head(0),m_last(0){}
	QtProperty* property_data_imp(QObject*,const QMetaObject*);
	MetaPropertyPair* append_metaProperty(QObject*, const QMetaObject*);
	void update_metaProperty(QObject*, const QMetaObject*, MetaPropertyPair*);
	static PartPropertyManager* m_instance;


	QtVariantPropertyManager* m_manager;

	QtProperty* m_result;

	MetaPropertyPair* m_head;
	MetaPropertyPair* m_last;
};

#endif // !PARTPROPERTYMANAGER_H_
