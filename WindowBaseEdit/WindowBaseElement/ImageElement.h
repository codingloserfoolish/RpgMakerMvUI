#pragma once
#include"IWinBaseElement.h"
#include"../../manager/ImageManager.h"
#include<QLineEdit>
#include<QLabel>
#include<QPushButton>
#include<QFileDialog>

#ifdef DLLIMAGEELEMENTEXPORT
#define IMAGEELEMENTEXPORT Q_DECL_EXPORT
#else
#define IMAGEELEMENTEXPORT Q_DECL_IMPORT
#endif // DLLIMAGEELEMENTEXPORT

#define IMAGE_ELEMENT_TYPE 0x0002

#define CLICKED_CONTENT    0x00
#define CLICKED_TOP		   0x01
#define CLICKED_LEFT	   0x02
#define CLICKED_RIGHT	   0x04
#define CLICKED_BOTTOM	   0x08

namespace hjdd
{
	namespace elements
	{
		class ImageElement;
		//裁剪图片编辑窗口
		class IMAGEELEMENTEXPORT ImageElementClipWidget :public QWidget
		{
			Q_OBJECT
		public:
			ImageElementClipWidget(QWidget* parent = nullptr);
			virtual~ImageElementClipWidget();
			void setItem(ImageElement* item);
			//该窗口下控制裁剪矩形大小
			QRect ClipRectControlRect()const;
		signals:
			void clipChanged();
		protected:
			virtual void paintEvent(QPaintEvent* e)override;
			virtual void mousePressEvent(QMouseEvent* event)override;
			virtual void mouseMoveEvent(QMouseEvent* event)override;

		private:
			void drawAnchor(QPainter* paniter, const QRect rect);
			ImageElement* m_img_element;
			unsigned char m_clickedStatue;
			bool m_active;
			QPoint m_diff;
		};
		class IMAGEELEMENTEXPORT ImageElementEdit :public ElementEdit
		{
			Q_OBJECT
		public:
			ImageElementEdit(QWidget* parent = nullptr);
			virtual~ImageElementEdit();
		public slots:
			void SelectPixmap();
		protected:
			virtual void setProperties(Element*);
			virtual void OthersConnect(Element*);
			virtual void OthersDisConnect(Element*);
		private:
			QLineEdit* m_pixmap_edit;
			ImageElementClipWidget* m_clipWidget;
		};

		class IMAGEELEMENTEXPORT ImageElement:public Element
		{
		public:
			ImageElement();
			virtual~ImageElement();

			virtual int element_type()const { return IMAGE_ELEMENT_TYPE; }
			virtual ElementEdit* editor() { return new ImageElementEdit; }

			/*
			* <Image>
			*	<Size>
			*		<width></width>
			*		<height></height>
			*	</Size>
			*	<ClipRect>
			*		<x></x>
			*		<y></y>
			*		<width></width>
			*		<height></height>
			*	</ClipRect>
			*	<Path></Path>
			* </Image>
			*/
			virtual QDomElement xmlSave(QDomDocument& dom);
			virtual void xmlLoad(QDomElement& selfPrototype);


			QRect& clipRect() { return m_clipRect; }
			manager::Pixmap* pixmap()const { return m_sprite_src; }
			void setImage(const QString& filename);
		protected:
			virtual void drawBackground(QPainter* painter);
		private:
			manager::Pixmap* m_sprite_src;
			//从原图裁剪一部分画出来
			QRect m_clipRect;
		};

		extern "C" IMAGEELEMENTEXPORT Element * create();
		extern "C" IMAGEELEMENTEXPORT int element_type();
		extern "C" IMAGEELEMENTEXPORT QString Js_RestParams(QDomElement & properties);
	}
}