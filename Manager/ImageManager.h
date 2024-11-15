#pragma once
#include"SingleModel.hpp"
#include"ConfigInfo.h"
#include<QPixmap>

#define MAX_PIXMAP_COUNT 24
#define MAX_EXPLODE_COUNT 32


#define LOAD_IMAGE(filename) hjdd::manager::ImageManager::instance()->loadImage(filename)
#define RELEASE_IMAGE(item) hjdd::manager::ImageManager::instance()->releseImage(item)

#define IV_ICON manager::ImageManager::instance()->ivIcon

namespace hjdd
{
	namespace manager
	{
		//imgpath: img/  filename:picture/*.png
		struct MANAGER_EXPORT Pixmap
		{
			QPixmap _pixmap;
			QString _filename;
			Pixmap* _next;
			Pixmap* _back;
			int _count;

			Pixmap(const QString& filename)
				:_pixmap(ConfigInfo::instance()->imgPath()+filename),
				_filename(filename),
				_next(0),
				_back(0),
				_count(0)
			{}
		};


		class MANAGER_EXPORT ImageManager :public SingleModel<ImageManager>
		{
			friend SingleModel<ImageManager>;
		public:
			virtual ~ImageManager();



			//加载图片
			// 超过MAX_PIXMAP_COUNT则会替换引用次数为0的图片
			// 超过MAX_EXPLODE_COUNT则会删除引用次数为0的图片
			//filename: [picture|...]/*.png
			Pixmap* loadImage(const QString& filename);
			void releseImage(Pixmap*node);



			Pixmap* contain(const QString&filename);
			void move_head(Pixmap*item);
			void move_last(Pixmap* item);

			Pixmap* addNode(const QString&filename);

			QPixmap ivIcon;
		protected:
			ImageManager();
		private:
			Pixmap* m_head;
			Pixmap* m_last;
			int m_size;
		};
	}
}