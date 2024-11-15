#pragma once
#include"../item/ObjectEditor.h"
#include<QPushButton>
#include<QFileDialog>
#include"../manager/ConfigInfo.h"

#ifdef DLL_SPRITE_EPORT
#define SPRITE_EXPORT Q_DECL_EXPORT
#else
#define SPRITE_EXPORT Q_DECL_IMPORT
#endif // DLL_SPRITE_EPORT

namespace hjdd
{
	namespace widget
	{
		//裁剪图片编辑窗口
		class SPRITE_EXPORT SpriteClipWidget :public QWidget
		{
			Q_OBJECT
		public:
			SpriteClipWidget(QWidget* parent = nullptr);
			virtual~SpriteClipWidget();
			void setItem(item::GraphicsItem*item);
			//该窗口下控制裁剪矩形大小
			QRect ClipRectControlRect()const;
		protected:
			virtual void paintEvent(QPaintEvent* e)override;
			virtual void mousePressEvent(QMouseEvent* event)override;
			virtual void mouseMoveEvent(QMouseEvent* event)override;

		private:
			void drawAnchor(QPainter* paniter,const QRect rect);
			item::GraphicsItem* m_sprite;
			quint8 m_moveCtrlStatue;
			bool m_active;
			QPoint m_diff;
		};
		class SPRITE_EXPORT SpriteEditor :public ObjectEditor
		{
			Q_OBJECT
		public:
			SpriteEditor(QWidget*parent=nullptr);
			virtual~SpriteEditor();
		public slots:
			void SelectPixmap();
		protected:
			virtual void initLayOut(QVBoxLayout* vlayout)override;
			virtual void setProperties(item::GraphicsItem* item)override;
			virtual void Connection()override;
			virtual void DisConnection()override;
		private:
			QLineEdit* m_pixmap_edit;
			SpriteClipWidget* m_clipWidget;
		};
	}
}