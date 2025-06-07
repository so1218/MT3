#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include "imgui.h"

#include "Structures.h"
#include "Matrix.h"
#include "MathUtils.h"
#include "Collision.h"
#include "Draw.h"

const char kWindowTitle[] = "LE2A_12_ホリ_ソウヘイ_タイトル";

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
	Vector3 translate = { 0.0f,0.0f,0.0f };
	Vector3 cameraScale = { 1.0f,1.0f,1.0f };
	Vector3 cameraRotate = { 0.2f,0.0f,0.0f };
	Vector3 cameraTranslate = { 0.0f,2.0f,-10.0f };

	Segment segment;
	segment.origin = { -1.12f, 0.12f, -1.33f };
	segment.diff = { -1.12f, 0.58f, 0.14f };
	Vector3 center1{ 1.160f,0.6f,-1.290f };
	Vector3 center2{ 1.0f,0.6f,0.6f };

	Vector3 project = Project(Subtract(center1, segment.origin), segment.diff);
	Vector3 closestPoint = CrosestPoint(center1, segment);

	// 速度
	const float moveSpeed = 0.1f;

	Sphere sphere1{ center1,0.4f };// 1cmの球を描画
	Sphere sphere2{ center2,0.4f };

	Plane plane{ {0,1,0},1 };

	Triangle triangle;
	triangle.vertices[0] = { 0, 0, 1 };
	triangle.vertices[1] = { 0, 1, 0 };
	triangle.vertices[2] = { 0, 0, -1 };

	AABB aabb1
	{
		.min{-2.25f,-0.14f,-0.99f},
		.max{-0.70f,0.910f,1.54f}
	};

	/*AABB aabb2
	{
		.min{0.2f,0.2f,0.2f},
		.max{1.0f,1.0f,1.0f}
	};*/
	Vector3 controlPoints[3]
	{
		{ -0.8f, 0.58f, 1.0f },
		{ 1.76f, 1.0f, -0.3f },
		{ 0.94f, -0.7f, 2.3f }
	};

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

		sphere1.color = WHITE;
		/*if (isCollision(sphere1, sphere2))
		{
			sphere1.color = RED;
		}
		else
		{
			sphere1.color = BLACK;
		}*/
		plane.normal = Normalize(plane.normal);
		/*if (IsCollision(sphere1, plane))
		{
			sphere1.color = RED;
		}*/

		if (IsCollision(aabb1, segment))
		{
			aabb1.color = RED;
		}
		else
		{
			aabb1.color = WHITE;
		}

	/*	if (IsCollision(triangle, segment))
		{
			segment.color = RED;
		}
		else
		{
			segment.color = WHITE;
		}

		if (IsCollision(aabb1, sphere1))
		{
			aabb1.color = RED;
		}
		else
		{
			aabb1.color = WHITE;
		}*/

		///
		/// ↑更新処理ここまで
		///
		/// 

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		/*DrawSphere(sphere1, worldViewProjectionMatrix, viewportMatrix, sphere1.color);*/
		/*DrawSphere(sphere2, worldViewProjectionMatrix, viewportMatrix, BLACK);*/
		/*Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), segment.color);*/
		/*DrawPlane(plane, worldViewProjectionMatrix, viewportMatrix, BLACK);*/
		/*DrawTriangle(triangle, worldViewProjectionMatrix, viewportMatrix, WHITE);*/
		/*DrawAABB(aabb1, worldViewProjectionMatrix, viewportMatrix, aabb1.color);*/
		/*DrawAABB(aabb2, worldViewProjectionMatrix, viewportMatrix, WHITE);*/
		DrawBezierCurve(controlPoints[0], controlPoints[1], controlPoints[2],
			worldViewProjectionMatrix, viewportMatrix, WHITE);
		ImGui::Begin("MyWindow");
		/*ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.07f, -1280, 1280);*/
		aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
		aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);
		aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
		aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);
		aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);
		aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);
		/*ImGui::DragFloat3("aabb2.min", &aabb2.min.x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("aabb2.max", &aabb2.max.x, 0.07f, -1280, 1280);*/
		/*aabb2.min.x = (std::min)(aabb2.min.x, aabb2.max.x);
		aabb2.max.x = (std::max)(aabb2.min.x, aabb2.max.x);
		aabb2.min.y = (std::min)(aabb2.min.y, aabb2.max.y);
		aabb2.max.y = (std::max)(aabb2.min.y, aabb2.max.y);
		aabb2.min.z = (std::min)(aabb2.min.z, aabb2.max.z);
		aabb2.max.z = (std::max)(aabb2.min.z, aabb2.max.z);*/
		/*ImGui::DragFloat3("triangle.v0", &triangle.vertices[0].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("triangle.v1", &triangle.vertices[1].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("triangle.v2", &triangle.vertices[2].x, 0.07f, -1280, 1280);*/
		/*ImGui::DragFloat3("sphere1.position", &sphere1.center.x, 0.07f, -1280, 1280);*/
		/*ImGui::DragFloat3("sphere2.position", &sphere2.center.x, 0.07f, 0, 1280);*/
		/*ImGui::DragFloat3("Segment origin", &segment.origin.x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("Segment diff", &segment.diff.x, 0.07f, -1280, 1280);*/
		/*ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::DragFloat3("normal", &plane.normal.x, 0.07f, -1, 1);
		ImGui::DragFloat("distance", &plane.distance, 0.07f, 0, 1280);*/
		ImGui::DragFloat3("controlPoints[0]", &controlPoints[0].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("controlPoints[1]", &controlPoints[1].x, 0.07f, -1280, 1280);
		ImGui::DragFloat3("controlPoints[2]", &controlPoints[2].x, 0.07f, -1280, 1280);
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
