#pragma once
#include<QWidget>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QLabel>
#include<QLineEdit>
#include<QRegExpValidator>
#include"config.h"
namespace hjdd
{
	namespace item{class GraphicsItem;}
	namespace widget
	{

		/*
		* ┌── Transform──┐
		* │    x  y  angle   │
		* ├──   Size   ──┤
		* │  width  height   │
		* └─────────┘
		* 
		* 必要的重写函数
		* protected:
		*	virtual void initLayOut(QVBoxLayout*vlayout) {};
		*	virtual void setProperties(item::GraphicsItem* item) {};
		*	virtual void Connection() {};
		*	virtual void DisConnection() {};
		*/
		class ITEMEXPORT ObjectEditor:public QWidget
		{
			Q_OBJECT
		public:
			ObjectEditor(QWidget* parent = nullptr);
			virtual~ObjectEditor() {  };
			void setObject(item::GraphicsItem* item);
			item::GraphicsItem* item()const { return m_owner; }
			void setBaseProperties();

			void initUI();
			void initConnection();
			void releaseConnection();


		public slots:
			void slot_positionChanged(const QPointF pos);
			void slot_angleChanged(const qreal angle);
			void slot_sizeChanged(const QRectF rect);
			
		protected:
			//基础属性下添加自定义控件
			virtual void initLayOut(QVBoxLayout*vlayout);
			//自定义属性
			virtual void setProperties(item::GraphicsItem* item);
			//关于自定义属性与自定义控件的连接
			virtual void Connection();
			//断开相关自定义属性与控件的连接
			virtual void DisConnection();
		private:
			item::GraphicsItem* m_owner;

			QLineEdit* m_positionX_edit;
			QLineEdit* m_positionY_edit;
			QLineEdit* m_angle_edit;

			QLineEdit* m_width_edit;
			QLineEdit* m_height_edit;
		};
	}
}