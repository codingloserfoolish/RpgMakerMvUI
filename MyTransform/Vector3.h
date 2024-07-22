#ifndef VECTOR3_H_
#define VECTOR3_H_

struct Vector3
{
	float _1, _2, _3;
};

struct Matrix3
{
	float _11, _12, _13;
	float _21, _22, _23;
	float _31, _32, _33;
};

Matrix3 Mat_mutiply(Matrix3&left, Matrix3&right);
Vector3 Mat_Vec_mutiply(Matrix3& left, Vector3& right);


#endif // !VECTOR3_H_
