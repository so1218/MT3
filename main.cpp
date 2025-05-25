#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include "imgui.h"
#include "Structures.h"
#include "Matrix.h"

const char kWindowTitle[] = "LE2A_12_ホリ_ソウヘイ_タイトル";

bool isCollision(const Sphere& s1, const Sphere& s2)
{
	float dx = s2.center.x - s1.center.x;
	float dy = s2.center.y - s1.center.y;
	float dz = s2.center.z - s1.center.z;

	float radiusSum = s1.radius + s2.radius;

	return dx * dx + dy * dy + dz * dz <= radiusSum * radiusSum;
}



void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
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

		Novice::DrawLine(static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x), static_cast<int>(end.y), 0x555555ff);
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

		Novice::DrawLine(static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x), static_cast<int>(end.y), 0x555555ff);
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

bool IsCollision(const Sphere& sphere, const Plane& plane)
{
	// 球の中心と平面の距離を計算
	float distanceFromCenterToPlane = std::abs(
		Dot(plane.normal, sphere.center) - plane.distance
	);

	// 球の半径以下なら衝突とみなす
	return distanceFromCenterToPlane <= sphere.radius;
}

bool IsCollision(const Segment& segment, const Plane& plane)
{
	// 線の方向ベクトルと平面の法線ベクトルの内積を計算
	float dot = Dot(plane.normal, segment.diff);

	// 線が平面と平行な場合
	if (dot == 0.0f)
	{
		return false;
	}

	// 交点までのパラメーター t を計算
	float t = (plane.distance - Dot(plane.normal, segment.origin)) / dot;

	// 交点が線分上にあるかチェック
	return t >= 0.0f && t <= 1.0f;
}

Vector3 Perpendicular(const Vector3& vector)
{
	if (vector.x != 0.0f || vector.y != 0.0f)
	{
		return{ -vector.y,vector.x,0.0f };
	}
	return{ 0.0f,-vector.z, vector.y };
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 center = Multiply(plane.distance, plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index)
	{
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	// 四角形として線を描画
	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[2].x, (int)points[2].y, color);
	Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[3].x, (int)points[3].y, color);
	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[3].x, (int)points[3].y, color);
	Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[2].x, (int)points[2].y, color);
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
	Vector3 cameraRotate = { 0.05f,0.0f,0.0f };
	Vector3 cameraTranslate = { 0.0f,1.0f,-6.49f };

	Segment segment;
	segment.origin = { 0.0f, 0.33f, 0.45f };
	segment.diff = { 0.3f, 0.58f, 0.45f };
	Vector3 center1{ -1.5f,0.6f,0.6f };
	Vector3 center2{ 1.0f,0.6f,0.6f };

	Vector3 project = Project(Subtract(center1, segment.origin), segment.diff);
	Vector3 closestPoint = CrosestPoint(center1, segment);

	// 速度
	const float moveSpeed = 0.1f;


	Sphere sphere1{ center1,0.4f };// 1cmの球を描画
	Sphere sphere2{ center2,0.4f };

	Plane plane{ {0,1,0},1 };


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
		// 線の終点を計算する
		Vector3 segmentTrueEnd = Add(segment.origin, segment.diff);

		// start と end をワールド座標系からスクリーン座標系へ変換
		Vector3 start = Transform(Transform(segment.origin, worldViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(segmentTrueEnd, worldViewProjectionMatrix), viewportMatrix); // 修正後

		if (isCollision(sphere1, sphere2))
		{
			sphere1.color = RED;
		}
		else
		{
			sphere1.color = BLACK;
		}
		plane.normal = Normalize(plane.normal);
		if (IsCollision(sphere1, plane))
		{
			sphere1.color = RED;
		}

		if (IsCollision(segment, plane))
		{
			segment.color = RED;
		}
		else
		{
			segment.color = WHITE;
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
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), segment.color);
		DrawPlane(plane, worldViewProjectionMatrix, viewportMatrix, BLACK);

		ImGui::Begin("MyWindow");
		ImGui::DragFloat3("sphere1.position", &sphere1.center.x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("sphere2.position", &sphere2.center.x, 0.07f, 0, 1280);
		ImGui::DragFloat3("Segment origin", &segment.origin.x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("Segment diff", &segment.diff.x, 0.07f, -1280, 1280);
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::DragFloat3("normal", &plane.normal.x, 0.07f, -1, 1);
		ImGui::DragFloat("distance", &plane.distance, 0.07f, 0, 1280);
		ImGui::DragFloat3("cameraScale", &cameraScale.x, 0.07f, 0, 10);
		ImGui::DragFloat3("cameraRotate", &cameraRotate.x, 0.07f, -10, 10);
		ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.07f, -1280, 1280);

		ImGui::End();


		plane.normal = Normalize(plane.normal);

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
