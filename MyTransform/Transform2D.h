#ifndef TRANSFORM2D_H_
#define TRANSFORM2D_H_
#include<qobject.h>
#include<qstring.h>
#include<qpainter.h>
#include"Vector3.h"
#define PI 3.141593

class Transform2D:public QObject
{
	Q_OBJECT
	Q_PROPERTY(qreal x READ R_X_ WRITE S_X_)
	Q_PROPERTY(qreal y READ R_Y_ WRITE S_Y_)
	Q_PROPERTY(qreal angle READ R_ANGLE_ WRITE S_ANGLE_)
public:
	Q_INVOKABLE qreal R_X_()const{ return m_x; }
	Q_INVOKABLE qreal R_Y_()const { return m_y; }
	Q_INVOKABLE qreal R_ANGLE_()const { return m_angle; }
	Q_INVOKABLE void S_X_(qreal value) { m_x = value; }
	Q_INVOKABLE void S_Y_(qreal value) { m_y = value; }
	Q_INVOKABLE void S_ANGLE_(qreal value) { m_angle = value; }

	explicit Transform2D(QObject* parent = nullptr);
	void rotate(qreal angle);
	void translate(float dx, float dy);
	QPointF pos() { return QPointF(m_x, m_y); }

	Matrix3 getTransMatrixIn();
	Matrix3 getTransMatrixOut();

	QPointF transform_point(float x, float y);
	void transform_painter(QPainter& p);
protected:
	float m_x;
	float m_y;
	float m_angle;
};

#endif // !TRANSFORM2D_H_
