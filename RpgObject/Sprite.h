#pragma once
#include"../item/GraphicsItem.h"
#include"../manager/ImageManager.h"
#include"SpriteEditor.h"

#ifdef DLL_SPRITE_EPORT
#define SPRITE_EXPORT Q_DECL_EXPORT
#else
#define SPRITE_EXPORT Q_DECL_IMPORT
#endif // DLL_SPRITE_EPORT

namespace hjdd
{
	namespace item
	{
		class SPRITE_EXPORT Sprite:public GraphicsItem
		{
		public:
			Sprite(QGraphicsItem* parent = nullptr);
			virtual~Sprite();
			QString filename()const
			{
				return m_sprite_src ? m_sprite_src->_filename : "";
			}
			virtual int rpgobject_type()const { return RPGOBJECT_SPRITE; }
			virtual widget::ObjectEditor* editor() { return new widget::SpriteEditor; }
			/*
			* <Image>
			*	<ClipRect>
			*		<Left></Left>
			*		<Top></Top>
			*		<Right></Right>
			*		<Bottom></Bottom>
			*	</ClipRect>
			*	<Path></Path>
			* </Image>
			*/
			virtual QDomElement OthersProperties(QDomDocument& dom);
			virtual void OthersPropertiesLoad(QDomElement& node);

			QRect& clipRect() { return m_clipRect; }
			manager::Pixmap* pixmap()const { return m_sprite_src; }
			void setImage(const QString&filename);
		protected:
			virtual void drawBackground(QPainter* painter, const QStyleOptionGraphicsItem* option);
		private:
			manager::Pixmap* m_sprite_src;
			//从原图裁剪一部分画出来
			QRect m_clipRect;
		};

		extern "C" SPRITE_EXPORT GraphicsItem * create();
		extern "C" SPRITE_EXPORT int rpgobject_type();
		extern "C" SPRITE_EXPORT QString Js_initProperties(QDomElement& properties,QString objectName);
	}
}

