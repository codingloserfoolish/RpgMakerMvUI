#pragma once
#include<QGraphicsItem>
#include<QGraphicsSceneMouseEvent>
#include<QPainter>
#include<QStyleOptionGraphicsItem>
#include<QDomDocument>
#include"../fasttools/fasttools.h"
#include"RpgObjectType.h"
#include"config.h"

#define GraphicsAnchor_HalfSquareSize 4
#define GraphicsAnchor_SquareSize (GraphicsAnchor_HalfSquareSize*2)

//1/32
#define ANGLE_RANGE 0.03125

#define MoveCtrStatue_CONTENT	0x00
#define MoveCtrStatue_BOTTOM	0x02
#define MoveCtrStatue_RIGHT		0x04
#define MoveCtrStatue_LEFT		0x08
#define MoveCtrStatue_TOP		0x10
#define MoveCtrStatue_ANGLE_BAR 0x11
class QStandardItem;
namespace hjdd
{
	namespace widget { class ObjectEditor; }
	namespace item
	{
		/*
		* 必要的重载函数
		* //背景绘制
		* protected:virtual void drawBackground(QPainter* painter, const QStyleOptionGraphicsItem* option);
		* //类型区别
		* public:virtual int rpgobject_type()const;
		* //编辑窗口
		* public:virtual widget::ObjectEditor* editor();
		* //xml自定义属性
		* public:virtual QDomElement OthersProperties(QDomDocument&dom);
		* public:virtual void OthersPropertiesLoad(QDomElement&node);
		* 
		* 导出的必要函数
		* GraphicsItem * create();
		* int rpgobject_type();
		* QString Js_initProperties(QDomElement & properties, QString objectName);
		*/
		class ITEMEXPORT GraphicsItem :public QObject, public QGraphicsItem
		{
			Q_OBJECT
		public:
			GraphicsItem(QGraphicsItem* parent = nullptr);
			GraphicsItem(int width,int height,QGraphicsItem* parent = nullptr);
			virtual ~GraphicsItem();

			bool findChild(GraphicsItem*item);

			//物体被激活后选择该区用来拖动物体，该区域外的地方用来拉伸
			QRectF ContentSelectedRect()const;
			//绘图区域
			QRectF ContentPaintRect() const;
			//锚点区域
			QRectF ContentAnchorRect()const;


			virtual QRectF boundingRect() const;
			virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr);

			//添加子物体
			void addChild(GraphicsItem* item);

			//圆域,刷新角度图用 范围(MIN_RADIUS-MAX_RADIUS)
			int radius()const 
			{ 
				return (m_width > m_height ? m_height: m_width)/2;
			}

			void active() { this->m_isActive = true; }
			void deactive() { this->m_isActive = false; }
			bool isActive()const { return m_isActive; }

			void setHeight(int height) { m_height = height; }
			void setWidth(int width) { m_width = width; }
			int width()const { return m_width; }
			int height()const { return m_height; }


			virtual int rpgobject_type()const { return 0; }
			//编辑窗口
			virtual widget::ObjectEditor* editor() { return 0; }
			virtual QDomElement OthersProperties(QDomDocument& dom) { return QDomElement(); };
			virtual void OthersPropertiesLoad(QDomElement& node){}

			QStandardItem* standardItem;
		signals:
			void positionChanged(const QPointF);
			void angleChanged(const qreal);
			void sizeChanged(const QRectF);
		protected:
			//绘制前景图，物体锚点等
			virtual void drawForeground(QPainter* painter, const QStyleOptionGraphicsItem* option);
			//绘制背景图，物体内容
			virtual void drawBackground(QPainter* painter, const QStyleOptionGraphicsItem* option);
			virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;
			virtual void mousePressEvent(QGraphicsSceneMouseEvent* event)override;

		private:
			//绘制矩形锚点(前景)
			void drawAnchor(QPainter*painter, const QStyleOptionGraphicsItem* option);
			//绘制角度图
			void drawAngle(QPainter* painter, const QStyleOptionGraphicsItem* option);


			bool m_isActive;

			quint8 m_moveCtrlStatue;

			qreal m_width;
			qreal m_height;


			//鼠标点击时，鼠标在被选中物体父坐标下的坐标值与该物体坐标的差
			static QPointF _local_mouse_diff;
		public:
			//所有物体只有一个能被选中
			static GraphicsItem* _selected_item;
		};
	}
}