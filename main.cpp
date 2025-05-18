#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include "imgui.h"

const char kWindowTitle[] = "LE2A_12_ホリ_ソウヘイ_タイトル";

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3 Normalize() const
	{
		float length = std::sqrtf(x * x + y * y + z * z);
		if (length == 0.0f)
		{
			return { 0,0,0 };
		}
		return { x / length,y / length,z / length };
	}

	static float Dot(const Vector3& a, const Vector3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

};

struct Matrix4x4
{
	float m[4][4];
};

struct Sphere
{
	Vector3 center;
	float radius;
	uint32_t color;
};

struct Line
{
	Vector3 origin;// 始点
	Vector3 diff;// 終点への差分ベクトル
};

struct Ray
{
	Vector3 origin;// 始点
	Vector3 diff;// 終点への差分ベクトル
};

struct Segment
{
	Vector3 origin;// 始点
	Vector3 diff;// 終点への差分ベクトル
};

struct Plane
{
	Vector3 normal;// 法線
	float distance;// 距離
};

bool isCollision(const Sphere& s1, const Sphere& s2)
{
	float dx = s2.center.x - s1.center.x;
	float dy = s2.center.y - s1.center.y;
	float dz = s2.center.z - s1.center.z;

	float radiusSum = s1.radius + s2.radius;

	return dx * dx + dy * dy + dz * dz <= radiusSum * radiusSum;
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label)
{
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row)
	{
		for (int column = 0; column < 4; ++column)
		{
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight + kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			result.m[row][col] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				result.m[row][col] += m1.m[row][k] * m2.m[k][col];
			}
		}
	}

	return result;
}

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m)
{
	Matrix4x4 result{};
	float inv[16], det;
	const float* src = &m.m[0][0];

	inv[0] = src[5] * src[10] * src[15] -
		src[5] * src[11] * src[14] -
		src[9] * src[6] * src[15] +
		src[9] * src[7] * src[14] +
		src[13] * src[6] * src[11] -
		src[13] * src[7] * src[10];

	inv[4] = -src[4] * src[10] * src[15] +
		src[4] * src[11] * src[14] +
		src[8] * src[6] * src[15] -
		src[8] * src[7] * src[14] -
		src[12] * src[6] * src[11] +
		src[12] * src[7] * src[10];

	inv[8] = src[4] * src[9] * src[15] -
		src[4] * src[11] * src[13] -
		src[8] * src[5] * src[15] +
		src[8] * src[7] * src[13] +
		src[12] * src[5] * src[11] -
		src[12] * src[7] * src[9];

	inv[12] = -src[4] * src[9] * src[14] +
		src[4] * src[10] * src[13] +
		src[8] * src[5] * src[14] -
		src[8] * src[6] * src[13] -
		src[12] * src[5] * src[10] +
		src[12] * src[6] * src[9];

	inv[1] = -src[1] * src[10] * src[15] +
		src[1] * src[11] * src[14] +
		src[9] * src[2] * src[15] -
		src[9] * src[3] * src[14] -
		src[13] * src[2] * src[11] +
		src[13] * src[3] * src[10];

	inv[5] = src[0] * src[10] * src[15] -
		src[0] * src[11] * src[14] -
		src[8] * src[2] * src[15] +
		src[8] * src[3] * src[14] +
		src[12] * src[2] * src[11] -
		src[12] * src[3] * src[10];

	inv[9] = -src[0] * src[9] * src[15] +
		src[0] * src[11] * src[13] +
		src[8] * src[1] * src[15] -
		src[8] * src[3] * src[13] -
		src[12] * src[1] * src[11] +
		src[12] * src[3] * src[9];

	inv[13] = src[0] * src[9] * src[14] -
		src[0] * src[10] * src[13] -
		src[8] * src[1] * src[14] +
		src[8] * src[2] * src[13] +
		src[12] * src[1] * src[10] -
		src[12] * src[2] * src[9];

	inv[2] = src[1] * src[6] * src[15] -
		src[1] * src[7] * src[14] -
		src[5] * src[2] * src[15] +
		src[5] * src[3] * src[14] +
		src[13] * src[2] * src[7] -
		src[13] * src[3] * src[6];

	inv[6] = -src[0] * src[6] * src[15] +
		src[0] * src[7] * src[14] +
		src[4] * src[2] * src[15] -
		src[4] * src[3] * src[14] -
		src[12] * src[2] * src[7] +
		src[12] * src[3] * src[6];

	inv[10] = src[0] * src[5] * src[15] -
		src[0] * src[7] * src[13] -
		src[4] * src[1] * src[15] +
		src[4] * src[3] * src[13] +
		src[12] * src[1] * src[7] -
		src[12] * src[3] * src[5];

	inv[14] = -src[0] * src[5] * src[14] +
		src[0] * src[6] * src[13] +
		src[4] * src[1] * src[14] -
		src[4] * src[2] * src[13] -
		src[12] * src[1] * src[6] +
		src[12] * src[2] * src[5];

	inv[3] = -src[1] * src[6] * src[11] +
		src[1] * src[7] * src[10] +
		src[5] * src[2] * src[11] -
		src[5] * src[3] * src[10] -
		src[9] * src[2] * src[7] +
		src[9] * src[3] * src[6];

	inv[7] = src[0] * src[6] * src[11] -
		src[0] * src[7] * src[10] -
		src[4] * src[2] * src[11] +
		src[4] * src[3] * src[10] +
		src[8] * src[2] * src[7] -
		src[8] * src[3] * src[6];

	inv[11] = -src[0] * src[5] * src[11] +
		src[0] * src[7] * src[9] +
		src[4] * src[1] * src[11] -
		src[4] * src[3] * src[9] -
		src[8] * src[1] * src[7] +
		src[8] * src[3] * src[5];

	inv[15] = src[0] * src[5] * src[10] -
		src[0] * src[6] * src[9] -
		src[4] * src[1] * src[10] +
		src[4] * src[2] * src[9] +
		src[8] * src[1] * src[6] -
		src[8] * src[2] * src[5];

	det = src[0] * inv[0] + src[1] * inv[4] + src[2] * inv[8] + src[3] * inv[12];

	det = 1.0f / det;

	Matrix4x4 resultMatrix;
	for (int i = 0; i < 16; ++i)
	{
		((float*)resultMatrix.m)[i] = inv[i] * det;
	}

	return resultMatrix;
}


// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 matrix = {};

	// 単位行列
	for (int i = 0; i < 4; ++i)
	{
		matrix.m[i][i] = 1.0f;
	}

	// 平行移動成分を設定
	matrix.m[3][0] = translate.x;
	matrix.m[3][1] = translate.y;
	matrix.m[3][2] = translate.z;

	return matrix;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 matrix = {};

	// 拡大縮小成分を設定
	matrix.m[0][0] = scale.x;
	matrix.m[1][1] = scale.y;
	matrix.m[2][2] = scale.z;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian)
{
	Matrix4x4 matrix = {};

	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = std::cosf(radian);
	matrix.m[1][2] = std::sinf(radian);
	matrix.m[2][1] = -std::sinf(radian);
	matrix.m[2][2] = std::cosf(radian);
	matrix.m[3][3] = 1.0f;

	return matrix;
}

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian)
{
	Matrix4x4 matrix = {};

	matrix.m[0][0] = std::cosf(radian);
	matrix.m[0][2] = -std::sinf(radian);
	matrix.m[1][1] = 1.0f;
	matrix.m[2][0] = std::sinf(radian);
	matrix.m[2][2] = std::cosf(radian);
	matrix.m[3][3] = 1.0f;

	return matrix;
}

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian)
{
	Matrix4x4 matrix = {};

	matrix.m[0][0] = std::cosf(radian);
	matrix.m[0][1] = std::sinf(radian);
	matrix.m[1][0] = -std::sinf(radian);
	matrix.m[1][1] = std::cosf(radian);
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	float x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
	float y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
	float z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];

	if (w != 0.0f && w != 1.0f) {
		x /= w;
		y /= w;
		z /= w;
	}

	return { x, y, z };
}

// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	// 回転行列を合成
	Matrix4x4 rotateMatrix = Multiply(Multiply(rotateXMatrix, rotateYMatrix), rotateZMatrix);

	// 最終的なアフィン行列
	Matrix4x4 affineMatrix = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);

	return affineMatrix;
}

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	Matrix4x4 matrix = {};

	matrix.m[0][0] = 2.0f / (right - left);
	matrix.m[1][1] = 2.0f / (top - bottom);
	matrix.m[2][2] = 1.0f / (farClip - nearClip);
	matrix.m[3][0] = (right + left) / (left - right);
	matrix.m[3][1] = (top + bottom) / (bottom - top);
	matrix.m[3][2] = nearClip / (nearClip - farClip);
	matrix.m[3][3] = 1.0f;

	return matrix;
}

//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	Matrix4x4 matrix = {};
	float f = 1.0f / std::tanf(fovY * 0.5f);
	matrix.m[0][0] = f / aspectRatio;
	matrix.m[1][1] = f;
	matrix.m[2][2] = farClip / (farClip - nearClip);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][2] = (-farClip * nearClip) / (farClip - nearClip);
	return matrix;
}

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 matrix = {};

	matrix.m[0][0] = width / 2.0f;
	matrix.m[1][1] = -height / 2.0f;
	matrix.m[2][2] = maxDepth - minDepth;
	matrix.m[3][0] = left + width / 2.0f;
	matrix.m[3][1] = top + height / 2.0f;
	matrix.m[3][2] = minDepth;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

// クロス積を求める関数
Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

// グリッドを描画する関数
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	// グリッドの半分の幅
	const float kGridHalfWidth = 2.0f;
	// 分割数
	const float kSubdivision = 10.0f;
	// 1つ分の長さ
	const float kGridEvery = (kGridHalfWidth * 2.0f) / kSubdivision;
	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex)
	{
		// 上の情報を使ってワールド座標系上の始点と終点を求める
		float x = -kGridHalfWidth + xIndex * kGridEvery;
		Vector3 start = { x, 0.0f, -kGridHalfWidth }; // 奥
		Vector3 end = { x, 0.0f, +kGridHalfWidth }; // 手前

		start = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		end = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		Novice::DrawLine(static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x), static_cast<int>(end.y), RED);
	}

	// 左から右への線を順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex)
	{
		// 上の情報を使ってワールド座標系上の始点と終点を求める
		float z = -kGridHalfWidth + zIndex * kGridEvery;
		Vector3 start = { -kGridHalfWidth, 0.0f, z }; // 左
		Vector3 end = { +kGridHalfWidth, 0.0f, z }; // 右

		start = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		end = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		Novice::DrawLine(static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x), static_cast<int>(end.y), RED);
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// 分割数
	const uint32_t kSubdivision = 20;
	// 経度分割一つ分の角度
	const float kLonEvery = (2.0f * float(M_PI)) / float(kSubdivision);
	// 緯度分割一つ分の角度
	const float kLatEvery = (float(M_PI)) / float(kSubdivision);
	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = kLonEvery * lonIndex;

			// 各点を計算
			Vector3 p1 = {
				sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon),
				sphere.center.y + sphere.radius * std::sinf(lat),
				sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon)
			};
			Vector3 p2 = {
				sphere.center.x + sphere.radius * std::cosf(lat + kLatEvery) * std::cosf(lon),
				sphere.center.y + sphere.radius * std::sinf(lat + kLatEvery),
				sphere.center.z + sphere.radius * std::cosf(lat + kLatEvery) * std::sinf(lon)
			};
			Vector3 p3 = {
				sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon + kLonEvery),
				sphere.center.y + sphere.radius * std::sinf(lat),
				sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon + kLonEvery)
			};

			p1 = Transform(Transform(p1, viewProjectionMatrix), viewportMatrix);
			p2 = Transform(Transform(p2, viewProjectionMatrix), viewportMatrix);
			p3 = Transform(Transform(p3, viewProjectionMatrix), viewportMatrix);

			// 経度方向の線
			Novice::DrawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, color);
			// 緯度方向の線
			Novice::DrawLine((int)p1.x, (int)p1.y, (int)p3.x, (int)p3.y, color);
		}
	}
}

