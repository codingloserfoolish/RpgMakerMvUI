#include"Transform2D.h"
#include<cmath>
Transform2D::Transform2D(QObject* parent):QObject(parent),m_x(0),m_y(0),m_angle(0)
{

}

void Transform2D::rotate(qreal angle)
{
	m_angle += angle;
}

void Transform2D::translate(float dx, float dy)
{
	m_x += dx;
	m_y += dy;
}

Matrix3 Transform2D::getTransMatrixIn()
{
	Matrix3 mat;
	float cos = std::cosf(m_angle * PI / 180.0f);
	float sin = std::sinf(m_angle * PI / 180.0f);


	mat._11 = cos;
	mat._12 = sin;
	mat._13 = -m_x * cos-m_y*sin;

	mat._21 = -sin;
	mat._22 = cos;
	mat._23 = m_x * sin - m_y * cos;

	mat._31 = 0;
	mat._32 = 0;
	mat._33 = 1;

	return mat;
}

Matrix3 Transform2D::getTransMatrixOut()
{
	Matrix3 mat;
	float cos = std::cosf(m_angle * PI / 180.0f);
	float sin = std::sinf(m_angle * PI / 180.0f);


	mat._11 = cos;
	mat._12 = -sin;
	mat._13 = m_x;

	mat._21 = sin;
	mat._22 = cos;
	mat._23 = m_y;

	mat._31 = 0;
	mat._32 = 0;
	mat._33 = 1;

	return mat;
}

QPointF Transform2D::transform_point(float x, float y)
{
	float cos = std::cosf(m_angle * PI / 180.0f);
	float sin = std::sinf(m_angle * PI / 180.0f);
	return QPointF(
		(x - m_x)*cos+(y-m_y)*sin,
		-(x-m_x)*sin+(y-m_y)*cos
	);
}

void Transform2D::transform_painter(QPainter& p)
{
	p.translate(m_x, m_y);
	p.rotate(m_angle);
}
