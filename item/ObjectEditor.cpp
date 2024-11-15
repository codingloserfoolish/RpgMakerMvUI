#include"ObjectEditor.h"
#include"../item/GraphicsItem.h"
namespace hjdd
{
	namespace widget
	{
		ObjectEditor::ObjectEditor(QWidget* parent):
			QWidget(parent),
			m_owner(0)
		{
			this->setWindowTitle("编辑");
		}
		void ObjectEditor::setObject(item::GraphicsItem* item)
		{
			if (item == 0)
			{
				this->releaseConnection();
				m_owner = 0;
			}
			if (m_owner != item)
			{
				if (m_owner != 0)this->releaseConnection();
				m_owner = item;
				this->setBaseProperties();


				this->setProperties(item);
				this->initConnection();
			}
		}
		void ObjectEditor::setBaseProperties()
		{
			m_positionX_edit->setText(QString::number(m_owner->x(),'f',1));
			m_positionY_edit->setText(QString::number(m_owner->y(),'f',1));
			m_angle_edit->setText(QString::number(m_owner->rotation(),'f',1));
			m_width_edit->setText(QString::number(m_owner->width()));
			m_height_edit->setText(QString::number(m_owner->height()));
		}
		void ObjectEditor::initUI()
		{
			QRegExpValidator* validator = new QRegExpValidator(QRegExp("^(-?\\d+)(\\.\\d+)?$"));
			QVBoxLayout* main_layer = new QVBoxLayout(this);

			//Transform
			
			
			QHBoxLayout* transform_layer = new QHBoxLayout;
			main_layer->addLayout(transform_layer);


			QLabel* pos_x_label = new QLabel("x", this);
			m_positionX_edit = new QLineEdit(this);
			m_positionX_edit->setValidator(validator);
			QLabel* pos_y_label = new QLabel("y", this);
			m_positionY_edit = new QLineEdit(this);
			m_positionY_edit->setValidator(validator);
			QLabel* angle_label = new QLabel("angle", this);
			m_angle_edit = new QLineEdit(this);
			m_angle_edit->setValidator(validator);


			transform_layer->addWidget(pos_x_label);
			transform_layer->addWidget(m_positionX_edit);
			transform_layer->addWidget(pos_y_label);
			transform_layer->addWidget(m_positionY_edit);
			transform_layer->addWidget(angle_label);
			transform_layer->addWidget(m_angle_edit);

			//Size
			QHBoxLayout* size_layer = new QHBoxLayout;
			main_layer->addLayout(size_layer);

			QLabel* width_label = new QLabel("width", this);
			m_width_edit = new QLineEdit(this);
			m_width_edit->setValidator(validator);
			QLabel* height_label = new QLabel("height", this);
			m_height_edit = new QLineEdit(this);
			m_height_edit->setValidator(validator);

			size_layer->addWidget(width_label);
			size_layer->addWidget(m_width_edit);
			size_layer->addWidget(height_label);
			size_layer->addWidget(m_height_edit);

			this->initLayOut(main_layer);
		}
		void ObjectEditor::initConnection()
		{
			if (m_owner)
			{
				connect(m_owner, &item::GraphicsItem::positionChanged, this, &ObjectEditor::slot_positionChanged);
				connect(m_positionX_edit, &QLineEdit::textChanged, [this](const QString& value) {this->m_owner->setX(value.toDouble()); });
				connect(m_positionY_edit, &QLineEdit::textChanged, [this](const QString& value) {this->m_owner->setY(value.toDouble()); });

				connect(m_owner, &item::GraphicsItem::angleChanged, this, &ObjectEditor::slot_angleChanged);
				connect(m_angle_edit, &QLineEdit::textChanged, [this](const QString& value) {this->m_owner->setRotation(value.toDouble()); });

				connect(m_owner, &item::GraphicsItem::sizeChanged, this, &ObjectEditor::slot_sizeChanged);
				connect(m_width_edit, &QLineEdit::textChanged, [this](const QString& value) {this->m_owner->setWidth(value.toDouble()); this->m_owner->update(); });
				connect(m_height_edit, &QLineEdit::textChanged, [this](const QString& value) {this->m_owner->setHeight(value.toDouble()); this->m_owner->update();  });

				this->Connection();
			}
		}
		void ObjectEditor::releaseConnection()
		{
			if (m_owner)
			{
				disconnect(m_owner, 0, this, 0);
				disconnect(m_positionX_edit, 0, m_owner, 0);
				disconnect(m_positionY_edit, 0, m_owner, 0);
				disconnect(m_angle_edit, 0, m_owner, 0);
				disconnect(m_width_edit, 0, m_owner, 0);
				disconnect(m_height_edit, 0, m_owner, 0);

				this->DisConnection();
				m_owner = 0;
			}
		}
		void ObjectEditor::slot_positionChanged(const QPointF pos)
		{
			m_positionX_edit->blockSignals(true);
			m_positionX_edit->setText(QString::number(pos.x(),'f',1));
			m_positionX_edit->blockSignals(false);
			m_positionY_edit->blockSignals(true);
			m_positionY_edit->setText(QString::number(pos.y(),'f',1));
			m_positionY_edit->blockSignals(false);
		}
		void ObjectEditor::slot_angleChanged(const qreal angle)
		{
			m_angle_edit->blockSignals(true);
			m_angle_edit->setText(QString::number(angle,'f',1));
			m_angle_edit->blockSignals(false);
		}
		void ObjectEditor::slot_sizeChanged(const QRectF rect)
		{
			m_width_edit->blockSignals(true);
			m_width_edit->setText(QString::number(rect.width()));
			m_width_edit->blockSignals(false);
			m_height_edit->blockSignals(true);
			m_height_edit->setText(QString::number(rect.height()));
			m_height_edit->blockSignals(false);
		}
		void ObjectEditor::initLayOut(QVBoxLayout* vlayout)
		{
		}
		void ObjectEditor::setProperties(item::GraphicsItem* item)
		{
		}
		void ObjectEditor::Connection()
		{
		}
		void ObjectEditor::DisConnection()
		{
		}
	}
}