// 正射影ベクトルを求める関数
Vector3 Project(const Vector3& v1, const Vector3& v2)
{
	float dot = Vector3::Dot(v1, v2); // v1とv2の内積
	float v2LengthSquared = Vector3::Dot(v2, v2); // v2の長さの2乗
	float scalar = dot / v2LengthSquared; // スカラー係数
	return { v2.x * scalar, v2.y * scalar, v2.z * scalar }; // 正射影ベクトル
}

// 最近接点を求める関数
Vector3 CrosestPoint(const Vector3& point, const Segment& segment)
{
	Vector3 segmentVec = { segment.diff.x - segment.origin.x, segment.diff.y - segment.origin.y, segment.diff.z - segment.origin.z };
	Vector3 startToPoint = { point.x - segment.origin.x, point.y - segment.origin.y ,point.z - segment.origin.z };

	float t = Vector3::Dot(startToPoint, segmentVec) / Vector3::Dot(segmentVec, segmentVec);

	// 0.0fから1.0f
	t = std::clamp(t, 0.0f, 1.0f);

	return { segment.origin.x + segmentVec.x * t, segment.origin.y + segmentVec.y * t, segment.origin.z + segmentVec.z * t };
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
	// v1 と v2 の各成分を引き算して、新しいベクトルを返す
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	// v1 と v2 の各成分を足し算して、新しいベクトルを返す
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	float fovY = 0.45f;
	float aspectRatio = 1280.0f / 720.0f;
	float nearClip = 0.1f;
	float farClip = 100.0f;

	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 translate = { 0.0f,0.0f,0.0 };
	Vector3 cameraScale = { 1.0f,1.0f,1.0f };
	Vector3 cameraRotate = { 0.26f,0.0f,0.0f };
	Vector3 cameraTranslate = { 0.0f,1.9f,-6.49f };

	Segment segment{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} };
	Vector3 center1{ -1.5f,0.6f,0.6f };
	Vector3 center2{ 1.0f,0.6f,0.6f };

	Vector3 project = Project(Subtract(center1, segment.origin), segment.diff);
	Vector3 closestPoint = CrosestPoint(center1, segment);

	// 速度
	const float moveSpeed = 0.1f;

	Sphere sphere1{ center1,0.4f };// 1cmの球を描画
	Sphere sphere2{ center2,0.4f };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// キー入力による移動
		if (keys[DIK_W]) {
			cameraTranslate.y += moveSpeed;
		}
		if (keys[DIK_S]) {
			cameraTranslate.y -= moveSpeed;
		}
		if (keys[DIK_A]) {
			cameraTranslate.x -= moveSpeed;

		}
		if (keys[DIK_D]) {
			cameraTranslate.x += moveSpeed;
		}

		if (keys[DIK_Q]) {
			cameraTranslate.z += moveSpeed;
		}

		if (keys[DIK_E]) {
			cameraTranslate.z -= moveSpeed;
		}
		Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraScale, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(fovY, aspectRatio, nearClip, farClip);
		// WVPMatrixを作る
		Matrix4x4 worldViewProjectionMatrix = Multiply(Multiply(worldMatrix, viewMatrix), projectionMatrix);

		// ViewportMatrixを作る
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, 1280, 720, 0.0f, 1.0f);

		Vector3 start = Transform(Transform(segment.origin, worldViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(segment.diff, worldViewProjectionMatrix), viewportMatrix);

		if (isCollision(sphere1, sphere2))
		{
			sphere1.color = RED;
		}
		else
		{
			sphere1.color = BLACK;
		}

		///
		/// ↑更新処理ここまで
		///
		/// 

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		DrawSphere(sphere1, worldViewProjectionMatrix, viewportMatrix, sphere1.color);
		DrawSphere(sphere2, worldViewProjectionMatrix, viewportMatrix, BLACK);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

		ImGui::Begin("MyWindow");

		ImGui::DragFloat3("sphere1.position", &sphere1.center.x, 0.07f, 0, 1280);
		ImGui::DragFloat3("sphere2.position", &sphere2.center.x, 0.07f, 0, 1280);
		ImGui::InputFloat3("Segment origin", &segment.origin.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("Segment diff", &segment.diff.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);

		ImGui::End();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
