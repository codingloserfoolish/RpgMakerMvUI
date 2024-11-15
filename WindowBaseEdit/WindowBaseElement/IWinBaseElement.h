#pragma once
#include<QPainter>
#include<QMouseEvent>
#include<QWidget>
#include<QDomDocument>
#include<qglobal.h>
#ifdef DLLIWINBASEELEMENTEXPORT
#define IWINBASEELEMENTEXPORT Q_DECL_EXPORT
#else
#define IWINBASEELEMENTEXPORT Q_DECL_IMPORT
#endif // IWINBASEELEMENTEXPORT

#define MOUSE_PRESS   0
#define MOUSE_RELEASE 1
#define MOUSE_MOVE    2

#define SQUARE_HALF_SIZE 4
#define SQUARE_SIZE SQUARE_HALF_SIZE*2
#define MIN_RECT SQUARE_SIZE*4

#define CONTENT_REGION			0x00
#define ANCHOR_REGION_LEFT		0x01
#define ANCHOR_REGION_TOP		0x02
#define ANCHOR_REGION_RIGHT		0x04
#define ANCHOR_REGION_BOTTOM	0x08

namespace hjdd
{
	namespace elements
	{
		class IWINBASEELEMENTEXPORT IWinBaseElement
		{
		public:
			//��ͼ��
			virtual void paint(QPainter* painter)=0;
			virtual QSize size()const = 0;
			QPoint pos()const { return QPoint(_x,_y); }
			void setPos(int x, int y) { _x = x; _y = y; }

			void mouseEvent(QMouseEvent*e,int type);

		protected:
			/*
			* �¼����ϲ㴰�ڹ���
			*/
			virtual void mousePressEvent(const QPoint& pos);
			virtual void mouseMoveEvent(const QPoint& pos);
			virtual void mouseReleaseEvent(const QPoint& pos);
			int _x=0;
			int _y=0;
		};

		class ElementEdit;

		/*Js�нṹ
		* ClassName(x,y,...)
		* ֻ��x,y���Ա�ȷ��
		* �ڹ���xml��ʱ����������һ����Ϊ��������
		* 
		* 
		* ����д�ĺ���
		* virtual void drawBackground(QPainter* painter);
		* virtual int element_type()const;
		* virtual ElementEdit* editor();
		* virtual QDomElement xmlSave(QDomDocument&dom);
		* virtual void xmlLoad(QDomElement&selfPrototype);
		*/
		class IWINBASEELEMENTEXPORT Element :public IWinBaseElement
		{
		public:
			Element(int w= MIN_RECT,int h= MIN_RECT);
			virtual~Element() {};

			bool isActive()const { return m_active; }
			void activate() { m_active = true; }
			void deactive() { m_active = false; }

			//��ͼ��
			virtual void paint(QPainter* painter);
			virtual QSize size()const;
			//�༭���ڵ���
			virtual int element_type()const { return -1; }
			virtual ElementEdit* editor() { return 0; }
			//xml-Js���
			/*
			* ����Ҫ���x,y������WindowBase���
			*/
			virtual QDomElement xmlSave(QDomDocument&dom) { return QDomElement(); }
			virtual void xmlLoad(QDomElement&selfPrototype){}

			//�������
			QRect Rect()const;
			QRect CompleteRect()const;
			QRect ContentRect()const;
			

			static Element* _selected_item;
		protected:
			virtual void drawForeground(QPainter*painter);
			virtual void drawBackground(QPainter* painter);

			virtual void mousePressEvent(const QPoint& pos);
			virtual void mouseMoveEvent(const QPoint& pos);
			virtual void mouseReleaseEvent(const QPoint& pos);


			void drawAnchor(QPainter* painter);
			
			static QPoint _diff;
			static unsigned char _clicked_region;

			int m_width;
			int m_height;
			bool m_active;

		private:
			bool isContentRegion(const QPoint&pos);
		};

		class IWINBASEELEMENTEXPORT ElementEdit:public QWidget
		{
			Q_OBJECT
		public:
			ElementEdit(QWidget* parent = nullptr);
			virtual~ElementEdit(){}
			void setElement(Element* ele);
			Element* element()const { return m_element; }
			void Connection();
			void DisConnection();
		signals:
			void propertiesChanged();
		protected:
			virtual void setProperties(Element*);
			virtual void OthersConnect(Element*);
			virtual void OthersDisConnect(Element*);
		private:
			Element* m_element;
		};
	}
}