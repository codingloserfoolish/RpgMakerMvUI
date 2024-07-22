#include"Vector3.h"

Matrix3 Mat_mutiply(Matrix3& left, Matrix3& right)
{
	Matrix3 result;
	result._11 = left._11 * right._11 + left._12 * right._21 + left._13 * right._31;
	result._12= left._11 * right._12 + left._12 * right._22 + left._13 * right._32;
	result._13 = left._11 * right._13 + left._12 * right._23 + left._13 * right._33;

	result._21 = left._21 * right._11 + left._22 * right._21 + left._23 * right._31;
	result._22 = left._21 * right._12 + left._22 * right._22 + left._23 * right._32;
	result._23 = left._21 * right._13 + left._22 * right._23 + left._23 * right._33;

	result._31 = left._31 * right._11 + left._32 * right._21 + left._33 * right._31;
	result._32 = left._31 * right._12 + left._32 * right._22 + left._33 * right._32;
	result._33 = left._31 * right._13 + left._32 * right._23 + left._33 * right._33;
	return result;
}

Vector3 Mat_Vec_mutiply(Matrix3& left, Vector3& right)
{
	Vector3 result;
	result._1 = left._11 * right._1 + left._12 * right._2 + left._13 * right._3;
	result._2 = left._21 * right._1 + left._22 * right._2 + left._23 * right._3;
	result._3 = left._31 * right._1 + left._32 * right._2 + left._33 * right._3;
	return result;
}
