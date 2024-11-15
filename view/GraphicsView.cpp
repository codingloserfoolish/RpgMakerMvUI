#include"GraphicsView.h"
#include<QDebug>
namespace hjdd
{
	namespace view
	{
		GraphicsView::GraphicsView(QWidget* parent):
			QGraphicsView(parent)
		{
			this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			m_currentScene = new scene::GraphicsScene;
			m_currentScene->setSceneRect(-400, -300, 800, 600);
			this->setScene(m_currentScene);
			this->centerOn(0, 0);
		}
		GraphicsView::~GraphicsView()
		{
			
		}

		void GraphicsView::releaseScene()
		{
			if (m_currentScene)
			{
				delete m_currentScene;
			}
		}		
	}
}