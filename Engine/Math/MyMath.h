#pragma once
#include <math.h>
#include<stdint.h>
#include<cassert>
#include <vector>
#include <string>
#include <cmath>
#define _USE_MATH_DEFINES
#include <algorithm>
const float PI = 3.1415926535f;
struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};
struct Vector3 {
	float x;
	float y;
	float z;
};
struct Vector2 {
	float x;
	float y;
};
struct Matrix4x4 final {
	float m[4][4];
};
struct Matrix3x3 {
	float m[3][3];
};
struct Transform
{
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};
struct Transformmatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};
struct DirectionalLightData {
	Vector4 color;//ライトの色
	Vector3 direction;//ライトの向き
	float intensity;//輝度
};

struct MaterialData {
	std::string textureFilePath;
};
struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};
struct AABB {
	Vector3 min;
	Vector3 max;
};

struct OBB {
	Vector3 center;
	Vector3 orientation[3];
	Vector3 size;
};
struct StructSphere {
	Vector3 center;
	float radius;
};

struct Quaternion {
	float w, x, y, z;
};

inline Vector3 operator-(const Vector3& v) {
	return { -v.x, -v.y, -v.z };
}
inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

inline Vector3 operator*(const Vector3& v, float s) {
	return { v.x * s, v.y * s, v.z * s };
}
inline Vector3 operator*(float s, const Vector3& v) {
	return { s * v.x, s * v.y, s * v.z };
}

inline Vector3 operator*(const Vector3& v, const Matrix4x4& m) {
	return {
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2] };
}
Matrix4x4 MakeRotateXMatrix(float theta);
Matrix4x4 MakeRotateYMatrix(float theta);

Matrix4x4 MakeRotateZMatrix(float theta);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
Matrix4x4 MakeQuatAffineMatrix(const Vector3& scale, const Matrix4x4& rotate, const Vector3& translate);
//1　行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
//２　行列の減法
Matrix4x4 Sub(const Matrix4x4& m1, const Matrix4x4& m2);
//３　行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
//４　逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
//５　転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
//６　単位行列
Matrix4x4 MakeIdentity4x4();
//透視投射行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
float cot(float theta);
// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
//ビューポート行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Vector3 Normalise(const Vector3& v);
Vector3 Add(const Vector3& a, const Vector3& b);


Vector3 Subtract(const Vector3& v1, const Vector3& v2);
Vector3 Multiply(float scalar, const Vector3& v);
float Dot(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v);
Vector3 vectorTransform(const Vector3& vector, const Matrix4x4& matrix);
Vector3 Slerp(float t, const Vector3& s, const Vector3& e);
Vector3 Lerp(float t, const Vector3& s, const Vector3& e);
float Lerp(float t, const float& s, const float& e);
Vector3 Distance(const Vector3& v1, const Vector3& v2);
Matrix4x4 MakeRotateMatrix(Vector3 theta);
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
Vector3 Cross(const Vector3& v1, const Vector3& v2);
/// obbの衝突判定
inline Matrix4x4& SetTranslate(Matrix4x4& m, const Vector3& v) {
	m.m[3][0] = v.x, m.m[3][1] = v.y, m.m[3][2] = v.z;
	return m;
}

inline Matrix4x4 MakeInverseMatrix(const Matrix4x4& rotate, const Vector3& translate) {
	Matrix4x4 RT = Transpose(rotate);
	return SetTranslate(RT, -translate * RT);
}
inline Matrix4x4 MakeRotateMatrixFromOrientations(const Vector3 orientations[3]) {
	return {
		orientations[0].x,orientations[0].y,orientations[0].z,0.0f,
		orientations[1].x,orientations[1].y,orientations[1].z,0.0f,
		orientations[2].x,orientations[2].y,orientations[2].z,0.0f,
		0.0f,0.0f,0.0f,1.0f };
}
inline bool IsCollision(const AABB& aabb, const StructSphere& sphere) {
	Vector3 clossestPoint{
		std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z)
	};

	float distance = Length(Subtract(clossestPoint, sphere.center));

	return distance <= sphere.radius;
}
inline bool IsCollision(const OBB& obb, const StructSphere& sphere) {
	Matrix4x4 obbWorldInverse = MakeInverseMatrix(MakeRotateMatrixFromOrientations(obb.orientation), obb.center);
	Vector3 centerInOBBLocalSpace = sphere.center * obbWorldInverse;
	AABB aabbOBBLocal{ .min = -obb.size, .max = obb.size };
	StructSphere sphereObbLocal{ centerInOBBLocalSpace, sphere.radius };

	return IsCollision(aabbOBBLocal, sphereObbLocal);
}
inline Vector3 GetXAxis(const Matrix4x4& m) {
	return { m.m[0][0],m.m[0][1],m.m[0][2] };
}
inline Vector3 GetYAxis(const Matrix4x4& m) {
	return { m.m[1][0],m.m[1][1],m.m[1][2] };
}inline Vector3 GetZAxis(const Matrix4x4& m) {
	return { m.m[2][0],m.m[2][1],m.m[2][2] };
}
inline void GetOrientations(const Matrix4x4& m, Vector3 orientations[3]) {
	orientations[0] = GetXAxis(m);
	orientations[1] = GetYAxis(m);
	orientations[2] = GetZAxis(m);
}
inline Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotate) {
	Vector3 s = { std::sin(rotate.x), std::sin(rotate.y), std::sin(rotate.z) };
	Vector3 c = { std::cos(rotate.x), std::cos(rotate.y), std::cos(rotate.z) };
	return {
		c.y * c.z,						c.y * s.z,						-s.y,		0.0f,
		s.x * s.y * c.z - c.x * s.z,	s.x * s.y * s.z + c.x * c.z,	s.x * c.y,	0.0f,
		c.x * s.y * c.z + s.x * s.z,	c.x * s.y * s.z - s.x * c.z,	c.x * c.y,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f };
}
inline Vector3 Reflect(const Vector3& input, const Vector3& normal) {
	Vector3 r;
	r = input - 2 * Dot(input, normal) * normal;
	r = r * 0.8f;
	return r;
}
inline Vector4 MakeQuaternion(Vector3 axis, float radian) {
	Vector4 quaternion;
	float halfSin, halfCos;      //動かす角度の半分のsin,cos
	float normal;

	quaternion = { 0,0,0,0 };
	// 回転軸の長さを求める
	//λ2x+λ2y+λ2z=1方向が重要だからノルムを１に統一
	normal = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
	if (normal <= 0.0f) return quaternion;

	// 方向ベクトルへ（単位ベクトル：長さは1）
	//ノルムは１という決まり事
	//sqrtfは平方根
	normal = 1.0f / sqrtf(normal);
	axis.x = axis.x * normal;
	axis.y = axis.y * normal;
	axis.z = axis.z * normal;

	//四次元ベクトル (λ.x*sinθ/2,λ.y*sinθ/2,λ.z*sinθ/2,cosθ/2)
	halfSin = sinf(radian * 0.5f);
	halfCos = cosf(radian * 0.5f);

	quaternion.w = halfCos;
	quaternion.x = axis.x * halfSin;
	quaternion.y = axis.y * halfSin;
	quaternion.z = axis.z * halfSin;

	return quaternion;
}
Quaternion createQuaternion(float Radian, Vector3 axis);

