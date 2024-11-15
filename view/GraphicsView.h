#pragma once
#include<QGraphicsView>
#include<QMap>
#include"../scene/GraphicsScene.h"
namespace hjdd
{
	namespace view
	{
		class GraphicsView :public QGraphicsView
		{
			Q_OBJECT
		public:
			GraphicsView(QWidget* parent = nullptr);
			virtual ~GraphicsView();
			void releaseScene();
		private:
			scene::GraphicsScene* m_currentScene;
			/*未来启用多场景
			* QMap<QString, scene::GraphicsScene*> m_scenes;
			*/
		};
	}
}
