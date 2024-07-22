#include"PartPropertyManager.h"
#include<qdebug.h>

PartPropertyManager* PartPropertyManager::m_instance=0;

QtProperty* PartPropertyManager::property_data(QObject* obj)
{
	m_result = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(),"");
	QtProperty* sub_result = property_data_imp(obj, obj->metaObject());
	if (sub_result)m_result->addSubProperty(sub_result);
	return m_result;
}

MetaPropertyPair* PartPropertyManager::contain(const QMetaObject* metaobj)
{
	MetaPropertyPair* temp = m_head;
	while (temp)
	{
		if (temp->_meta_ptr == metaobj)break;;
		temp = temp->_next;
	}
	return temp;
}

QtProperty* PartPropertyManager::property_data_imp(QObject*obj, const QMetaObject*metaobj)
{
	if (metaobj)
	{
		QtProperty*sub_result=property_data_imp(obj, metaobj->superClass());
		if (sub_result)m_result->addSubProperty(sub_result);
	}
	else
	{
		return 0;
	}

	MetaPropertyPair* meta_data = this->contain(metaobj);
	if (!meta_data)
	{
		meta_data=this->append_metaProperty(obj, metaobj);
	}
	else
	{
		this->update_metaProperty(obj,metaobj,meta_data);
	}

	return meta_data->_groupProperty;
}

MetaPropertyPair* PartPropertyManager::append_metaProperty(QObject*obj, const QMetaObject*metaobj)
{
	MetaPropertyPair* item = new MetaPropertyPair;
	item->_meta_ptr = metaobj;
	item->_groupProperty = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(), metaobj->className());
	QtVariantProperty* variant_property = 0;
	QMetaProperty meta_property;
	for (int idx = metaobj->propertyOffset(); idx < metaobj->propertyCount(); ++idx)
	{
		meta_property = metaobj->property(idx);
		variant_property = m_manager->addProperty(meta_property.userType(), meta_property.name());
		variant_property->setValue(meta_property.read(obj));
		item->_groupProperty->addSubProperty(variant_property);
		item->_variant_properties.push_back(variant_property);
	}
	if (!m_head) { m_head = item; m_last = item; }
	else { m_last->_next = item; m_last = item; }

	return item;
}

void PartPropertyManager::update_metaProperty(QObject*obj, const QMetaObject* metaobj, MetaPropertyPair* metaproperty)
{
	for (int idx = 0; idx < metaproperty->_variant_properties.size(); ++idx)
	{
		metaproperty->_variant_properties[idx]->setValue(metaobj->property(idx + metaobj->propertyOffset()).read(obj));
	}
}