// クォータニオンからオイラー角への変換
Vector3 quaternionToEulerAngles(const Quaternion& quat);

inline Matrix4x4 quaternionToMatrix(const Quaternion& quat) {
	Matrix4x4 result;
	float xx = quat.x * quat.x;
	float xy = quat.x * quat.y;
	float xz = quat.x * quat.z;
	float xw = quat.x * quat.w;

	float yy = quat.y * quat.y;
	float yz = quat.y * quat.z;
	float yw = quat.y * quat.w;

	float zz = quat.z * quat.z;
	float zw = quat.z * quat.w;

	result.m[0][0] = 1.0f - 2.0f * (yy + zz);
	result.m[0][1] = 2.0f * (xy - zw);
	result.m[0][2] = 2.0f * (xz + yw);
	result.m[0][3] = 0.0f;
	
	result.m[1][0] = 2.0f * (xy + zw);
	result.m[1][1] = 1.0f - 2.0f * (xx + zz);
	result.m[1][2] = 2.0f * (yz - xw);
	result.m[1][3] = 0.0f;
	
	result.m[2][0] = 2.0f * (xz - yw);
	result.m[2][1] = 2.0f * (yz + xw);
	result.m[2][2] = 1.0f - 2.0f * (xx + yy);
	result.m[2][3] = 0.0f;
	
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}
inline Vector3 matrixToEulerAngles(const Matrix4x4 mat) {
	float pitch;
	float yaw;
	float roll;
	pitch = asin(-mat.m[2][0]); // ピッチ

	if (cos(pitch) != 0.0f) {
		roll = atan2(mat.m[2][1] / cos(pitch), mat.m[2][2] / cos(pitch)); // ロール
		yaw = atan2(mat.m[1][0] / cos(pitch), mat.m[0][0] / cos(pitch));   // ヨー
	}
	else {
		// ジンバルロックの場合
		yaw = 0.0f;
		roll = atan2(-mat.m[0][1], mat.m[1][1]);
	}

	//// ラジアンから度に変換
	//yaw = yaw * 180.0f / PI;
	//pitch = pitch * 180.0f / PI;
	//roll = roll * 180.0f / PI;
	return{ yaw,pitch,roll };
}
inline float LengthQuaternion(const Quaternion& q) {
	return std::sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}
inline Quaternion Normalize(const Quaternion& q) {
	float len = LengthQuaternion(q);
	Quaternion result;
	if (len != 0.0f) {
		result.w = q.w / len;
		result.x = q.x / len;
		result.y = q.y / len;
		result.z = q.z / len;
		return result;
	}
	else {
		return q;
	}
}
inline Vector3 extractEulerAnglesFromRotationMatrix(const Matrix4x4& rotationMatrix) {
	Vector3 eulerAngle;

	// 回転行列の各要素を抽出
	float m00 = rotationMatrix.m[0][0];
	float m01 = rotationMatrix.m[0][1];
	float m02 = rotationMatrix.m[0][2];
	float m10 = rotationMatrix.m[1][0];
	float m11 = rotationMatrix.m[1][1];
	float m12 = rotationMatrix.m[1][2];
	float m20 = rotationMatrix.m[2][0];
	float m21 = rotationMatrix.m[2][1];
	float m22 = rotationMatrix.m[2][2];

	// Yaw（ヨー）角度の計算
	eulerAngle.x = atan2(m01, m00);

	// Pitch（ピッチ）角度の計算
	eulerAngle.y = atan2(-m02, sqrt(m12 * m12 + m22 * m22));

	// Roll（ロール）角度の計算
	eulerAngle.z = atan2(m12, m22);

	return eulerAngle;
}
